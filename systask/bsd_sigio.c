/**
 * 
 * 	ItIs - An ITRON Implementation for Research and Education
 * 
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
 * 3. Neither the name of the university nor the name of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: bsd_sigio.c,v 1.9 1998/07/06 14:44:26 hiro Exp $
 */

/*
 *  BSD UNIX用 ノンブロッキングI/O サポートモジュール
 */

#include "systask.h"
#include "../kernel/queue.h"
#include <signal.h>
#include <bsd_sigio.h>

/*
 *  SIGIO通知イベントキュー
 */

static QUEUE	sigio_queue;

/*
 *  SIGIO通知ハンドラ
 *
 *  SIGIO通知処理タスクを起床する．
 */

static void
bsd_sigio_handler()
{
	syscall(wup_tsk(TSK_BSD_SIGIO));
}

/*
 *  ノンブロッキングI/O モジュール起動ルーチン
 *
 *  SIGIO通知イベントキューを初期化．SIGIO通知処理タスクとセマフォを生
 *  成．SIGIO通知ハンドラを設定．
 */

static int	initflag = 0;		/* 初期化済フラグ */

void
bsd_sigio_startup(void)
{
	struct sigvec	vec;

	if (!initflag) {
		queue_initialize(&sigio_queue);

		syscall(cre_tsk(TSK_BSD_SIGIO, &TSK_BSD_SIGIO_CTSK));
		syscall(sta_tsk(TSK_BSD_SIGIO, 0));
		syscall(cre_sem(SEM_BSD_SIGIO, &SEM_BSD_SIGIO_CSEM));

		vec.sv_handler = bsd_sigio_handler;
		vec.sv_mask = sigmask(SIGUSR2);
		vec.sv_flags = SV_ONSTACK|SV_INTERRUPT;
		sigvec(SIGIO, &vec, 0);
		initflag = 1;
	}
}

/*
 *  SIGIO通知処理タスク本体
 */

void
bsd_sigio_task(void)
{
	QUEUE	*q;
	SIGIOEB	*event;

	while (slp_tsk() == E_OK) {
		/*
		 *  SIGIO通知イベントキュー中の各イベントブロックのコー
		 *  ルバックルーチンを呼びだす．コールバックルーチンが
		 *  0 以外を返した場合，そのイベントブロックを削除する．
		 */
		syscall(wai_sem(SEM_BSD_SIGIO));
		q = sigio_queue.next;
		while (q != &sigio_queue) {
			event = ((SIGIOEB *) q);
			q = q->next;
			if ((*(event->callback))(event->arg)) {
				queue_delete((QUEUE *) &(event->queue));
			}
		}
		syscall(sig_sem(SEM_BSD_SIGIO));
	}
	ext_tsk();
}

/*
 *  SIGIO通知イベントをSIGIO通知イベントキューへ挿入
 *
 *  システムオブジェクトにアクセスするために，拡張SVCハンドラとして動
 *  作させている．
 */

ER
enqueue_sigioeb(SIGIOEB *event)
{
	ENTER_EXTENDED_SVC;
	syscall(wai_sem(SEM_BSD_SIGIO));
	queue_insert((QUEUE *) &(event->queue), &sigio_queue);
	syscall(sig_sem(SEM_BSD_SIGIO));
	EXIT_EXTENDED_SVC;
	return(E_OK);
}
