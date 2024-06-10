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
 *  @(#) $Id: task.c,v 1.26 1998/01/30 09:52:44 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "ready_queue.h"
#include "cpu_task.h"

/*
 *  $B%?%9%/%G%#%9%Q%C%A6X;_>uBV(B
 */
#ifdef USE_DISPATCH_DISABLED
BOOL	dispatch_disabled;
#endif /* USE_DISPATCH_DISABLED */

/*
 *  $B<B9TCf$N%?%9%/(B
 */
TCB	*ctxtsk;

/*
 *  $B<B9T$9$Y$-%?%9%/(B
 */
TCB	*schedtsk;

/*
 *  TCB $B$N%(%j%"(B
 */
TCB	tcb_table[NUM_TSKID];

/*
 *  $B%l%G%#%-%e!<(B
 */ 
RDYQUE	ready_queue;

/*
 *  $BL$;HMQ$N(B TCB $B$N%j%9%H(B
 */
#ifndef _i_vcre_tsk
QUEUE	free_tcb;
#endif /* _i_vcre_tsk */

/*
 *  TCB $B$N=i4|2=(B
 */
void
task_initialize(void)
{
	INT	i;
	TCB	*tcb;
	ID	tskid;

	ctxtsk = schedtsk = (TCB *) 0;
	ready_queue_initialize(&ready_queue);
#ifndef _i_vcre_tsk
	queue_initialize(&free_tcb);
#endif /* _i_vcre_tsk */

	for(tcb = tcb_table, i = 0; i < NUM_TSKID; tcb++, i++) {
		tskid = ID_TSK(i);
		tcb->tskid = tskid;
		tcb->state = TS_NONEXIST;
#ifdef USE_POR
		tcb->wrdvno = (W)(UH) tskid;
#endif /* USE_POR */
#ifndef _i_vcre_tsk
		if (!SYS_TSKID(tskid)) {
			queue_insert(&(tcb->tskque), &free_tcb);
		}
#endif /* _i_vcre_tsk */
	}
#ifdef USE_DISPATCH_DISABLED
	dispatch_disabled = 0;
#endif /* USE_DISPATCH_DISABLED */
}

/*
 *  $B%?%9%/$N<B9T=`Hw$r$9$k!%(B
 */
void
make_dormant(TCB *tcb)
{
	/*
	 *  DORMANT$B>uBV$G$O%j%;%C%H$5$l$F$$$k$Y$-JQ?t$r=i4|2=!%(B
	 */
	tcb->state = TS_DORMANT;
	tcb->priority = tcb->ipriority;
	tcb->wupcnt = 0;
	tcb->suscnt = 0;
#ifdef USE_QTSK_PORTION
	tcb->sysmode = tcb->isysmode;
#endif /* USE_QTSK_PORTION */
#ifdef PRISEM_SPEC1
	tcb->pislist = (PISCB *) 0;
#endif /* PRISEM_SPEC1 */
#ifdef USE_TASK_MAILBOX
	tcb->tmq_head = (T_MSG *) 0;
#endif /* USE_TASK_MAILBOX */

	/*
	 *  $B%?%9%/5/F0$N$?$a$N%3%s%F%-%9%H$N@_Dj!%(B
	 */
	setup_context(tcb);
}

/*
 *  $B<B9T$9$Y$-%?%9%/$rA*Br$7D>$9!%(B
 *
 *  schedtsk $B$r%l%G%#%-%e!<$N@hF,$N%?%9%/$H0lCW$5$;$k!%(B
 */
Inline void
reschedule(void)
{
	TCB	*toptsk;

	if (schedtsk != (toptsk = ready_queue_top(&ready_queue))) {
		schedtsk = toptsk;
		dispatch_request();
	}
}

/*
 *  $B%?%9%/$r<B9T2DG=>uBV$K$9$k!%(B
 *
 *  $B%?%9%/>uBV$r99?7$7!$%l%G%#%-%e!<$KA^F~$9$k!%I,MW$J$i!$(Bschedtsk $B$r(B
 *  $B99?7$7!$%?%9%/%G%#%9%Q%C%A%c$N5/F0$rMW5a$9$k!%(B
 */
void
make_ready(TCB *tcb)
{
	tcb->state = TS_READY;
	if (ready_queue_insert(&ready_queue, tcb)) {
		schedtsk = tcb;
		dispatch_request();
	}
}

/*
 *  $B%?%9%/$r<B9T2DG=0J30$N>uBV$K$9$k!%(B
 *
 *  $B%?%9%/$r%l%G%#%-%e!<$+$i:o=|$9$k!%:o=|$7$?%?%9%/$,(B schedtsk $B$G$"$C(B
 *  $B$?>l9g$K$O!$(Bschedtsk $B$r%l%G%#%-%e!<Cf$N:G9bM%@hEY%?%9%/$K@_Dj$9$k!%(B
 */
void
make_non_ready(TCB *tcb)
{
	assert(tcb->state == TS_READY);
	ready_queue_delete(&ready_queue, tcb);
	if (schedtsk == tcb) {
		schedtsk = ready_queue_top(&ready_queue);
		dispatch_request();
	}
}

/*
 *  $B%?%9%/$NM%@hEY$rJQ99$9$k!%(B
 */
void
change_task_priority(TCB *tcb, INT priority)
{
	INT	oldpri;

	if (tcb->state == TS_READY) {
		/*
		 *  $B%?%9%/$r%l%G%#%-%e!<$+$i:o=|$9$k:]$K(B TCB $B$N(B
		 *  priority $B%U%#!<%k%I$NCM$,I,MW$K$J$k$?$a!$%l(B
		 *  $B%G%#%-%e!<$+$i$N:o=|$O!$(Btcb->priority $B$r=q(B
		 *  $B$-49$($kA0$K9T$o$J$1$l$P$J$i$J$$!%(B
		 */
		ready_queue_delete(&ready_queue, tcb);
		tcb->priority = priority;
		ready_queue_insert(&ready_queue, tcb);
		reschedule();
	}
	else {
		oldpri = tcb->priority;
		tcb->priority = priority;
		if ((tcb->state & TS_WAIT) && tcb->wspec->chg_pri_hook) {
			(*(tcb->wspec->chg_pri_hook))(tcb, oldpri);
		}
	}
}

/*
 *  $B%l%G%#%-%e!<$r2sE>$5$;$k!%(B
 */
void
rotate_ready_queue(INT priority)
{
	ready_queue_rotate(&ready_queue, priority);
	reschedule();
}

/*
 *  $B:G9bM%@hEY$N%?%9%/$r4^$`%l%G%#%-%e!<$r2sE>$5$;$k!%(B
 */
void
rotate_ready_queue_run(void)
{
	if (schedtsk) {
		ready_queue_rotate(&ready_queue,
				ready_queue_top_priority(&ready_queue));
		reschedule();
	}
}
