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
 *  @(#) $Id: sys_conf.h,v 1.8 1998/06/08 10:52:27 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  OS $B$N<oN`$NDj5A(B
 */
#define	SUNOS4

/*
 *  CPU $B$H(B _setjmp/_longjmp $B%i%$%V%i%j$N<BAu$K0MB8$9$kDj?t$N@_Dj(B
 *
 *  JMPBUF_PC: jmp_buf $B9=B$BNCf$G!$(BPC $B$NCM$rF~$l$k>l=j$N%*%U%;%C%H(B
 *  JMPBUF_SP: jmp_buf $B9=B$BNCf$G!$(BSP $B$NCM$rF~$l$k>l=j$N%*%U%;%C%H(B
 *
 *  STACK_MERGIN: $B%9%?%C%/%]%$%s%?$N=i4|CM$r!$%9%?%C%/NN0h$N>e8B(B ($B3NJ](B
 *  $B$7$?NN0h$N<!$NHVCO(B) $B$+$i$I(B $B$l$@$1N%$9$+!%%9%?%C%/%]%$%s%?$,;X$9HV(B
 *  $BCO$+$i;H$$;O$a$k%W%m%;%C%5$N(B $B$N>l9g$K$O!$$3$l$r;H$C$F%9%?%C%/%]%$(B
 *  $B%s%?$N=i4|CM$r>e8B$+$i2<$2$k!%$^$?!$(Blongjmp $B$7$?8e$K!$%9%?%C%/%]%$(B
 *  $B%s%?$h$j>e$NHVCO$r(B $B%"%/%;%9$9$k2DG=@-$,$"$k>l9g$K$O!$$3$l$r;H$C$F(B
 *  $B%^!<%8%sNN0h$r3NJ]$9$k!%(B
 *
 *  SIGSTACK_MERGIN: $B%7%0%J%k%9%?%C%/$r%W%m%;%9%9%?%C%/>e$K<h$k;~$N%^!<(B
 *  $B%8%s!%%9%?!<%H%"%C%W%k!<%A%s$,;H$&$?$a$N%9%?%C%/NN0h!%(B
 */

#ifdef sparc

#define JMPBUF_PC		3
#define JMPBUF_SP		2
#define STACK_MERGIN		256
#define SIGSTACK_MERGIN		8192

#else /* sparc */

#error not supported.

#endif /* sparc */

/*
 *  $B%7%0%J%k%9%?%C%/$NJQ99$K(B sigstack $B$r;H$&(B
 */
#define USE_SIGSTACK

/*
 *  $B%9%?%C%/%(%j%"$N<hF@(B/$BJV5Q4X?t$r(B mprotect $B$r;H$&$b$N$K:9$7$+$($k(B
 */
#define	USE_MPROTECT_STACK

/*
 *  $B5?;w%7%j%"%k%I%i%$%P$N$?$a$NDj5A(B
 */

#ifdef _BSD_SERIAL_

#include <sys/ioctl.h>

Inline void
set_rawmode(struct sgttyb *ttyb)
{
	ttyb->sg_flags |= (RAW | CBREAK);
	ttyb->sg_flags &= ~(ECHO);
}

#undef RAW			/* $B>WFM$9$k%7%s%\%k$NDj5A$rKu>C(B */
#undef ECHO

#endif /* _BSD_SERIAL_ */
#endif /* _SYS_CONF_ */
