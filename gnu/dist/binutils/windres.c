/* windres.c -- a program to manipulate Windows resources
   Copyright 1997, 1998 Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Cygnus Support.

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/* This program can read and write Windows resources in various
   formats.  In particular, it can act like the rc resource compiler
   program, and it can act like the cvtres res to COFF conversion
   program.

   It is based on information taken from the following sources:

   * Microsoft documentation.

   * The rcl program, written by Gunther Ebert
     <gunther.ebert@ixos-leipzig.de>.

   * The res2coff program, written by Pedro A. Aranda <paag@tid.es>.

   */

#include "bfd.h"
#include "getopt.h"
#include "bucomm.h"
#include "libiberty.h"
#include "obstack.h"
#include "windres.h"

#include <assert.h>
#include <ctype.h>
#include <time.h>

/* An enumeration of format types.  */

enum res_format
{
  /* Unknown format.  */
  RES_FORMAT_UNKNOWN,
  /* Textual RC file.  */
  RES_FORMAT_RC,
  /* Binary RES file.  */
  RES_FORMAT_RES,
  /* COFF file.  */
  RES_FORMAT_COFF
};

/* A structure used to map between format types and strings.  */

struct format_map
{
  const char *name;
  enum res_format format;
};

/* A mapping between names and format types.  */

static const struct format_map format_names[] =
{
  { "rc", RES_FORMAT_RC },
  { "res", RES_FORMAT_RES },
  { "coff", RES_FORMAT_COFF },
  { NULL, RES_FORMAT_UNKNOWN }
};

/* A mapping from file extensions to format types.  */

static const struct format_map format_fileexts[] =
{
  { "rc", RES_FORMAT_RC },
  { "res", RES_FORMAT_RES },
  { "exe", RES_FORMAT_COFF },
  { "obj", RES_FORMAT_COFF },
  { "o", RES_FORMAT_COFF },
  { NULL, RES_FORMAT_UNKNOWN }
};

/* A list of include directories.  */

struct include_dir
{
  struct include_dir *next;
  char *dir;
};

static struct include_dir *include_dirs;

/* Long options.  */

/* 150 isn't special; it's just an arbitrary non-ASCII char value.  */

#define OPTION_DEFINE 150
#define OPTION_HELP (OPTION_DEFINE + 1)
#define OPTION_INCLUDE_DIR (OPTION_HELP + 1)
#define OPTION_LANGUAGE (OPTION_INCLUDE_DIR + 1)
#define OPTION_PREPROCESSOR (OPTION_LANGUAGE + 1)
#define OPTION_VERSION (OPTION_PREPROCESSOR + 1)
#define OPTION_YYDEBUG (OPTION_VERSION + 1)

static const struct option long_options[] =
{
  {"define", required_argument, 0, OPTION_DEFINE},
  {"help", no_argument, 0, OPTION_HELP},
  {"include-dir", required_argument, 0, OPTION_INCLUDE_DIR},
  {"input-format", required_argument, 0, 'I'},
  {"language", required_argument, 0, OPTION_LANGUAGE},
  {"output-format", required_argument, 0, 'O'},
  {"preprocessor", required_argument, 0, OPTION_PREPROCESSOR},
  {"target", required_argument, 0, 'F'},
  {"version", no_argument, 0, OPTION_VERSION},
  {"yydebug", no_argument, 0, OPTION_YYDEBUG},
  {0, no_argument, 0, 0}
};

/* Static functions.  */

static void res_init PARAMS ((void));
static int extended_menuitems PARAMS ((const struct menuitem *));
static enum res_format format_from_name PARAMS ((const char *));
static enum res_format format_from_filename PARAMS ((const char *, int));
static void usage PARAMS ((FILE *, int));
static int cmp_res_entry PARAMS ((const PTR, const PTR));
static struct res_directory *sort_resources PARAMS ((struct res_directory *));

/* When we are building a resource tree, we allocate everything onto
   an obstack, so that we can free it all at once if we want.  */

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

/* The resource building obstack.  */

static struct obstack res_obstack;

/* Initialize the resource building obstack.  */

static void
res_init ()
{
  obstack_init (&res_obstack);
}

/* Allocate space on the resource building obstack.  */

PTR
res_alloc (bytes)
     size_t bytes;
{
  return (PTR) obstack_alloc (&res_obstack, bytes);
}

