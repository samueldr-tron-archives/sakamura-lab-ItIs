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
 *  タスクディスパッチ禁止状態
 */
#ifdef USE_DISPATCH_DISABLED
BOOL	dispatch_disabled;
#endif /* USE_DISPATCH_DISABLED */

/*
 *  実行中のタスク
 */
TCB	*ctxtsk;

/*
 *  実行すべきタスク
 */
TCB	*schedtsk;

/*
 *  TCB のエリア
 */
TCB	tcb_table[NUM_TSKID];

/*
 *  レディキュー
 */ 
RDYQUE	ready_queue;

/*
 *  未使用の TCB のリスト
 */
#ifndef _i_vcre_tsk
QUEUE	free_tcb;
#endif /* _i_vcre_tsk */

/*
 *  TCB の初期化
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
 *  タスクの実行準備をする．
 */
void
make_dormant(TCB *tcb)
{
	/*
	 *  DORMANT状態ではリセットされているべき変数を初期化．
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
	 *  タスク起動のためのコンテキストの設定．
	 */
	setup_context(tcb);
}

/*
 *  実行すべきタスクを選択し直す．
 *
 *  schedtsk をレディキューの先頭のタスクと一致させる．
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
 *  タスクを実行可能状態にする．
 *
 *  タスク状態を更新し，レディキューに挿入する．必要なら，schedtsk を
 *  更新し，タスクディスパッチャの起動を要求する．
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
 *  タスクを実行可能以外の状態にする．
 *
 *  タスクをレディキューから削除する．削除したタスクが schedtsk であっ
 *  た場合には，schedtsk をレディキュー中の最高優先度タスクに設定する．
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
 *  タスクの優先度を変更する．
 */
void
change_task_priority(TCB *tcb, INT priority)
{
	INT	oldpri;

	if (tcb->state == TS_READY) {
		/*
		 *  タスクをレディキューから削除する際に TCB の
		 *  priority フィールドの値が必要になるため，レ
		 *  ディキューからの削除は，tcb->priority を書
		 *  き換える前に行わなければならない．
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
 *  レディキューを回転させる．
 */
void
rotate_ready_queue(INT priority)
{
	ready_queue_rotate(&ready_queue, priority);
	reschedule();
}

/*
 *  最高優先度のタスクを含むレディキューを回転させる．
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
