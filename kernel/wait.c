/**
 * 
 * 	    ItIs - ITRON Implementation by Sakamura Lab
 * 
 * Copyright (C) 1989-1996 by Sakamura Lab, the University of Tokyo, JAPAN
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: wait.c,v 1.8 1997/01/10 13:15:28 hiro Exp $
 */

/*
 *  タスク間同期・通信オブジェクト汎用ルーチン
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"

/*
 *  待ち状態が解除するようタスク状態を更新する．レディ状態になる場合は，
 *  レディキューにつなぐ．
 *
 *  タスクが待ち状態 (二重待ちを含む) の時に呼ぶこと．
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
 *  タスクの待ち状態を解除する．
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
 *  実行中のタスクを待ち状態に移行させ，タイマイベントキューにつなぐ．
 *
 *  ctxtsk は RUN状態になっているのが通常であるが，実行中のシステムコー 
 *  ルの途中で割込みが発生し，その割込みハンドラ中で呼ばれたシステムコー 
 *  ルによって ctxtsk がその他の状態になる場合がある．ただし，WAIT状態
 *  になることはない．
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
 *  待ちキューにつながっているタスクの待ち状態をすべて解除し，E_DLTエラー
 *  とする．
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
 *  待ちキューの先頭のタスクの ID を取り出す．
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
 *  タスクを優先度順の待ちキューにつなぐ．
 */
Inline void
queue_insert_tpri(TCB *tcb, QUEUE *queue)
{
	QUEUE	*q;

	q = queue_search_gt(queue, tcb->priority, offsetof(TCB, priority));
	queue_insert(&(tcb->tskque), q);
}

/*
 *  実行中のタスクを待ち状態に移行させ，タイマイベントキューおよびオブ
 *  ジェクトの待ちキューにつなぐ．また，ctxtsk の wgcb を設定する．
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
 *  タスクの優先度が変わった際に，待ちキューの中でのタスクの位置を修正
 *  する．オブジェクト属性に TA_TPRI が指定されている場合にのみ，呼び出
 *  される．
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
