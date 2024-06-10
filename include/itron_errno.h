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
 *  @(#) $Id: itron_errno.h,v 1.6 1998/01/30 09:54:02 hiro Exp $
 */

#ifndef _ITRON_ERRNO_
#define _ITRON_ERRNO_

/* 
 *  ITRON $B$N%(%i!<%3!<%I$NDj5A(B
 */

#define	E_OK		0		/* $B@5>o=*N;(B */

#define E_SYS		(-5)		/* $B%7%9%F%`%(%i!<(B */
#define	E_NOMEM		(-10)		/* $B%a%b%jITB-(B */
#define	E_NOSPT		(-17)		/* $BL$%5%]!<%H5!G=(B */
#define	E_INOSPT	(-18)		/* ITRON/FILE $B$G$NL$%5%]!<%H5!G=(B */
#define	E_RSFN		(-20)		/* $BM=Ls5!G=HV9f(B */
#define	E_RSATR		(-24)		/* $BM=LsB0@-(B */
#define	E_PAR		(-33)		/* $B%Q%i%a%?!<%(%i!<(B */
#define	E_ID		(-35)		/* $BIT@5(BID$BHV9f(B */
#define	E_NOEXS		(-52)		/* $B%*%V%8%'%/%H$,B8:_$7$F$$$J$$(B */
#define	E_OBJ		(-63)		/* $B%*%V%8%'%/%H$N>uBV$,IT@5(B */
#define	E_MACV		(-65)		/* $B%a%b%j%"%/%;%9ITG=(B/$B%"%/%;%98"0cH?(B */
#define	E_OACV		(-66)		/* $B%*%V%8%'%/%H%"%/%;%98"0cH?(B */
#define	E_CTX		(-69)		/* $B%3%s%F%-%9%H%(%i!<(B */
#define	E_QOVR		(-73)		/* $B%-%e!<%$%s%0$N%*!<%P!<%U%m!<(B */
#define	E_DLT		(-81)		/* $BBT$A%*%V%8%'%/%H$,:o=|$5$l$?(B */
#define	E_TMOUT		(-85)		/* $B%]!<%j%s%0<:GT(B/$B%?%$%`%"%&%H(B */
#define	E_RLWAI		(-86)		/* $BBT$A>uBV6/@)2r=|(B */

#define	EN_NOND		(-113)		/* $BBP>]%N!<%I$,B8:_$7$J$$(B */
#define	EN_OBJNO	(-114)		/* $B%"%/%;%9$G$-$J$$%*%V%8%'%/%HHV9f(B */
#define	EN_PROTO	(-115)		/* $BL$%5%]!<%H%W%m%H%3%k(B */
#define	EN_RSFN		(-116)		/* $BL$%5%]!<%H%7%9%F%`%3!<%k(B */
#define	EN_COMM		(-117)		/* $BBP>]%N!<%I$+$i$N1~Ez$,$J$$(B */
#define	EN_RLWAI	(-118)		/* $B@\B35!G=$K4X$o$kBT$A>uBV6/@)2r=|(B */
#define	EN_PAR		(-119)		/* $BMW5a;~$N%Q%i%a!<%?%(%i!<(B */
#define	EN_RPAR		(-120)		/* $B%j%?!<%s;~$N%Q%i%a!<%?%(%i!<(B */
#define	EN_CTXID	(-121)		/* $B@\B35!G=$r;H$($J$$%3%s%F%-%9%H(B */
#define	EN_EXEC		(-122)		/* $BBP>]%N!<%I$K$*$1$k;q8;ITB-(B */
#define	EN_NOSPT	(-123)		/* $BL$%5%]!<%H@\B35!G=(B */

#define EV_FULL		(-225)		/* $B%*%V%8%'%/%H%F!<%V%k%U%k(B */

#endif /* _ITRON_ERRNO_ */