/* We also use an obstack to save memory used while writing out a set
   of resources.  */

static struct obstack reswr_obstack;

/* Initialize the resource writing obstack.  */

static void
reswr_init ()
{
  obstack_init (&reswr_obstack);
}

/* Allocate space on the resource writing obstack.  */

PTR
reswr_alloc (bytes)
     size_t bytes;
{
  return (PTR) obstack_alloc (&reswr_obstack, bytes);
}

/* Open a file using the include directory search list.  */

FILE *
open_file_search (filename, mode, errmsg, real_filename)
     const char *filename;
     const char *mode;
     const char *errmsg;
     char **real_filename;
{
  FILE *e;
  struct include_dir *d;

  e = fopen (filename, mode);
  if (e != NULL)
    {
      *real_filename = xstrdup (filename);
      return e;
    }

  if (errno == ENOENT)
    {
      for (d = include_dirs; d != NULL; d = d->next)
	{
	  char *n;

	  n = (char *) xmalloc (strlen (d->dir) + strlen (filename) + 2);
	  sprintf (n, "%s/%s", d->dir, filename);
	  e = fopen (n, mode);
	  if (e != NULL)
	    {
	      *real_filename = n;
	      return e;
	    }

	  if (errno != ENOENT)
	    break;
	}
    }

  fatal ("can't open %s `%s': %s", errmsg, filename, strerror (errno));

  /* Return a value to avoid a compiler warning.  */
  return NULL;
}

/* Unicode support.  */

/* Convert an ASCII string to a unicode string.  We just copy it,
   expanding chars to shorts, rather than doing something intelligent.  */

void
unicode_from_ascii (length, unicode, ascii)
     int *length;
     unichar **unicode;
     const char *ascii;
{
  int len;
  const char *s;
  unsigned short *w;

  len = strlen (ascii);

  if (length != NULL)
    *length = len;

  *unicode = ((unichar *) res_alloc ((len + 1) * sizeof (unichar)));

  for (s = ascii, w = *unicode; *s != '\0'; s++, w++)
    *w = *s & 0xff;
  *w = 0;
}

/* Print the unicode string UNICODE to the file E.  LENGTH is the
   number of characters to print, or -1 if we should print until the
   end of the string.  */

void
unicode_print (e, unicode, length)
     FILE *e;
     const unichar *unicode;
     int length;
{
  while (1)
    {
      unichar ch;

      if (length == 0)
	return;
      if (length > 0)
	--length;

      ch = *unicode;

      if (ch == 0 && length < 0)
	return;

      ++unicode;

      if ((ch & 0x7f) == ch)
	{
	  if (ch == '\\')
	    fputs ("\\", e);
	  else if (isprint (ch))
	    putc (ch, e);
	  else
	    {
	      switch (ch)
		{
		case ESCAPE_A:
		  fputs ("\\a", e);
		  break;

		case ESCAPE_B:
		  fputs ("\\b", e);
		  break;

		case ESCAPE_F:
		  fputs ("\\f", e);
		  break;

		case ESCAPE_N:
		  fputs ("\\n", e);
		  break;

		case ESCAPE_R:
		  fputs ("\\r", e);
		  break;

		case ESCAPE_T:
		  fputs ("\\t", e);
		  break;

		case ESCAPE_V:
		  fputs ("\\v", e);
		  break;

		default:
		  fprintf (e, "\\%03o", (unsigned int) ch);
		  break;
		}
	    }
	}
      else if ((ch & 0xff) == ch)
	fprintf (e, "\\%03o", (unsigned int) ch);
      else
	fprintf (e, "\\x%x", (unsigned int) ch);
    }
}

/* Compare two resource ID's.  We consider name entries to come before
   numeric entries, because that is how they appear in the COFF .rsrc
   section.  */

int
res_id_cmp (a, b)
     struct res_id a;
     struct res_id b;
{
  if (! a.named)
    {
      if (b.named)
	return 1;
      if (a.u.id > b.u.id)
	return 1;
      else if (a.u.id < b.u.id)
	return -1;
      else
	return 0;
    }
  else
    {
      unichar *as, *ase, *bs, *bse;

      if (! b.named)
	return -1;

      as = a.u.n.name;
      ase = as + a.u.n.length;
      bs = b.u.n.name;
      bse = bs + b.u.n.length;

      while (as < ase)
	{
	  int i;

	  if (bs >= bse)
	    return 1;
	  i = (int) *as - (int) *bs;
	  if (i != 0)
	    return i;
	  ++as;
	  ++bs;
	}

      if (bs < bse)
	return -1;

      return 0;
    }
}

