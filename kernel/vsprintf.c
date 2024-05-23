/**
 * 
 * 	    ItIs - ITRON Implementation by Sakamura Lab
 * 
 * Copyright (C) 1989-1996 by Sakamura Lab, the University of Tokyo, JAPAN
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: vsprintf.c,v 1.5 1996/02/17 09:33:58 hiro Exp $
 */

/*
 *  ItIsカーネル用 簡易版 vsprintfライブラリ
 */

#include <stdarg.h>
#include "itis_kernel.h"

#ifdef USE_LONGLONG
typedef long long		LONG;
typedef unsigned long long	ULONG;
#else /* USE_LONGLONG */
typedef int		LONG;
typedef unsigned int	ULONG;
#endif /* USE_LONGLONG */

static void
convert(ULONG val, char **bufpt, int radix,
	const char *radchar, int width, int minus, int padzero)
{
	char	buf[24];
	int	i, j;

	i = 0;
	do {
		buf[i++] = radchar[val % radix];
		val /= radix;
	} while (val);
	if (minus) {
		buf[i++] = '-';
	}

	for (j = i; j < width; j++) {
		*(*bufpt)++ = padzero ? '0' : ' ';
	}
	while (i > 0) {
		*(*bufpt)++ = buf[--i];
	}
}

static char const raddec[] = "0123456789";
static char const radhex[] = "0123456789abcdef";
static char const radHEX[] = "0123456789ABCDEF";

#ifdef USE_LONGLONG
#define GET_ARG	((longlong) ? va_arg(ap, long long) : va_arg(ap, int))
#else /* USE_LONGLONG */
#define GET_ARG	(va_arg(ap, int))
#endif /* USE_LONGLONG */

int 
itis_vsprintf(char *buf, const char *format, va_list ap)
{
	int	c;
	char	*bufpt;
#ifdef USE_LONGLONG
	int	longlong;
#endif /* USE_LONGLONG */
	LONG	val;
	int	width;
	int	padzero;
	char	*str;

	bufpt = buf;
	while (c = *format++) {
		if (c != '%') {
			*bufpt++ = c;
			continue;
		}

		width = padzero = 0;
#ifdef USE_LONGLONG
		longlong = 0;
#endif /* USE_LONGLONG */
		if ((c = *format++) == '0') {
			padzero = 1;
			c = *format++;
		}
		while ('0' <= c && c <= '9') {
			width = width*10 + c - '0';
			c = *format++;
		}
#ifdef USE_LONGLONG
		if (c == 'L') {
			longlong++;
			c = *format++;
		}
#endif /* USE_LONGLONG */
		switch (c) {
		case 'd':
			val = GET_ARG;
			if (val >= 0) {
				convert(val, &bufpt, 10, raddec,
						 width, 0, padzero);
			}
			else {
				convert(-val, &bufpt, 10, raddec,
						width, 1, padzero);
			}
			break;
		case 'u':
			val = GET_ARG;
			convert(val, &bufpt, 10, raddec, width, 0, padzero);
			break;
		case 'x':
			val = GET_ARG;
			convert(val, &bufpt, 16, radhex, width, 0, padzero);
			break;
		case 'X':
			val = GET_ARG;
			convert(val, &bufpt, 16, radHEX, width, 0, padzero);
			break;
		case 'c':
			*bufpt++ = va_arg(ap, int);
			break;
		case 's':
			str = va_arg(ap, char *);
			while (c = *str++) {
				*bufpt++ = c;
			}
			break;
		case '%':
			*bufpt++ = '%';
			break;
		case 0:
			format--;
			break;
		default:
			break;
		}
	}
	*bufpt = 0;
	return(bufpt - buf);
}
