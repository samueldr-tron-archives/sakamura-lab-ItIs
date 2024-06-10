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
 *  @(#) $Id: timer.h,v 1.5 1998/01/30 09:52:51 hiro Exp $
 */

#ifndef _TIMER_
#define _TIMER_

/*
 *  $B%?%$%^%b%8%e!<%k(B
 */

#include "queue.h"

/* 
 *  $B%?%$%^%$%Y%s%H%V%m%C%/$NDj5A(B
 */

typedef void	(*CBACK)(VP);	/* $B%3!<%k%P%C%/4X?t$N7?(B */

typedef struct timer_event_block {
	QUEUE	queue;		/* $B%?%$%^%$%Y%s%H%-%e!<(B */
	SYSTIME	time;		/* $B%$%Y%s%H;~9o(B */
	CBACK	callback;	/* $B%3!<%k%P%C%/4X?t(B */
	VP	arg;		/* $B%3!<%k%P%C%/4X?t$XEO$90z?t(B */
} TMEB;

/*
 *  $B8=:_;~9o(B ($B%=%U%H%&%'%"%/%m%C%/(B)
 */
extern SYSTIME	current_time;

/*
 *  $B%?%$%^$N=i4|2=$HDd;_(B
 */
extern void	timer_initialize(void);
extern void	timer_shutdown(void);

/*
 *  $B%?%$%^%$%Y%s%H$N%?%$%^%-%e!<$X$NEPO?(B
 */
extern void	timer_insert(TMEB *event, TMO tmout,
				CBACK callback, VP arg);
extern void	timer_insert_abs(TMEB *event, SYSTIME *time,
				CBACK callback, VP arg);
extern void	timer_insert_rel(TMEB *event, SYSTIME *time,
				CBACK callback, VP arg);

/*
 *  $B%?%$%^%-%e!<$+$i$N:o=|(B
 */
Inline void
timer_delete(TMEB *event)
{
	queue_delete(&(event->queue));
}

#endif /* _TIMER_ */
