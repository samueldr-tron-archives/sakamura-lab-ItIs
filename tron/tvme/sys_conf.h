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
 *  @(#) $Id: sys_conf.h,v 1.6 1998/01/30 09:57:23 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  ItIs $BF0:n;~$N%a%b%j%^%C%W(B
 *
 *	00000000 -		$B%+!<%M%k%3!<%INN0h(B
 *		 - 00001000	ROM$B%b%K%?$N=i4|(BSPI ($B?t%P%$%H2u$5$l$k(B)
 *	00040000 - 0007ffff	$B%f!<%6%W%m%0%i%`NN0h(B (256KB)
 *	00080000 - 000fffff	$B%+!<%M%k%G!<%?NN0h(B (512KB)
 *	fff20000 - fff27fff	$B%b%K%?%o!<%/NN0h(B (32KB)
 *	fff28000 - fff37fff	$BL$;HMQ(B (32KB, $B0lIt%@%$%l%/%H5/F0MQ$N%9%?%C%/(B)
 *		 - fff3ffff	$B%?%9%/FHN)ItMQ%9%?%C%/NN0h(B
 *	fffb0000 - fffbffff	$B%m!<%+%k(BI/O$BNN0h(B
 *	fffc0000 - fffc007f	$B%a%$%k%\%C%/%9NN0h(B
 */

/*
 *  $B%9%?%C%/%(%j%"!$%R!<%W%(%j%"$NDj5A(B
 *
 *  _end $B$O!$%W%m%0%i%`$G;HMQ$9$k%G!<%?%(%j%"$N:G8e$N<!$NHVCO!%(B
 */
#define STACKTOP	0xfff40000	/* $B%?%9%/FHN)ItMQ%9%?%C%/$N=i4|CM(B */
#define HEAPTOP		_end		/* $B%R!<%W%(%j%"$N@hF,(B */
#define HEAPLIMIT	0x00100000	/* $B%R!<%W%(%j%"$N>e8B(B */

/*
 *  $B%7%9%F%`%?%9%/$K4X$9$kDj5A(B
 */
#define	CONSOLE_PORT	1	/* $B%3%s%=!<%kMQ$KMQ$$$k%7%j%"%k%]!<%HHV9f(B */
#define	LOGTASK_PORT	1	/* $B%7%9%F%`%m%0$r=PNO$9$k%7%j%"%k%]!<%HHV9f(B */

#endif /* _SYS_CONF_ */
