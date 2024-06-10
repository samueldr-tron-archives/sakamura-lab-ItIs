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
 *  @(#) $Id: sys_timer.h,v 1.5 1998/06/02 03:07:08 hiro Exp $
 */

#ifndef _SYS_TIMER_
#define _SYS_TIMER_

#include <sys/time.h>

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
 *  void start_hw_timer(FP handler) : $B%?%$%^$r=i4|2=$7!$<~4|E*$J%?%$%^(B
 *  $B3d9~$_$r3+;O$5$;$k!%(Bhandler $B$O!$<~4|E*$K5/F0$9$Y$-%O%s%I%i$N3+;O%"(B
 *  $B%I%l%9!%(B
 *
 *  void clear_hw_timer_interupt(void) : $B%?%$%^3d9~$_MW5a$r%/%j%"$9$k!%(B
 *  $B%?%$%^3d9~$_%O%s%I%i$N:G=i$G8F$P$l$k!%(B
 *
 *  void terminate_hw_timer(void) : $B%?%$%^$NF0:n$rDd;_$5$;$k!%%7%9%F%`(B
 *  $BDd;_;~$K8F$P$l$k!%(B
 */ 

/*
 *  $B%?%$%^3d9~$_$N<~4|(B ($BC10L$O(B msec)
 */
#define TIMER_PERIOD	10

/*
 *  $B%?%$%^%O%s%I%i(B
 */
extern void	timer_handler();

/*
 *  $B%?%$%^$N%9%?!<%H=hM}(B
 */
Inline void
start_hw_timer(void)
{
	struct sigvec	vec;
	struct itimerval val;

	vec.sv_handler = timer_handler;
	vec.sv_mask = SIGMASK_INDP;
	vec.sv_flags = SV_ONSTACK|SV_INTERRUPT;
	sigvec(SIGALRM, &vec, 0);

	val.it_interval.tv_sec = TIMER_PERIOD / 1000;
	val.it_interval.tv_usec = (TIMER_PERIOD % 1000) * 1000;
	val.it_value = val.it_interval;
	setitimer(ITIMER_REAL, &val, 0);
}

/*
 *  $B%?%$%^3d9~$_$N%/%j%"(B
 */
Inline void
clear_hw_timer_interrupt(void)
{
}

/*
 *  $B%?%$%^$NDd;_=hM}(B
 */
Inline void
terminate_hw_timer(void)
{
	struct itimerval val;

	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = 0;
	val.it_value = val.it_interval;
	setitimer(ITIMER_REAL, &val, 0);
}

#endif /* _SYS_TIMER_ */
