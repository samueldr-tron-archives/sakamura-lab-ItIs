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
 *  @(#) $Id: wait.c,v 1.9 1998/01/30 09:52:53 hiro Exp $
 */

/*
 *  $B%?%9%/4VF14|!&DL?.%*%V%8%'%/%HHFMQ%k!<%A%s(B
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"

/*
 *  $BBT$A>uBV$,2r=|$9$k$h$&%?%9%/>uBV$r99?7$9$k!%%l%G%#>uBV$K$J$k>l9g$O!$(B
 *  $B%l%G%#%-%e!<$K$D$J$0!%(B
 *
 *  $B%?%9%/$,BT$A>uBV(B ($BFs=EBT$A$r4^$`(B) $B$N;~$K8F$V$3$H!%(B
 */
Inline void
make_non_wait(TCB *tcb)
{
	assert(tcb->state & TS_WAIT);

	if (tcb->state == TS_WAIT) {
		make_ready(tcb);
	}
	else {
		tcb->state = TS_SUSPEND;
	}
}

/*
 *  $B%?%9%/$NBT$A>uBV$r2r=|$9$k!%(B
 */
__inline__ void
wait_release(TCB *tcb)
{
	timer_delete(&(tcb->wtmeb));
	queue_delete(&(tcb->tskque));
	make_non_wait(tcb);
}

void
wait_release_ok(TCB *tcb)
{
	*(tcb->wercd) = E_OK;
	wait_release(tcb);
}

void
wait_release_tmout(TCB *tcb)
{
	queue_delete(&(tcb->tskque));
	make_non_wait(tcb);
#ifdef USE_REL_WAI_HOOK
	if (tcb->wspec->rel_wai_hook) {
		(*(tcb->wspec->rel_wai_hook))(tcb);
	}
#endif /* USE_REL_WAI_HOOK */
}

/*
 *  $B<B9TCf$N%?%9%/$rBT$A>uBV$K0\9T$5$;!$%?%$%^%$%Y%s%H%-%e!<$K$D$J$0!%(B
 *
 *  ctxtsk $B$O(B RUN$B>uBV$K$J$C$F$$$k$N$,DL>o$G$"$k$,!$<B9TCf$N%7%9%F%`%3!<(B 
 *  $B%k$NESCf$G3d9~$_$,H/@8$7!$$=$N3d9~$_%O%s%I%iCf$G8F$P$l$?%7%9%F%`%3!<(B 
 *  $B%k$K$h$C$F(B ctxtsk $B$,$=$NB>$N>uBV$K$J$k>l9g$,$"$k!%$?$@$7!$(BWAIT$B>uBV(B
 *  $B$K$J$k$3$H$O$J$$!%(B
 */
void
make_wait(TMO tmout)
{
	assert(!(ctxtsk->state & TS_WAIT));

	switch (ctxtsk->state) {
	case TS_READY:
		make_non_ready(ctxtsk);
		ctxtsk->state = TS_WAIT;
		break;
	case TS_SUSPEND:
		ctxtsk->state = TS_WAITSUS;
		break;
	}
	timer_insert(&(ctxtsk->wtmeb), tmout,
				(CBACK) wait_release_tmout, ctxtsk);
}

/*
 *  $BBT$A%-%e!<$K$D$J$,$C$F$$$k%?%9%/$NBT$A>uBV$r$9$Y$F2r=|$7!$(BE_DLT$B%(%i!<(B
 *  $B$H$9$k!%(B
 */
void
wait_delete(QUEUE *wait_queue)
{
	TCB	*tcb;

	while (!queue_empty_p(wait_queue)) {
		tcb = (TCB *)(wait_queue->next);
		*(tcb->wercd) = E_DLT;
		wait_release(tcb);
	}
}

/*
 *  $BBT$A%-%e!<$N@hF,$N%?%9%/$N(B ID $B$r<h$j=P$9!%(B
 */
ID
wait_tskid(QUEUE *wait_queue)
{
	if (queue_empty_p(wait_queue)) {
		return((ID) FALSE);
	}
	else {
		return(((TCB *)(wait_queue->next))->tskid);
	}
}

/*
 *  $B%?%9%/$rM%@hEY=g$NBT$A%-%e!<$K$D$J$0!%(B
 */
Inline void
queue_insert_tpri(TCB *tcb, QUEUE *queue)
{
	QUEUE	*q;

	q = queue_search_gt(queue, tcb->priority, offsetof(TCB, priority));
	queue_insert(&(tcb->tskque), q);
}

/*
 *  $B<B9TCf$N%?%9%/$rBT$A>uBV$K0\9T$5$;!$%?%$%^%$%Y%s%H%-%e!<$*$h$S%*%V(B
 *  $B%8%'%/%H$NBT$A%-%e!<$K$D$J$0!%$^$?!$(Bctxtsk $B$N(B wgcb $B$r@_Dj$9$k!%(B
 */
void
gcb_make_wait(GCB *gcb, TMO tmout)
{
	ctxtsk->wgcb = gcb;
	make_wait(tmout);
	if (gcb->objatr & TA_TPRI) {
		queue_insert_tpri(ctxtsk, &(gcb->wait_queue));
	}
	else {
		queue_insert(&(ctxtsk->tskque), &(gcb->wait_queue));
	}
}

/*
 *  $B%?%9%/$NM%@hEY$,JQ$o$C$?:]$K!$BT$A%-%e!<$NCf$G$N%?%9%/$N0LCV$r=$@5(B
 *  $B$9$k!%%*%V%8%'%/%HB0@-$K(B TA_TPRI $B$,;XDj$5$l$F$$$k>l9g$K$N$_!$8F$S=P(B
 *  $B$5$l$k!%(B
 */
inline void
gcb_change_priority(GCB *gcb, TCB *tcb)
{
	assert(gcb->objatr & TA_TPRI);
	queue_delete(&(tcb->tskque));
	queue_insert_tpri(tcb, &(gcb->wait_queue));
}

void
obj_chg_pri(TCB *tcb, INT oldpri)
{
	gcb_change_priority(tcb->wgcb, tcb);
}