/* Print a resource ID.  */

void
res_id_print (stream, id, quote)
     FILE *stream;
     struct res_id id;
     int quote;
{
  if (! id.named)
    fprintf (stream, "%lu", id.u.id);
  else
    {
      if (quote)
	putc ('"', stream);
      unicode_print (stream, id.u.n.name, id.u.n.length);
      if (quote)
	putc ('"', stream);
    }
}

/* Print a list of resource ID's.  */

void
res_ids_print (stream, cids, ids)
     FILE *stream;
     int cids;
     const struct res_id *ids;
{
  int i;

  for (i = 0; i < cids; i++)
    {
      res_id_print (stream, ids[i], 1);
      if (i + 1 < cids)
	fprintf (stream, ": ");
    }
}

/* Convert an ASCII string to a resource ID.  */

void
res_string_to_id (res_id, string)
     struct res_id *res_id;
     const char *string;
{
  res_id->named = 1;
  unicode_from_ascii (&res_id->u.n.length, &res_id->u.n.name, string);
}

/* Define a resource.  The arguments are the resource tree, RESOURCES,
   and the location at which to put it in the tree, CIDS and IDS.
   This returns a newly allocated res_resource structure, which the
   caller is expected to initialize.  If DUPOK is non-zero, then if a
   resource with this ID exists, it is returned.  Otherwise, a warning
   is issued, and a new resource is created replacing the existing
   one.  */

struct res_resource *
define_resource (resources, cids, ids, dupok)
     struct res_directory **resources;
     int cids;
     const struct res_id *ids;
     int dupok;
{
  struct res_entry *re = NULL;
  int i;

  assert (cids > 0);
  for (i = 0; i < cids; i++)
    {
      struct res_entry **pp;

      if (*resources == NULL)
	{
	  static unsigned long timeval;

	  /* Use the same timestamp for every resource created in a
             single run.  */
	  if (timeval == 0)
	    timeval = time (NULL);

	  *resources = ((struct res_directory *)
			res_alloc (sizeof **resources));
	  (*resources)->characteristics = 0;
	  (*resources)->time = timeval;
	  (*resources)->major = 0;
	  (*resources)->minor = 0;
	  (*resources)->entries = NULL;
	}

      for (pp = &(*resources)->entries; *pp != NULL; pp = &(*pp)->next)
	if (res_id_cmp ((*pp)->id, ids[i]) == 0)
	  break;

      if (*pp != NULL)
	re = *pp;
      else
	{
	  re = (struct res_entry *) res_alloc (sizeof *re);
	  re->next = NULL;
	  re->id = ids[i];
	  if ((i + 1) < cids)
	    {
	      re->subdir = 1;
	      re->u.dir = NULL;
	    }
	  else
	    {
	      re->subdir = 0;
	      re->u.res = NULL;
	    }

	  *pp = re;
	}

      if ((i + 1) < cids)
	{
	  if (! re->subdir)
	    {
	      fprintf (stderr, "%s: ", program_name);
	      res_ids_print (stderr, i, ids);
	      fprintf (stderr, ": expected to be a directory\n");
	      xexit (1);
	    }

	  resources = &re->u.dir;
	}
    }

  if (re->subdir)
    {
      fprintf (stderr, "%s: ", program_name);
      res_ids_print (stderr, cids, ids);
      fprintf (stderr, ": expected to be a leaf\n");
      xexit (1);
    }

  if (re->u.res != NULL)
    {
      if (dupok)
	return re->u.res;

      fprintf (stderr, "%s: warning: ", program_name);
      res_ids_print (stderr, cids, ids);
      fprintf (stderr, ": duplicate value\n");
    }

  re->u.res = ((struct res_resource *)
	       res_alloc (sizeof (struct res_resource)));

  re->u.res->type = RES_TYPE_UNINITIALIZED;
  memset (&re->u.res->res_info, 0, sizeof (struct res_res_info));
  memset (&re->u.res->coff_info, 0, sizeof (struct res_coff_info));

  return re->u.res;
}

/* Define a standard resource.  This is a version of define_resource
   that just takes type, name, and language arguments.  */

