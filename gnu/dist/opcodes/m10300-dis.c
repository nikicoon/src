/* Disassemble MN10300 instructions.
   Copyright (C) 1996, 1997, 1998 Free Software Foundation, Inc.

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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */


#include <stdio.h>

#include "ansidecl.h"
#include "opcode/mn10300.h" 
#include "dis-asm.h"

static void disassemble PARAMS ((bfd_vma, struct disassemble_info *,
				 unsigned long insn, unsigned int));

int 
print_insn_mn10300 (memaddr, info)
     bfd_vma memaddr;
     struct disassemble_info *info;
{
  int status;
  bfd_byte buffer[4];
  unsigned long insn;
  unsigned int consume;

  /* First figure out how big the opcode is.  */
  status = (*info->read_memory_func) (memaddr, buffer, 1, info);
  if (status != 0)
    {
      (*info->memory_error_func) (status, memaddr, info);
      return -1;
    }
  insn = *(unsigned char *) buffer;

  /* These are one byte insns.  */
  if ((insn & 0xf3) == 0x00
      || (insn & 0xf0) == 0x10
      || (insn & 0xfc) == 0x3c
      || (insn & 0xf3) == 0x41
      || (insn & 0xf3) == 0x40
      || (insn & 0xfc) == 0x50
      || (insn & 0xfc) == 0x54
      || (insn & 0xf0) == 0x60
      || (insn & 0xf0) == 0x70
      || ((insn & 0xf0) == 0x80
	  && (insn & 0x0c) >> 2 != (insn & 0x03))
      || ((insn & 0xf0) == 0x90
	  && (insn & 0x0c) >> 2 != (insn & 0x03))
      || ((insn & 0xf0) == 0xa0
	  && (insn & 0x0c) >> 2 != (insn & 0x03))
      || ((insn & 0xf0) == 0xb0
	  && (insn & 0x0c) >> 2 != (insn & 0x03))
      || (insn & 0xff) == 0xcb
      || (insn & 0xfc) == 0xd0
      || (insn & 0xfc) == 0xd4
      || (insn & 0xfc) == 0xd8
      || (insn & 0xf0) == 0xe0
      || (insn & 0xff) == 0xff)
    {
      consume = 1;
    }

  /* These are two byte insns.  */
  else if ((insn & 0xf0) == 0x80
	   || (insn & 0xf0) == 0x90
	   || (insn & 0xf0) == 0xa0
	   || (insn & 0xf0) == 0xb0
	   || (insn & 0xfc) == 0x20
	   || (insn & 0xfc) == 0x28
	   || (insn & 0xf3) == 0x43
	   || (insn & 0xf3) == 0x42
	   || (insn & 0xfc) == 0x58
	   || (insn & 0xfc) == 0x5c
	   || ((insn & 0xf0) == 0xc0
	       && (insn & 0xff) != 0xcb
	       && (insn & 0xff) != 0xcc
	       && (insn & 0xff) != 0xcd)
	   || (insn & 0xff) == 0xf0
	   || (insn & 0xff) == 0xf1
	   || (insn & 0xff) == 0xf2
	   || (insn & 0xff) == 0xf3
	   || (insn & 0xff) == 0xf4
	   || (insn & 0xff) == 0xf5
	   || (insn & 0xff) == 0xf6)
    {
      status = (*info->read_memory_func) (memaddr, buffer, 2, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	   return -1;
	}
      insn = bfd_getb16 (buffer);
      consume = 2;
    }

  /* These are three byte insns.  */
  else if ((insn & 0xff) == 0xf8
	   || (insn & 0xff) == 0xcc 
	   || (insn & 0xff) == 0xf9
	   || (insn & 0xf3) == 0x01
	   || (insn & 0xf3) == 0x02
	   || (insn & 0xf3) == 0x03
	   || (insn & 0xfc) == 0x24
	   || (insn & 0xfc) == 0x2c
	   || (insn & 0xfc) == 0x30
	   || (insn & 0xfc) == 0x34
	   || (insn & 0xfc) == 0x38
	   || (insn & 0xff) == 0xde
	   || (insn & 0xff) == 0xdf
	   || (insn & 0xff) == 0xcc)
    {
      status = (*info->read_memory_func) (memaddr, buffer, 2, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	  return -1;
	}
      insn = bfd_getb16 (buffer);
      insn <<= 8;
      status = (*info->read_memory_func) (memaddr + 2, buffer, 1, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	  return -1;
	}
      insn |= *(unsigned char *)buffer;
      consume = 3;
    }

  /* These are four byte insns.  */
  else if ((insn & 0xff) == 0xfa
	   || (insn & 0xff) == 0xfb)
    {
      status = (*info->read_memory_func) (memaddr, buffer, 4, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	  return -1;
	}
      insn = bfd_getb32 (buffer);
      consume = 4;
    }

  /* These are five byte insns.  */
  else if ((insn & 0xff) == 0xcd
	   || (insn & 0xff) == 0xdc)
    {
      status = (*info->read_memory_func) (memaddr, buffer, 4, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	  return -1;
	}
      insn = bfd_getb32 (buffer);
      consume = 5;
    }

