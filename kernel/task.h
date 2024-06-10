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
 *  @(#) $Id: task.h,v 1.18 1997/01/10 13:36:31 hiro Exp $
 */

#ifndef _TASK_
#define _TASK_

typedef struct task_control_block	TCB;
typedef struct generic_control_block	GCB;
typedef struct prisem_control_block	PISCB;

#include "queue.h"
#include "timer.h"
#include "winfo.h"

/*
 *  タスク状態の内部表現の定義
 *
 *  タスクが待ち状態かどうかは (state & TS_WAIT) でチェックできる.
 *  タスクが強制待ち状態かどうかは (state & TS_SUSPEND) でチェックできる.
 */
typedef enum {
	TS_NONEXIST = 0,	/* 未登録状態 */
	TS_READY = 1,		/* 実行または実行可能状態 */
	TS_WAIT = 2,		/* 待ち状態 */
	TS_SUSPEND = 4,		/* 強制待ち状態 */
	TS_WAITSUS = 6,		/* 二重待ち状態 */
	TS_DORMANT = 8		/* 休止状態 */
} TSTAT;

/*
 *  タスクが生きているか (NON-EXISTENT，DORMANT以外か) のチェック
 */
Inline BOOL
task_alive(TSTAT state)
{
	return(state & (TS_READY|TS_WAIT|TS_SUSPEND));
}

/*
 *  タスク優先度の内部/外部表現変換マクロ
 */
#define int_priority(x)	((INT)((x) - MIN_PRI))
#define ext_tskpri(x)	((PRI)((x) + MIN_PRI))

/*
 *  タスクコントロールブロック (TCB) の定義
 */
struct task_control_block {
	QUEUE	tskque;		/* タスクキュー */
	ID	tskid;		/* タスクID */
	VP	exinf;		/* 拡張情報 */
	ATR	tskatr;		/* タスク属性 */
	FP	task;		/* タスク起動アドレス */
	INT	ipriority;	/* タスク起動時優先度 */
#ifdef USE_SEPARATE_SSTACK
	INT	stksz;		/* ユーザスタックサイズ */
#endif /* USE_SEPARATE_SSTACK */
	INT	sstksz;		/* システムスタックサイズ */
	INT	priority;	/* 現在の優先度 */
	TSTAT	state;		/* タスク状態 (内部表現) */
	WSPEC	*wspec;		/* 待ち仕様 */
	GCB	*wgcb;		/* 待ちオブジェクトのコントロールブロック */
	INT	wupcnt;		/* 起床要求キューイング数 */
	INT	suscnt;		/* SUSPEND要求ネスト数 */
	INT	sysmode;	/* タスク動作モード，準タスク部呼出しレベル */
#ifdef USE_QTSK_PORTION
	INT	isysmode;	/* タスク動作モード初期値 */
#endif /* USE_QTSK_PORTION */

#ifdef USE_POR
	RNO	wrdvno;		/* ランデブ番号生成用 */
#endif /* USE_POR */
#ifdef PRISEM_SPEC1
	PISCB	*pislist;	/* 獲得優先度継承セマフォリスト */
#endif /* PRISEM_SPEC1 */
#ifdef USE_TASK_MAILBOX
	T_MSG	*tmq_head;	/* タスク付属メッセージキューの先頭 */
	T_MSG	*tmq_tail;	/* タスク付属メッセージキューの末尾 */
#endif /* USE_TASK_MAILBOX */
	ER	*wercd;		/* 待ちエラーコード設定エリア */
	WINFO	winfo;		/* 待ち情報 */
	TMEB	wtmeb;		/* 待ちタイマイベントブロック */

#ifdef USE_SEPARATE_SSTACK
	VP	istack;		/* ユーザスタックポインタの初期値 */
#endif /* USE_SEPARATE_SSTACK */
	VP	isstack;	/* システムスタックポインタの初期値 */
	CTXB	tskctxb;	/* タスクコンテキストブロック */
};

/*
 *  タスクディスパッチ禁止状態
 *
 *  dispatch_disabled は，タスクディスパッチ禁止状態を記憶しておくため
 *  の変数．遅延割込みを使う場合などには，必要ない．
 */
#ifdef USE_DISPATCH_DISABLED
extern BOOL	dispatch_disabled;
#endif /* USE_DISPATCH_DISABLED */

/*
 *  実行中のタスク
 *
 *  ctxtsk は，実行中のタスク (= CPU がコンテキストを持っているタスク) 
 *  の TCB を指す変数．システムコールの処理中で，システムコールを要求し
 *  たタスクに関する情報を参照する場合は，ctxtsk を使う．ctxtsk を書き
 *  換えるのは，タスクディスパッチャのみ．
 */
extern TCB	*ctxtsk;

/*
 *  実行すべきタスク
 *
 *  schedtsk は，実行すべきタスクの TCB を指す変数．遅延ディスパッチや
 *  ディスパッチの禁止によりディスパッチが遅延されている状態では，
 *  ctxtsk と一致しない．
 */
extern TCB	*schedtsk;

/*
 *  TCB のエリア
 *
 *  TCB のエリアは，システム生成時に静的に割り付けている．
 */
extern TCB	tcb_table[];

/*
 *  未使用の TCB のリスト
 */
#ifndef _i_vcre_tsk
extern QUEUE	free_tcb;
#endif /* _i_vcre_tsk */

/*
 *  タスクID から TCB を取り出す．
 */
#define get_tcb(id)	 (&tcb_table[INDEX_TSK(id)])
#define get_tcb_self(id) ((id)==TSK_SELF ? ctxtsk : &tcb_table[INDEX_TSK(id)])

/*
 *  タスクの実行準備をする．
 */
extern void	make_dormant(TCB *tcb);

/*
 *  タスクを実行可能状態にする．
 *
 *  tcb で示されるタスクの優先度が，現在実行中のタスクの優先度よりも高
 *  い場合は，実行状態にする．そうでない場合は，タスクをレディキューに
 *  つなぐ．
 */
extern void	make_ready(TCB *tcb);

/*
 *  タスクを実行可能以外の状態にする．
 *
 *  tcb で示されるタスクを実行可能以外の状態，つまり待ち状態，強制待ち
 *  状態，休止状態へ移行させる．この関数を呼び出す際には，タスクが実行
 *  可能状態になっていること．tcb->state は，呼び出した側で，この関数か
 *  らリターン後に変更する．
 */
extern void	make_non_ready(TCB *tcb);

/*
 *  タスクの優先度を変更する．
 *
 *  tcb で示されるタスクの優先度を priority に変更する．それに伴って必
 *  要となるタスクの状態遷移を起こさせる．
 */
extern void	change_task_priority(TCB *tcb, INT priority);

/*
 *  レディキューを回転させる．
 *
 *  rotate_ready_queue は，priority で指定された優先度のレディキューを
 *  回転させる．rotate_ready_queue_run は，レディキュー中の最高優先度
 *  のタスクを含むレディキューを回転させる．
 */
extern void	rotate_ready_queue(INT priority);
extern void	rotate_ready_queue_run(void);

#endif /* _TASK_ */