struct res_resource *
define_standard_resource (resources, type, name, language, dupok)
     struct res_directory **resources;
     int type;
     struct res_id name;
     int language;
     int dupok;
{
  struct res_id a[3];

  a[0].named = 0;
  a[0].u.id = type;
  a[1] = name;
  a[2].named = 0;
  a[2].u.id = language;
  return define_resource (resources, 3, a, dupok);
}

/* Comparison routine for resource sorting.  */

static int
cmp_res_entry (p1, p2)
     const PTR p1;
     const PTR p2;
{
  const struct res_entry **re1, **re2;

  re1 = (const struct res_entry **) p1;
  re2 = (const struct res_entry **) p2;
  return res_id_cmp ((*re1)->id, (*re2)->id);
}

/* Sort the resources.  */

static struct res_directory *
sort_resources (resdir)
     struct res_directory *resdir;
{
  int c, i;
  struct res_entry *re;
  struct res_entry **a;

  if (resdir->entries == NULL)
    return resdir;

  c = 0;
  for (re = resdir->entries; re != NULL; re = re->next)
    ++c;

  /* This is a recursive routine, so using xmalloc is probably better
     than alloca.  */
  a = (struct res_entry **) xmalloc (c * sizeof (struct res_entry *));

  for (i = 0, re = resdir->entries; re != NULL; re = re->next, i++)
    a[i] = re;

  qsort (a, c, sizeof (struct res_entry *), cmp_res_entry);

  resdir->entries = a[0];
  for (i = 0; i < c - 1; i++)
    a[i]->next = a[i + 1];
  a[i]->next = NULL;

  free (a);

  /* Now sort the subdirectories.  */

  for (re = resdir->entries; re != NULL; re = re->next)
    if (re->subdir)
      re->u.dir = sort_resources (re->u.dir);

  return resdir;
}

/* Return whether the dialog resource DIALOG is a DIALOG or a
   DIALOGEX.  */

int
extended_dialog (dialog)
     const struct dialog *dialog;
{
  const struct dialog_control *c;

  if (dialog->ex != NULL)
    return 1;

  for (c = dialog->controls; c != NULL; c = c->next)
    if (c->data != NULL || c->help != 0)
      return 1;

  return 0;
}

/* Return whether MENUITEMS are a MENU or a MENUEX.  */

int
extended_menu (menu)
     const struct menu *menu;
{
  return extended_menuitems (menu->items);
}

static int
extended_menuitems (menuitems)
     const struct menuitem *menuitems;
{
  const struct menuitem *mi;

  for (mi = menuitems; mi != NULL; mi = mi->next)
    {
      if (mi->help != 0 || mi->state != 0)
	return 1;
      if (mi->popup != NULL && mi->id != 0)
	return 1;
      if ((mi->type
	   & ~ (MENUITEM_CHECKED
		| MENUITEM_GRAYED
		| MENUITEM_HELP
		| MENUITEM_INACTIVE
		| MENUITEM_MENUBARBREAK
		| MENUITEM_MENUBREAK))
	  != 0)
	return 1;
      if (mi->popup != NULL)
	{
	  if (extended_menuitems (mi->popup))
	    return 1;
	}
    }

  return 0;
}

/* Convert a string to a format type, or exit if it can't be done.  */

static enum res_format
format_from_name (name)
     const char *name;
{
  const struct format_map *m;

  for (m = format_names; m->name != NULL; m++)
    if (strcasecmp (m->name, name) == 0)
      break;

  if (m->name == NULL)
    {
      fprintf (stderr, "%s: unknown format type `%s'\n", program_name, name);
      fprintf (stderr, "%s: supported formats:", program_name);
      for (m = format_names; m->name != NULL; m++)
	fprintf (stderr, " %s", m->name);
      fprintf (stderr, "\n");
      xexit (1);
    }

  return m->format;
}

/* Work out a format type given a file name.  If INPUT is non-zero,
   it's OK to look at the file itself.  */

