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
 *  @(#) $Id: timer.c,v 1.8 1998/01/30 09:52:51 hiro Exp $
 */

#include "itis_kernel.h"
#include "timer.h"
#include "task.h"
#include "sys_timer.h"

/*
 *  $B8=:_;~9o(B ($B%=%U%H%&%'%"%/%m%C%/(B)
 */
SYSTIME	current_time;

/* 
 *  $B%?%$%^%-%e!<(B
 */
static QUEUE	timer_queue;

/*
 *  $B%?%$%^%b%8%e!<%k$N=i4|2=(B
 */
void
timer_initialize(void)
{
	current_time = 0;
	queue_initialize(&timer_queue);
	start_hw_timer();
}

/*
 *  $B%?%$%^$NDd;_(B
 */
void
timer_shutdown(void)
{
	terminate_hw_timer();
}

/*
 *  $B%?%$%^%$%Y%s%H$r%?%$%^%-%e!<$XA^F~(B
 */
static void
enqueue_tmeb(TMEB *event)
{
	QUEUE	*q;

	for (q = timer_queue.next; q != &timer_queue; q = q->next) {
		if (event->time <= ((TMEB *) q)->time) {
			break;
		}
	}
	queue_insert(&(event->queue), q);
}

/*
 *  $B%?%$%^%$%Y%s%H(B event $B$r!$%?%$%`%"%&%H;~4V(B tmout $B8e$K5/F0$5$l$k$h$&(B
 *  $B$K%?%$%^%-%e!<$XEPO?$9$k!%%?%$%`%"%&%H;~4V$,Mh$k$H!$%3!<%k%P%C%/4X(B 
 *  $B?t(B callback $B$K0z?t(B arg $B$rM?$($F5/F0$9$k!%(Btmout $B$,(B TMO_FEVR $B$N;~$O!$(B
 *  $B%?%$%^%-%e!<$K$OEPO?$7$J$$$,!$8e$G(B timer_delete $B$,8F$P$l$F$b$h$$$h(B
 *  $B$&$K!$%-%e!<$N%(%j%"$r=i4|2=$9$k!%(B
 */
void
timer_insert(TMEB *event, TMO tmout, CBACK callback, VP arg)
{
	event->callback = callback;
	event->arg = arg;

	if (tmout == TMO_FEVR) {
		queue_initialize(&(event->queue));
	}
	else {
		assert(tmout > 0);
		event->time = current_time + tmout;
		enqueue_tmeb(event);
	}
}

/*
 *  $B%?%$%^%$%Y%s%H(B event $B$r!$(B($B@dBP(B) $B;~9o(B time $B$K5/F0$5$l$k$h$&$K%?%$%^(B
 *  $B%-%e!<$XEPO?$9$k!%(B
 */
void
timer_insert_abs(TMEB *event, SYSTIME *time, CBACK callback, VP arg)
{
	event->callback = callback;
	event->arg = arg;

	event->time = *time;
	enqueue_tmeb(event);
}

/*
 *  $B%?%$%^%$%Y%s%H(B event $B$r!$(Btime $B;~4V8e$K5/F0$5$l$k$h$&$K$K%?%$%^%-%e!<(B
 *  $B$XEPO?$9$k!%(B
 */
void
timer_insert_rel(TMEB *event, SYSTIME *time, CBACK callback, VP arg)
{
	event->callback = callback;
	event->arg = arg;

	event->time = current_time + *time;
	enqueue_tmeb(event);
}

/*
 *  $B%?%$%^3d9~$_%O%s%I%i(B
 *
 *  $B%?%$%^3d9~$_%O%s%I%i$O!$%O!<%I%&%'%"%?%$%^$K$h$j(B TIMER_PERIOD$B%_%jIC(B
 *  $B$N<~4|$G5/F0$5$l$k!%%=%U%H%&%'%"%/%m%C%/$r99?7$7!$5/F0;~4V$NMh$?%?(B
 *  $B%$%^%$%Y%s%H$N5/F0$r9T$&!%(B
 */
void
timer_handler(void)
{
	TMEB	*event;

	clear_hw_timer_interrupt();		/* $B%?%$%^3d9~$_$N%/%j%"(B */

	BEGIN_CRITICAL_SECTION;
	current_time += TIMER_PERIOD;

	while (!queue_empty_p(&timer_queue)) {
		event = (TMEB *)(timer_queue.next);
		if (event->time <= current_time) {
			queue_delete(&(event->queue));
			if (event->callback) {
				(*(event->callback))(event->arg);
			}
		}
		else {
			break;
		}
	}
	END_CRITICAL_SECTION;
}

/*
 *  $B@-G=I>2AMQ%7%9%F%`;~9o;2>H5!G=(B
 */

#ifndef _i_vget_tim

SYSCALL ER
i_vget_tim(SYSUTIME *pk_utim)
{
	SYSTIME	ctime;
	TICK	tick;
	BOOL	ireq;

	BEGIN_CRITICAL_SECTION;
	ctime = current_time;
	tick = get_current_hw_time();
	ireq = fetch_hw_timer_interrupt();
	END_CRITICAL_SECTION;

	if (ireq && tick < (TO_TICK(TIMER_PERIOD) - GET_TOLERANCE)) {
		ctime += TIMER_PERIOD;
	}
	*pk_utim = ctime * 1000 + TO_USEC(tick);
	return(E_OK);
}

#endif /* _i_vget_tim */
