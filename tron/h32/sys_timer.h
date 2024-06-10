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
 *  @(#) $Id: sys_timer.h,v 1.5 1998/01/30 09:56:45 hiro Exp $
 */

#ifndef _SYS_TIMER_
#define _SYS_TIMER_

/*
 *  $B%O!<%I%&%'%"0MB8%?%$%^%b%8%e!<%k(B
 *
 *  $B$3$N%b%8%e!<%kFb$G!$0J2<$NDj?t$*$h$S4X?t$rDj5A$7$J$1$l$P$J$i$J$$!%(B
 * 
 *  TIMER_PERIOD : $B%?%$%^3d9~$_$N<~4|(B ($BC10L$O(B msec)$B!%(BItIs $B$G$NI8=`CM$O(B 
 *  1msec $B$G$"$k$,!$%?%$%^3d9~$_$K$h$k%*!<%P%X%C%I$r>.$5$/$7$?$$>l9g$K(B
 *  $B$O!$$b$C$HD9$$<~4|$K@_Dj$7$F$b$h$$!%$b$A$m$s!$%?%$%`%"%&%H;~4V$J$I(B
 *  $B$NJ,2rEY$OAF$/$J$k(B ($BC10L$OJQ$o$i$J$$(B)$B!%(B
 *
 *  void start_hw_timer() : $B%?%$%^$r=i4|2=$7!$<~4|E*$J%?%$%^3d9~$_$r3+(B
 *  $B;O$5$;$k!%(B
 *
 *  void clear_hw_timer_interupt(void) : $B%?%$%^3d9~$_MW5a$r%/%j%"$9$k!%(B
 *  $B%?%$%^3d9~$_%O%s%I%i$N:G=i$G8F$P$l$k!%(B
 *
 *  void terminate_hw_timer(void) : $B%?%$%^$NF0:n$rDd;_$5$;$k!%%7%9%F%`(B
 *  $BDd;_;~$K8F$P$l$k!%(B
 */

#include "h32sbc.h"

/*
 *  $B%?%$%^3d9~$_$N<~4|(B ($BC10L$O(B msec)
 */
#define TIMER_PERIOD	1

/*
 *  $B%?%$%^CM$NFbItI=8=$N7?(B
 */
typedef unsigned int	TICK;

/*
 *  $B%?%$%^CM$NFbItI=8=$H(B msec $BC10L$H$NJQ49(B
 *
 *  H32SBC$B%7%9%F%`(B CPU$B%\!<%I$G$O!$%?%$%^$O(B 4$B&L(Bsec $BKh$K%+%&%s%H%"%C%W$9(B
 *  $B$k!%(B
 */
#define TIMER_TICK	250
#define TO_TICK(msec)	((msec) * TIMER_TICK)

/*
 *  $B%?%$%^CM$NFbItI=8=$H&L(Bsec $BC10L$H$NJQ49(B
 */
#if TIMER_TICK <= 1000
#define TO_USEC(tick)	((tick) * (1000 / TIMER_TICK))
#else
#define TO_USEC(tick)	((tick) / (TIMER_TICK / 1000))
#endif

/*
 *  $B@-G=I>2AMQ%7%9%F%`;~9o$r<h$j=P$9:]$N=hM};~4V$N8+@Q$jCM(B ($BC10L$OFbItI=8=(B)
 */
#define GET_TOLERANCE	(TIMER_TICK / 5)

/*
 *  $B@_Dj$G$-$k:GBg$N%?%$%^<~4|(B ($BC10L$OFbItI=8=(B)
 *
 *  $BFbItI=8=$G(B 0xffffff $B$OLs(B 67$BIC!%(B
 */
#define MAX_TICK	((TICK) 0xffffff)

/*
 *  PIT $B%?%$%^%+%&%s%?%l%8%9%?$N@_DjCM(B
 */
#define PIT_CYCLIC	0xe0

/*
 *  $B%"%;%s%V%i%l%Y%k$N%?%$%^%O%s%I%i(B
 */
extern void	timer_handler_startup(void);

/*
 *  $B%?%$%^$N%9%?!<%H=hM}(B
 */
Inline void
start_hw_timer()
{
	TICK	t = TO_TICK(TIMER_PERIOD) - 1;

	/*
	 *  $B3d9~$_%O%s%I%i$NDj5A(B
	 */
	define_eit(INT_VECTOR(2), EITATR(0, 15), timer_handler_startup);

	/*
	 *  IRC $B$N@_Dj(B
	 */
	irc_assign(IRC_LMR(2), LIR6_BIT);
	irc_or_assign(IRC_VMR, LIR6_BIT);
	irc_or_assign(IRC_TMR, LIR6_BIT);	/* $B%(%C%8%H%j%,%b!<%I(B */
	irc_and_assign(IRC_IMR, ~LIR6_BIT);	/* $B3d9~$_%^%9%/2r=|(B */

	/*
	 *  PIT $B$N@_Dj(B
	 */
	io_assign(PIT_TCR, PIT_CYCLIC);
	assert(t <= MAX_TICK);			/* $B%?%$%^>e8BCM$N%A%'%C%/(B */
	io_assign(PIT_CPRH, (t >> 16) & 0xff);
	io_assign(PIT_CPRM, (t >> 8) & 0xff);
	io_assign(PIT_CPRL, t & 0xff);
	io_assign(PIT_TSR, 0x01);		/* $B%<%m8!=P$r%/%j%"(B */
	io_or_assign(PIT_TCR, 0x01);		/* $B%?%$%^!<$r%9%?!<%H$5$;$k(B */

	irc_assign(IRC_IRR, LIR6_BIT);		/* $B3d9~$_MW5a$r%/%j%"(B */
}

/*
 *  $B%?%$%^3d9~$_$N%/%j%"(B
 */
Inline void
clear_hw_timer_interrupt(void)
{
	irc_assign(IRC_IRR, LIR6_BIT);		/* $B3d9~$_MW5a$r%/%j%"(B */
	io_assign(PIT_TSR, 0x01);		/* $B%<%m8!=P$r%/%j%"(B */
}

/*
 *  $B%?%$%^$NDd;_=hM}(B
 */
Inline void
terminate_hw_timer(void)
{
	io_and_assign(PIT_TCR, ~0x01);		/* $B%?%$%^!<$r;_$a$k(B */
	io_assign(PIT_TSR, 0x01);		/* $B%<%m8!=P$r%/%j%"(B */
	irc_or_assign(IRC_IMR, LIR6_BIT);	/* $B3d9~$_$r%^%9%/(B */
}

/*
 *  $B%?%$%^$N8=:_CM$NFI$_$@$7(B
 *
 *  $B3d9~$_6X;_6h4VCf$G8F$S=P$9$3$H!%(B
 */
Inline TICK
get_current_hw_time(void)
{
#if TO_TICK(TIMER_PERIOD) < 256
	return(TO_TICK(TIMER_PERIOD) - 1 - *PIT_CNTRL);
#else
#error \
  get_current_hw_time is not implemented when TO_TICK(TIMER_PERIOD) >= 256.
#endif
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(irc_read(IRC_IRR) & LIR6_BIT);
}

#endif /* _SYS_TIMER_ */