static enum res_format
format_from_filename (filename, input)
     const char *filename;
     int input;
{
  const char *ext;
  FILE *e;
  unsigned char b1, b2, b3, b4, b5;
  int magic;

  /* If we have an extension, see if we recognize it as implying a
     particular format.  */
  ext = strrchr (filename, '.');
  if (ext != NULL)
    {
      const struct format_map *m;

      ++ext;
      for (m = format_fileexts; m->name != NULL; m++)
	if (strcasecmp (m->name, ext) == 0)
	  return m->format;
    }

  /* If we don't recognize the name of an output file, assume it's a
     COFF file.  */

  if (! input)
    return RES_FORMAT_COFF;

  /* Read the first few bytes of the file to see if we can guess what
     it is.  */

  e = fopen (filename, FOPEN_RB);
  if (e == NULL)
    fatal ("%s: %s", filename, strerror (errno));

  b1 = getc (e);
  b2 = getc (e);
  b3 = getc (e);
  b4 = getc (e);
  b5 = getc (e);

  fclose (e);

  /* A PE executable starts with 0x4d 0x5a.  */
  if (b1 == 0x4d && b2 == 0x5a)
    return RES_FORMAT_COFF;

  /* A COFF .o file starts with a COFF magic number.  */
  magic = (b2 << 8) | b1;
  switch (magic)
    {
    case 0x14c: /* i386 */
    case 0x166: /* MIPS */
    case 0x184: /* Alpha */
    case 0x268: /* 68k */
    case 0x1f0: /* PowerPC */
    case 0x290: /* PA */
      return RES_FORMAT_COFF;
    }

  /* A RES file starts with 0x0 0x0 0x0 0x0 0x20 0x0 0x0 0x0.  */
  if (b1 == 0 && b2 == 0 && b3 == 0 && b4 == 0 && b5 == 0x20)
    return RES_FORMAT_RES;

  /* If every character is printable or space, assume it's an RC file.  */
  if ((isprint (b1) || isspace (b1))
      && (isprint (b2) || isspace (b2))
      && (isprint (b3) || isspace (b3))
      && (isprint (b4) || isspace (b4))
      && (isprint (b5) || isspace (b5)))
    return RES_FORMAT_RC;

  /* Otherwise, we give up.  */
  fatal ("can not determine type of file `%s'; use the -I option",
	 filename);

  /* Return something to silence the compiler warning.  */
  return RES_FORMAT_UNKNOWN;
}

/* Print a usage message and exit.  */

static void
usage (stream, status)
     FILE *stream;
     int status;
{
  fprintf (stream, "Usage: %s [options] [input-file] [output-file]\n",
	   program_name);
  fprintf (stream, "\
Options:\n\
  -i FILE, --input FILE       Name input file\n\
  -o FILE, --output FILE      Name output file\n\
  -I FORMAT, --input-format FORMAT\n\
                              Specify input format\n\
  -O FORMAT, --output-format FORMAT\n\
                              Specify output format\n\
  -F TARGET, --target TARGET  Specify COFF target\n\
  --preprocessor PROGRAM      Program to use to preprocess rc file\n\
  --include-dir DIR           Include directory when preprocessing rc file\n\
  --define SYM[=VAL]          Define SYM when preprocessing rc file\n\
  --language VAL              Set language when reading rc file\n");
#ifdef YYDEBUG
  fprintf (stream, "\
  --yydebug                   Turn on parser debugging\n");
#endif
  fprintf (stream, "\
  --help                      Print this help message\n\
  --version                   Print version information\n");
  fprintf (stream, "\
FORMAT is one of rc, res, or coff, and is deduced from the file name\n\
extension if not specified.  A single file name is an input file.\n\
No input-file is stdin, default rc.  No output-file is stdout, default rc.\n");
  list_supported_targets (program_name, stream);
  if (status == 0)
    fprintf (stream, "Report bugs to bug-gnu-utils@gnu.org\n");
  exit (status);
}

/* The main function.  */

