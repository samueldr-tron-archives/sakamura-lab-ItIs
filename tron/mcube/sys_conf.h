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
 *  @(#) $Id: sys_conf.h,v 1.5 1998/01/30 09:57:03 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  ItIs $BF0:n;~$N%a%b%j%^%C%W(B ($B%G%U%)%k%H;~(B)
 *
 *	80000000 - 8000ffff	ROM$B%b%K%?%o!<%/%(%j%"(B
 *	80020000 -		$B%+!<%M%k%3!<%INN0h(B
 *		 - 8007efff	$B%?%9%/FHN)ItMQ%9%?%C%/NN0h(B (ItIs $B$N(B SPI)
 *	8007f000 - 8007ffff	EIT$B%Y%/%?%F!<%V%k(B ($B<B:]$O$3$NH>J,$N$_;HMQ(B)
 *	80080000 - 800fffff	$B%+!<%M%k%G!<%?NN0h(B ($BBg$-$/$9$k$3$H$b2DG=(B)
 *	80100000 -		$BL$;HMQ(B ($B%f!<%6%W%m%0%i%`NN0h(B)
 *		 - 80ffffff	ROM$B%b%K%?$N=i4|(BSPI ($B?t%P%$%H2u$5$l$k(B)
 */

/*
 *  $B%9%?%C%/%(%j%"!$%R!<%W%(%j%"$NDj5A(B
 *
 *  _end $B$O!$%W%m%0%i%`$G;HMQ$9$k%G!<%?%(%j%"$N:G8e$N<!$NHVCO!%(B
 */
#define STACKTOP	0x8007f000	/* $B%?%9%/FHN)ItMQ%9%?%C%/$N=i4|CM(B */
#define HEAPTOP		_end		/* $B%R!<%W%(%j%"$N@hF,(B */
#define HEAPLIMIT	0x80100000	/* $B%R!<%W%(%j%"$N>e8B(B */

/*
 *  EIT$B%Y%/%?%F!<%V%k4XO"$NDj5A(B
 *
 *  MCUBE $B$N(B ROM$B%b%K%?(B (BMS) $B$G$O!$(BEIT$B%Y%/%?%F!<%V%k$r(B FFFFF000 $BHVCO$+(B
 *  $B$i(B ROM $B$KCV$$$F$$$k!%$=$3$G!$(BItIs $B5/F0;~$K!$(BEIT$B%Y%/%?%F!<%V%k$r(B RAM 
 *  $B>e$K:n$kI,MW$,$"$k!%(BEITVB_ITIS $BHVCO$K!$(BEIT$B%Y%/%?%F!<%V%k$N=i4|@_Dj(B
 *  $B$r(B ROM $B$+$i%3%T!<$7$F;H$&!%(B
 */
#define EITVB_ORIG	((EITVE *) 0xfffff000)
#define EITVB_ITIS	((EITVE *) 0x8007f000)
#define EITVT_LEN	0x800

/*
 *  $B%7%9%F%`%?%9%/$K4X$9$kDj5A(B
 */
#define	CONSOLE_PORT	1	/* $B%3%s%=!<%kMQ$KMQ$$$k%7%j%"%k%]!<%HHV9f(B */
#define	LOGTASK_PORT	1	/* $B%7%9%F%`%m%0$r=PNO$9$k%7%j%"%k%]!<%HHV9f(B */

#endif /* _SYS_CONF_ */
