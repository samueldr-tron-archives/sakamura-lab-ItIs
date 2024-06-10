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
 *  @(#) $Id: task_sync.c,v 1.12 1998/01/30 09:52:47 hiro Exp $
 */

/*
 *  タスク付属同期機能
 */

#include <limits.h>
#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifndef _i_sus_tsk

SYSCALL ER
i_sus_tsk(ID tskid)
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

#ifndef USE_SUSPEND_SELF
	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	tcb = get_tcb(tskid);
#else /* USE_SUSPEND_SELF */
	CHECK_TSKID_SELF(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb_self(tskid);
#endif /* USE_SUSPEND_SELF */

	BEGIN_CRITICAL_SECTION;
	if (!task_alive(state = tcb->state)) {
		ercd = (state == TS_NONEXIST) ? E_NOEXS : E_OBJ;
	}
	else if (tcb->suscnt == INT_MAX) {
		ercd = E_QOVR;
	}
	else {
		++(tcb->suscnt);
		if (state == TS_READY) {
			make_non_ready(tcb);
			tcb->state = TS_SUSPEND;
		}
		else if (state == TS_WAIT) {
			tcb->state = TS_WAITSUS;
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_sus_tsk */
#ifndef _i_rsm_tsk

SYSCALL ER
i_rsm_tsk(ID tskid)
{
	TCB	*tcb;
	ER	ercd = E_OK;

#ifndef USE_SUSPEND_SELF
	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	tcb = get_tcb(tskid);
#else /* USE_SUSPEND_SELF */
	CHECK_TSKID_SELF(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb_self(tskid);
#endif /* USE_SUSPEND_SELF */

	BEGIN_CRITICAL_SECTION;
	switch (tcb->state) {
	case TS_NONEXIST:
		ercd = E_NOEXS;
		break;
	case TS_DORMANT:
	case TS_READY:
	case TS_WAIT:
		ercd = E_OBJ;
		break;
	case TS_SUSPEND:
		if (--(tcb->suscnt) == 0) {
			make_ready(tcb);
		}
		break;
	case TS_WAITSUS:
		if (--(tcb->suscnt) == 0) {
			tcb->state = TS_WAIT;
		}
		break;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_rsm_tsk */
#ifndef _i_frsm_tsk

SYSCALL ER
i_frsm_tsk(ID tskid)
{
	TCB	*tcb;
	ER	ercd = E_OK;

#ifndef USE_SUSPEND_SELF
	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	tcb = get_tcb(tskid);
#else /* USE_SUSPEND_SELF */
	CHECK_TSKID_SELF(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb_self(tskid);
#endif /* USE_SUSPEND_SELF */

	BEGIN_CRITICAL_SECTION;
	switch (tcb->state) {
	case TS_NONEXIST:
		ercd = E_NOEXS;
		break;
	case TS_DORMANT:
	case TS_READY:
	case TS_WAIT:
		ercd = E_OBJ;
		break;
	case TS_SUSPEND:
		tcb->suscnt = 0;
		make_ready(tcb);
		break;
	case TS_WAITSUS:
		tcb->suscnt = 0;
		tcb->state = TS_WAIT;
		break;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_frsm_tsk */

/*
 *  起床待ち仕様の定義
 */
#if !defined(_i_slp_tsk) || !defined(_i_tslp_tsk) || !defined(_i_wup_tsk)
static WSPEC wspec_slp = { TTW_SLP, 0, 0 };
#endif

#ifndef _i_slp_tsk

SYSCALL ER
i_slp_tsk(void)
{
	return(i_tslp_tsk(TMO_FEVR));
}

#endif /* _i_slp_tsk */
#if !defined(_i_slp_tsk) || !defined(_i_tslp_tsk)

SYSCALL ER
i_tslp_tsk(TMO tmout)
{
	ER	ercd = E_OK;

	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	BEGIN_CRITICAL_SECTION;
	if (ctxtsk->wupcnt > 0) {
		(ctxtsk->wupcnt)--;
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = &wspec_slp;
			ctxtsk->wgcb = (GCB *) 0;
			ctxtsk->wercd = &ercd;
			make_wait(tmout);
			queue_initialize(&(ctxtsk->tskque));
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_tslp_tsk */
#ifndef _i_wup_tsk

SYSCALL ER
i_wup_tsk(ID tskid)
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if (!task_alive(state = tcb->state)) {
		ercd = (state == TS_NONEXIST) ? E_NOEXS : E_OBJ;
	}
	else if ((state & TS_WAIT) && tcb->wspec == &wspec_slp) {
		wait_release_ok(tcb);
	}
	else if (tcb->wupcnt == INT_MAX) {
		ercd = E_QOVR;
	}
	else {
		++(tcb->wupcnt);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_wup_tsk */
#ifndef _i_can_wup

SYSCALL ER
i_can_wup(W *p_wupcnt, ID tskid)
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	switch (tcb->state) {
	case TS_NONEXIST:
		ercd = E_NOEXS;
		break;
	case TS_DORMANT:
		ercd = E_OBJ;
		break;
	default:
		*p_wupcnt = tcb->wupcnt;
		tcb->wupcnt = 0;
		break;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_can_wup */
