/**
 * 
 * 	ItIs - An ITRON Implementation for Research and Education
 * 
 * Copyright (C) 1989-1997 by Sakamura Laboratory, Univ. of Tokyo, JAPAN
 * Copyright (C) 1997-1998 by Embedded and Real-Time Systems Laboratory,
 * 				Toyohashi Univ. of Technology, JAPAN
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the universities nor the names of the laboratories
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITIES OR THE LABORATORIES BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: itis_services.h,v 1.9 1998/01/30 09:53:59 hiro Exp $
 */

#ifndef _ITIS_SERVICES_
#define _ITIS_SERVICES_

/* 
 *  ItIs$B%"%W%j%1!<%7%g%s(B $BI8=`%$%s%/%k!<%I%U%!%$%k(B
 *
 *  ItIs $B$N%7%9%F%`%3!<%k$r8F$S=P$9%"%W%j%1!<%7%g%s$O!$$9$Y$F$N$3$N%U%!(B
 *  $B%$%k$r%$%s%/%k!<%I$9$Y$-$G$"$k!%(B
 *  $B%7%9%F%`%3!<%k$r%5%V%k!<%A%s%3!<%k$G8F$S=P$9>l9g$K$O!$$3$N%U%!%$%k(B
 *  $B$r%$%s%/%k!<%I$9$kA0$K!$(BLINK_KERNEL $B$rDj5A(B (#define) $B$9$k$3$H!%(B
 */

#include <itron.h>
#include <itron_errno.h>
#include <itron_fncode.h>
#include <itis_debug.h>
#include <itis_stddefs.h>
#include <itis_isyscall.h>

/*
 *  $B%7%9%F%`%3!<%k8F=P%^%/%m(B
 */
extern void	itis_perror(const char *file, int line,
				const char *expr, ER ercd);
extern void	itis_panic(const char *file, int line,
				const char *expr, ER ercd);

Inline ER
_itis_perror(const char *file, int line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		itis_perror(file, line, expr, ercd);
	}
	return(ercd);
}

Inline ER
_itis_panic(const char *file, int line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		itis_panic(file, line, expr, ercd);
	}
	return(ercd);
}

#define	syscall(s)	_itis_perror(__FILE__, __LINE__, #s, (s));
#define	_syscall(s)	_itis_panic(__FILE__, __LINE__, #s, (s));

#endif /* _ITIS_SERVICES_ */
