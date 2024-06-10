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
 *  @(#) $Id: cpu_conf.h,v 1.7 1998/06/02 10:21:52 hiro Exp $
 */

#ifndef _CPU_CONF_
#define _CPU_CONF_

/*
 *  get_ver $B$G;2>H$5$l$k(B CPU$B%3!<%I$NDj5A(B
 */
#define	CPU_CODE	0x0001		/* TRON$B;EMM%A%C%W0lHL(B */

/*
 *  CPU$B0MB8$N%7%9%F%`%3!<%kL>(B/$B%Q%i%a!<%?L>$N@_Dj(B
 */
#define chg_iXX	chg_ims
#define ref_iXX	ref_ims
#define iXXXX	imask
#define p_iXXXX	p_imask

/*
 *  TRON$B;EMM%A%C%W$G$O%5%]!<%H$5$l$J$$5!G=$NDj5A(B
 */
#define _i_dis_int	_no_support	/* dis_int$B%7%9%F%`%3!<%k(B */
#define _i_ena_int	_no_support	/* ena_int$B%7%9%F%`%3!<%k(B */

/*
 *  $B%?%9%/B0@-Cf$N<B9T%b!<%I$K4X$9$kDj5A(B
 */
#define TA_MODEMASK	(TA_RNG3)
#define SYSMODE(atr)	(((atr) & TA_MODEMASK) == TA_RNG0)

/*
 *  $B%7%9%F%`%9%?%C%/$H%f!<%6%9%?%C%/$rJL!9$K;}$D(B
 */
#define	USE_SEPARATE_SSTACK

/*
 *  dispatch_disabled $B$O!$CY1d3d9~$_$r;H$o$J$$>l9g$K$N$_I,MW(B
 */
#ifdef TRON_NO_DI
#define USE_DISPATCH_DISABLED
#endif /* TRON_NO_DI */

/*
 *  $B%7%9%F%`%9%?%C%/$N:G>.%5%$%:$NDj5A(B
 *
 *  cre_tsk $B$G%?%9%/Kh$K%7%9%F%`%9%?%C%/%5%$%:$r@_Dj$9$k>l9g$N:G>.$N%7(B
 *  $B%9%F%`%9%?%C%/%5%$%:!%(B
 */
#ifndef MIN_SYS_STACK_SIZE
#define	MIN_SYS_STACK_SIZE	256
#endif /* MIN_SYS_STACK_SIZE */

/*
 *  $B%G%U%)%k%H%7%9%F%`%9%?%C%/%5%$%:$NDj5A(B
 *
 *  cre_tsk $B$G%?%9%/Kh$K%7%9%F%`%9%?%C%/%5%$%:$r@_Dj$7$J$$>l9g$N%G%U%)(B
 *  $B%k%H$N%7%9%F%`%9%?%C%/%5%$%:!%(B
 */
#ifndef DEF_SYS_STACK_SIZE
#define DEF_SYS_STACK_SIZE	1024
#endif /* DEF_SYS_STACK_SIZE */

#endif /* _CPU_CONF_ */