  /* These are six byte insns.  */
  else if ((insn & 0xff) == 0xfd
	   || (insn & 0xff) == 0xfc)
    {
      status = (*info->read_memory_func) (memaddr, buffer, 4, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	  return -1;
	}

      insn = bfd_getb32 (buffer);
      consume = 6;
    }

  /* Else its a seven byte insns (in theory).  */
  else
    {
      status = (*info->read_memory_func) (memaddr, buffer, 4, info);
      if (status != 0)
	{
	  (*info->memory_error_func) (status, memaddr, info);
	  return -1;
	}

      insn = bfd_getb32 (buffer);
      consume = 7;
    }

  disassemble (memaddr, info, insn, consume);

  return consume;
}

static void
disassemble (memaddr, info, insn, size)
     bfd_vma memaddr;
     struct disassemble_info *info;
     unsigned long insn;
     unsigned int size;
{
  struct mn10300_opcode *op = (struct mn10300_opcode *)mn10300_opcodes;
  const struct mn10300_operand *operand;
  bfd_byte buffer[4];
  unsigned long extension = 0;
  int status, match = 0;

  /* Find the opcode.  */
  while (op->name)
    {
      int mysize, extra_shift;

      if (op->format == FMT_S0)
	mysize = 1;
      else if (op->format == FMT_S1
	       || op->format == FMT_D0)
	mysize = 2;
      else if (op->format == FMT_S2
	       || op->format == FMT_D1)
	mysize = 3;
      else if (op->format == FMT_S4)
	mysize = 5;
      else if (op->format == FMT_D2)
	mysize = 4;
      else if (op->format == FMT_D4)
	mysize = 6;
      else
	mysize = 7;
	
      if ((op->mask & insn) == op->opcode
	  && size == (unsigned int) mysize)
	{
	  const unsigned char *opindex_ptr;
	  unsigned int nocomma;
	  int paren = 0;
	  
	  if (op->format == FMT_D1 || op->format == FMT_S1)
	    extra_shift = 8;
	  else if (op->format == FMT_D2 || op->format == FMT_D4
		   || op->format == FMT_S2 || op->format == FMT_S4
		   || op->format == FMT_S6 || op->format == FMT_D5)
	    extra_shift = 16;
	  else
	    extra_shift = 0;

	  if (size == 1 || size == 2)
	    {
	      extension = 0;
	    }
	  else if (size == 3
		   && (op->format == FMT_D1
		       || op->opcode == 0xdf0000
		       || op->opcode == 0xde0000))
	    {
	      extension = 0;
	    }
	  else if (size == 3)
	    {
	      insn &= 0xff0000;
	      status = (*info->read_memory_func) (memaddr + 1, buffer, 2, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}

	      insn |= bfd_getl16 (buffer);
	      extension = 0;
	    }
	  else if (size == 4
		   && (op->opcode == 0xfaf80000
		       || op->opcode == 0xfaf00000
		       || op->opcode == 0xfaf40000))
	    {
	      extension = 0;
	    }
	  else if (size == 4)
	    {
	      insn &= 0xffff0000;
	      status = (*info->read_memory_func) (memaddr + 2, buffer, 2, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}

	      insn |= bfd_getl16 (buffer);
	      extension = 0;
	    }
	  else if (size == 5 && op->opcode == 0xdc000000)
	    {
	      unsigned long temp = 0;
	      status = (*info->read_memory_func) (memaddr + 1, buffer, 4, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      temp |= bfd_getl32 (buffer);

	      insn &= 0xff000000;
	      insn |= (temp & 0xffffff00) >> 8;
	      extension = temp & 0xff;
	    }
	  else if (size == 5)
	    {
	      unsigned long temp = 0;
	      status = (*info->read_memory_func) (memaddr + 1, buffer, 2, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      temp |= bfd_getl16 (buffer);

	      insn &= 0xff0000ff;
	      insn |= temp << 8;

	      status = (*info->read_memory_func) (memaddr + 4, buffer, 1, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      extension = *(unsigned char *)buffer;
	    }
	  else if (size == 6)
	    {
	      unsigned long temp = 0;
	      status = (*info->read_memory_func) (memaddr + 2, buffer, 4, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      temp |= bfd_getl32 (buffer);

	      insn &= 0xffff0000;
	      insn |= (temp >> 16) & 0xffff;
	      extension = temp & 0xffff;
	    }
	  else if (size == 7 && op->opcode == 0xdd000000)
	    {
	      unsigned long temp = 0;
	      status = (*info->read_memory_func) (memaddr + 1, buffer, 4, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      temp |= bfd_getl32 (buffer);

	      insn &= 0xff000000;
	      insn |= (temp >> 8) & 0xffffff;
	      extension = (temp & 0xff) << 16;
	      
	      status = (*info->read_memory_func) (memaddr + 5, buffer, 2, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      extension |= bfd_getb16 (buffer);
	    }
	  else if (size == 7)
	    {
	      unsigned long temp = 0;
	      status = (*info->read_memory_func) (memaddr + 2, buffer, 4, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      temp |= bfd_getl32 (buffer);

	      insn &= 0xffff0000;
	      insn |= (temp >> 16) & 0xffff;
	      extension = (temp & 0xffff) << 8;
	      
	      status = (*info->read_memory_func) (memaddr + 6, buffer, 1, info);
	      if (status != 0)
		{
		  (*info->memory_error_func) (status, memaddr, info);
		  return;
		}
	      extension |= *(unsigned char *)buffer;
	    }

	  match = 1;
	  (*info->fprintf_func) (info->stream, "%s\t", op->name);

	  /* Now print the operands.  */
	  for (opindex_ptr = op->operands, nocomma = 1;
	       *opindex_ptr != 0;
	       opindex_ptr++)
	    {
	      unsigned long value;

	      operand = &mn10300_operands[*opindex_ptr];

	      if ((operand->flags & MN10300_OPERAND_SPLIT) != 0)
		{
		  unsigned long temp;
		  value = insn & ((1 << operand->bits) - 1);
		  value <<= (32 - operand->bits);
		  temp = extension >> operand->shift;
		  temp &= ((1 << (32 - operand->bits)) - 1);
		  value |= temp;
		}
	      else if ((operand->flags & MN10300_OPERAND_EXTENDED) != 0)
		{
		  value = ((extension >> (operand->shift))
			   & ((1 << operand->bits) - 1));
		}
	      else
		{
		  value = ((insn >> (operand->shift))
			   & ((1 << operand->bits) - 1));
		}

	      if ((operand->flags & MN10300_OPERAND_SIGNED) != 0)
		value = ((long)(value << (32 - operand->bits))
			  >> (32 - operand->bits));

	      if (!nocomma
		  && (!paren
		      || ((operand->flags & MN10300_OPERAND_PAREN) == 0)))
		(*info->fprintf_func) (info->stream, ",");

	      nocomma = 0;
		
	      if ((operand->flags & MN10300_OPERAND_DREG) != 0)
		{
		  value = ((insn >> (operand->shift + extra_shift))
			   & ((1 << operand->bits) - 1));
		  (*info->fprintf_func) (info->stream, "d%d", value);
		}

	      else if ((operand->flags & MN10300_OPERAND_AREG) != 0)
		{
		  value = ((insn >> (operand->shift + extra_shift))
			   & ((1 << operand->bits) - 1));
		  (*info->fprintf_func) (info->stream, "a%d", value);
		}

	      else if ((operand->flags & MN10300_OPERAND_SP) != 0)
		(*info->fprintf_func) (info->stream, "sp");

	      else if ((operand->flags & MN10300_OPERAND_PSW) != 0)
		(*info->fprintf_func) (info->stream, "psw");

	      else if ((operand->flags & MN10300_OPERAND_MDR) != 0)
		(*info->fprintf_func) (info->stream, "mdr");

	      else if ((operand->flags & MN10300_OPERAND_PAREN) != 0)
		{
		  if (paren)
		    (*info->fprintf_func) (info->stream, ")");
		  else
		    {
		      (*info->fprintf_func) (info->stream, "(");
		      nocomma = 1;
		    }
		  paren = !paren;
		}

	      else if ((operand->flags & MN10300_OPERAND_PCREL) != 0)
		(*info->print_address_func) (value + memaddr, info);

	      else if ((operand->flags & MN10300_OPERAND_MEMADDR) != 0)
		(*info->print_address_func) (value, info);

	      else if ((operand->flags & MN10300_OPERAND_REG_LIST) != 0)
		{
		  int comma = 0;

		  (*info->fprintf_func) (info->stream, "[");
		  if (value & 0x80)
		    {
		      (*info->fprintf_func) (info->stream, "d2");
		      comma = 1;
		    }

		  if (value & 0x40)
		    {
		      if (comma)
			(*info->fprintf_func) (info->stream, ",");
		      (*info->fprintf_func) (info->stream, "d3");
		      comma = 1;
		    }

		  if (value & 0x20)
		    {
		      if (comma)
			(*info->fprintf_func) (info->stream, ",");
		      (*info->fprintf_func) (info->stream, "a2");
		      comma = 1;
		    }

		  if (value & 0x10)
		    {
		      if (comma)
			(*info->fprintf_func) (info->stream, ",");
		      (*info->fprintf_func) (info->stream, "a3");
		      comma = 1;
		    }

		  if (value & 0x08)
		    {
		      if (comma)
			(*info->fprintf_func) (info->stream, ",");
		      (*info->fprintf_func) (info->stream, "other");
		      comma = 1;
		    }
		  (*info->fprintf_func) (info->stream, "]");
		}

	      else 
		(*info->fprintf_func) (info->stream, "%d", value);
	    }
	  /* All done. */
	  break;
	}
      op++;
    }

  if (!match)
    {
	(*info->fprintf_func) (info->stream, "unknown\t0x%04x", insn);
    }
}
