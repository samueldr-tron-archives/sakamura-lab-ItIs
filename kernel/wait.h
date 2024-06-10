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
 *  @(#) $Id: wait.h,v 1.11 1998/01/30 09:52:54 hiro Exp $
 */

#ifndef _WAIT_
#define _WAIT_

/*
 *  タスク間同期・通信オブジェクト汎用ルーチン
 */

#include "queue.h"
#include "timer.h"
#include "task.h"

/*
 *  タスクの待ち状態を解除する．
 *
 *  タスクをタイマキューおよび待ちキューからはずし，タスク状態を更新す
 *  る．wait_release_ok は，待ち解除されたタスクに E_OK を渡す．
 *  wait_release_tmout は，タイマキューからはずす処理をしない．タイム
 *  アウト処理の時に用いる．
 */
extern void	wait_release(TCB *tcb);
extern void	wait_release_ok(TCB *tcb);
extern void	wait_release_tmout(TCB *tcb);

/*
 *  タスクの待ち状態をキャンセルする．
 *
 *  タスクをタイマキューおよび待ちキューからはずす．タスク状態は更新し
 *  ない．
 */
Inline void
wait_cancel(TCB *tcb)
{
	timer_delete(&(tcb->wtmeb));
	queue_delete(&(tcb->tskque));
}

/*
 *  実行中のタスクを待ち状態に移行させ，タイマイベントキューにつなぐ．
 */
extern void	make_wait(TMO tmout);

/*
 *  待ちキューにつながっているタスクの待ち状態をすべて解除し，E_DLTエラー
 *  とする．
 *
 *  タスク間同期・通信オブジェクトが削除された時に使う．
 */
extern void	wait_delete(QUEUE *wait_queue);

/*
 *  待ちキューの先頭のタスクの ID を取り出す．
 */
extern ID	wait_tskid(QUEUE *wait_queue);

/*
 *  コントロールブロック共通部分操作ルーチン
 *
 *  タスク間同期・通信オブジェクトはいずれも，コントロールブロックの先
 *  頭部分が共通になっている．以下は，その共通部分を扱うための汎用ルー
 *  チンである．共通部分を，GCB (汎用コントロールブロック) という型にす
 *  る．複数の待ちキューを持つオブジェクトの場合，2つめ以降の待ちキュー
 *  を操作する場合には，これらのルーチンは使えない．また，オブジェクト
 *  属性の TA_TPRIビットを参照するので，このビットを他の目的に使ってい
 *  る場合も，これらのルーチンは使えない．オブジェクトが生成されていな
 *  い場合には，オブジェクト属性を OBJ_NONEXIST にする (そのため，オブ
 *  ジェクト属性のすべてのビットを独立に使うことができないという制限が
 *  ある)．
 */

struct generic_control_block {
	QUEUE	wait_queue;	/* 待ちキュー */
	ID	objid;		/* オブジェクトID */
	VP	exinf;		/* 拡張情報 */
	ATR	objatr;		/* オブジェクト属性 */
	/*  これ以降に他のフィールドがあってもよいが， */
	/*  汎用操作ルーチンでは扱われない． */
};

#define OBJ_NONEXIST	(-1)

/*
 *  実行中のタスクを待ち状態に移行させ，タイマイベントキューおよびオブ
 *  ジェクトの待ちキューにつなぐ．また，ctxtsk の wid を設定する．
 */
extern void	gcb_make_wait(GCB *gcb, TMO tmout);

/*
 *  タスクの優先度が変わった際に，待ちキューの中でのタスクの位置を修正
 *  する．オブジェクト属性に TA_TPRI が指定されている場合にのみ，呼び出
 *  される．
 */
extern void	gcb_change_priority(GCB *gcb, TCB *tcb);
extern void	obj_chg_pri(TCB *tcb, INT oldpri);

#endif /* _WAIT_ */
