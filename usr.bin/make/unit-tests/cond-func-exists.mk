# $NetBSD: cond-func-exists.mk,v 1.3 2020/08/23 14:07:20 rillig Exp $
#
# Tests for the exists() function in .if conditions.

.if exists(.)
.else
.error
.endif

# The argument to the function must not be enclosed in quotes.
# Neither double quotes nor single quotes are allowed.
.if exists(".")
.error
.endif

.if exists('.')
.error
.endif

# The only way to escape characters that would otherwise influence the parser
# is to enclose them in a variable expression.  For function arguments,
# neither the backslash nor the dollar sign act as escape character.
.if exists(\.)
.error
.endif

.if exists(${:U.})
.else
.error
.endif

# The argument to the function can have several variable expressions.
# See cond-func.mk for the characters that cannot be used directly.
.if exists(${.PARSEDIR}/${.PARSEFILE})
.else
.error
.endif

all:
	@:;