int
main (argc, argv)
     int argc;
     char **argv;
{
  int c;
  char *input_filename;
  char *output_filename;
  enum res_format input_format;
  enum res_format output_format;
  char *target;
  char *preprocessor;
  char *preprocargs;
  int language;
  struct res_directory *resources;

  program_name = argv[0];
  xmalloc_set_program_name (program_name);

  bfd_init ();
  set_default_bfd_target ();

  res_init ();

  input_filename = NULL;
  output_filename = NULL;
  input_format = RES_FORMAT_UNKNOWN;
  output_format = RES_FORMAT_UNKNOWN;
  target = NULL;
  preprocessor = NULL;
  preprocargs = NULL;
  language = -1;

  while ((c = getopt_long (argc, argv, "i:o:I:O:F:", long_options,
			   (int *) 0)) != EOF)
    {
      switch (c)
	{
	case 'i':
	  input_filename = optarg;
	  break;

	case 'o':
	  output_filename = optarg;
	  break;

	case 'I':
	  input_format = format_from_name (optarg);
	  break;

	case 'O':
	  output_format = format_from_name (optarg);
	  break;

	case 'F':
	  target = optarg;
	  break;

	case OPTION_PREPROCESSOR:
	  preprocessor = optarg;
	  break;

	case OPTION_DEFINE:
	  if (preprocargs == NULL)
	    {
	      preprocargs = xmalloc (strlen (optarg) + 3);
	      sprintf (preprocargs, "-D%s", optarg);
	    }
	  else
	    {
	      char *n;

	      n = xmalloc (strlen (preprocargs) + strlen (optarg) + 4);
	      sprintf (n, "%s -D%s", preprocargs, optarg);
	      free (preprocargs);
	      preprocargs = n;
	    }
	  break;

	case OPTION_INCLUDE_DIR:
	  if (preprocargs == NULL)
	    {
	      preprocargs = xmalloc (strlen (optarg) + 3);
	      sprintf (preprocargs, "-I%s", optarg);
	    }
	  else
	    {
	      char *n;

	      n = xmalloc (strlen (preprocargs) + strlen (optarg) + 4);
	      sprintf (n, "%s -I%s", preprocargs, optarg);
	      free (preprocargs);
	      preprocargs = n;
	    }

	  {
	    struct include_dir *n, **pp;

	    n = (struct include_dir *) xmalloc (sizeof *n);
	    n->next = NULL;
	    n->dir = optarg;

	    for (pp = &include_dirs; *pp != NULL; pp = &(*pp)->next)
	      ;
	    *pp = n;
	  }

	  break;

	case OPTION_LANGUAGE:
	  language = strtol (optarg, (char **) NULL, 16);
	  break;

#ifdef YYDEBUG
	case OPTION_YYDEBUG:
	  yydebug = 1;
	  break;
#endif

	case OPTION_HELP:
	  usage (stdout, 0);
	  break;

	case OPTION_VERSION:
	  print_version ("windres");
	  break;

	default:
	  usage (stderr, 1);
	  break;
	}
    }

  if (input_filename == NULL && optind < argc)
    {
      input_filename = argv[optind];
      ++optind;
    }

  if (output_filename == NULL && optind < argc)
    {
      output_filename = argv[optind];
      ++optind;
    }

  if (argc != optind)
    usage (stderr, 1);

  if (input_format == RES_FORMAT_UNKNOWN)
    {
      if (input_filename == NULL)
	input_format = RES_FORMAT_RC;
      else
	input_format = format_from_filename (input_filename, 1);
    }

  if (output_format == RES_FORMAT_UNKNOWN)
    {
      if (output_filename == NULL)
	output_format = RES_FORMAT_RC;
      else
	output_format = format_from_filename (output_filename, 0);
    }

  /* Read the input file.  */

  switch (input_format)
    {
    default:
      abort ();
    case RES_FORMAT_RC:
      resources = read_rc_file (input_filename, preprocessor, preprocargs,
				language);
      break;
    case RES_FORMAT_RES:
      resources = read_res_file (input_filename);
      break;
    case RES_FORMAT_COFF:
      resources = read_coff_rsrc (input_filename, target);
      break;
    }

  if (resources == NULL)
    fatal ("no resources");

  /* Sort the resources.  This is required for COFF, convenient for
     rc, and unimportant for res.  */

  resources = sort_resources (resources);

  /* Write the output file.  */

  reswr_init ();

  switch (output_format)
    {
    default:
      abort ();
    case RES_FORMAT_RC:
      write_rc_file (output_filename, resources);
      break;
    case RES_FORMAT_RES:
      write_res_file (output_filename, resources);
      break;
    case RES_FORMAT_COFF:
      write_coff_file (output_filename, target, resources);
      break;
    }

  xexit (0);
  return 0;
}

struct res_directory *
read_res_file (filename)
     const char *filename;
{
  fatal ("read_res_file unimplemented");
  return NULL;
}

void
write_res_file (filename, resources)
     const char *filename;
     const struct res_directory *resources;
{
  fatal ("write_res_file unimplemented");
}
