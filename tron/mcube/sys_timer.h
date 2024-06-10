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
 *  @(#) $Id: sys_timer.h,v 1.5 1998/01/30 09:57:05 hiro Exp $
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

#include "mcube.h"

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
 *  MCUBE $B4pK\%\!<%I$G$O!$%?%$%^(B ($B%+%&%s%?(B2 $B$rMQ$$$k(B) $B$O(B 1$B&L(Bsec $BKh$K%+(B
 *  $B%&%s%H%"%C%W$9$k!%(B
 */
#define TIMER_TICK	1000
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
#define GET_TOLERANCE	(TIMER_TICK / 10)

/*
 *  $B@_Dj$G$-$k:GBg$N%?%$%^<~4|(B ($BC10L$OFbItI=8=(B)
 *
 *  $BFbItI=8=$G(B 0xffff $B$OLs(B 65msec$B!%(B
 */
#define MAX_TICK	((TICK) 0xffff)

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
	TICK	t = TO_TICK(TIMER_PERIOD);

	/*
	 *  $B3d9~$_%O%s%I%i$NDj5A(B
	 */
	define_eit(IRC1_VECTOR(1), EITATR(0, 15), timer_handler_startup);

	/*
	 *  IRC $B$N@_Dj(B
	 */
	irc_assign(IRC1_LMR(1), LIR1_BIT);
	irc_or_assign(IRC1_TMR, LIR1_BIT);	/* $B%(%C%8%H%j%,%b!<%I(B */
	irc_and_assign(IRC1_IMR, ~LIR1_BIT);	/* $B3d9~$_%^%9%/2r=|(B */

	/*
	 *  $B%?%$%^$N@_Dj(B
	 */
	tmr_write(TMR_CNTL, 0xb4);		/* $B%?%$%^%b!<%I(B2 */
	assert(t <= MAX_TICK);			/* $B%?%$%^>e8BCM$N%A%'%C%/(B */
	tmr_write(TMR_CNT2, t & 0xff);
	tmr_write(TMR_CNT2, (t >> 8) & 0xff);

	irc_assign(IRC1_IRR, LIR1_BIT);		/* $B3d9~$_MW5a$r%/%j%"(B */
}

/*
 *  $B%?%$%^3d9~$_$N%/%j%"(B
 */
Inline void
clear_hw_timer_interrupt(void)
{
	irc_assign(IRC1_IRR, LIR1_BIT);		/* $B3d9~$_MW5a$r%/%j%"(B */
}

/*
 *  $B%?%$%^$NDd;_=hM}(B
 */
Inline void
terminate_hw_timer(void)
{
	irc_or_assign(IRC1_IMR, LIR1_BIT);	/* $B3d9~$_%^%9%/@_Dj(B */
}

/*
 *  $B%?%$%^$N8=:_CM$NFI$_=P$7(B
 *
 *  $B3d9~$_6X;_6h4VCf$G8F$S=P$9$3$H!%(B
 *
 *  $BK\Mh!$(B0 $B!A(B TO_TICK(TIMER_PERIOD)-1 $B$NCM$,JV$k$O$:$@$,!$<B:]$K$OIQEY(B
 *  $B$ODc$$$,(B TO_TICK(TIMER_PERIOD) $B$,JV$k$3$H$,$"$k!%(B
 */
Inline TICK
get_current_hw_time(void)
{
	TICK	t;

	tmr_write(TMR_CNTL, 0x80);		/* $B%?%$%^CMFI$_=P$73+;O(B */
	t = tmr_read(TMR_CNT2);
	t += tmr_read(TMR_CNT2) << 8;
	return(TO_TICK(TIMER_PERIOD) - t);
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(irc_read(IRC1_IRR) & LIR1_BIT);
}

#endif /* _SYS_TIMER_ */
