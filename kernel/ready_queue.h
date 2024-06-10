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
 *  @(#) $Id: ready_queue.h,v 1.9 1997/01/10 11:50:14 hiro Exp $
 */

#ifndef _READY_QUEUE_
#define _READY_QUEUE_

/*
 *  レディキュー操作ルーチン
 */

#include "queue.h"

/*
 *  レディキューの構造の定義
 *
 *  レディキュー中には，優先度ごとのタスクキュー tskque が用意されてお
 *  り，タスクの TCB は，該当する優先度のキューに登録される．
 *  レディキューのサーチを効率よく行うために，優先度ごとのタスクキュー
 *  にタスクが入っているかどうかを示すビットマップ領域 bitmap を用意し
 *  ている．この領域を使うことで，ワード単位のサーチが可能になり，メモ
 *  リアクセスの回数を減らすことができる．ただし，優先度のレベル数が少
 *  なく，ビット操作命令が充実していない CPU の場合には，ビットマップ操
 *  作のオーバーヘッドのために，逆に効率が落ちる可能性もある．TRON仕様
 *  チップ用のコードと汎用コードで，ワード内のビットの番号付けが逆にな
 *  っているので注意せよ．
 *  また，レディキュー中の最高優先度のタスクを参照する操作を効率よく行
 *  うために，レディキュー中の最高優先度のタスクの優先度を top_priority
 *  フィールドに入れておく．レディキューが空の場合には，このフィールド
 *  の値を NUM_PRI にする．この時に tskque[top_priority] を参照すると 
 *  NULL を返したいため，常に NULL が入っている nullフィールドを用意し
 *  ている．
 */

#define BITMAPSZ	(sizeof(UINT) * 8)
#define NUM_BITMAP	((NUM_PRI + BITMAPSZ - 1) / BITMAPSZ)

typedef	struct ready_queue {
	INT	top_priority;		/* レディキュー中の最高優先度 */
	QUEUE	tskque[NUM_PRI];	/* 優先度ごとのタスクキュー */
	TCB	*null;			/* レディキューが空になった時のため */
	UINT	bitmap[NUM_BITMAP];	/* 優先度ごとのビットマップ領域 */
} RDYQUE;

/*
 *  ビットマップ領域操作ライブラリ
 *
 *  汎用コードで (1 << xxx) としている部分は，テーブルを用意した方が効
 *  率がよいと思われるが，テーブルの作り方が CPU のワード長に依存してし
 *  まうので，(1 << xxx) の形にしている．
 */

/*
 *  レディキュー rq のビットマップ領域中の，優先度 priority に対応する
 *  ビットをセットする．
 */
Inline void
bitmap_set(RDYQUE *rq, INT priority)
{
#ifdef tron
	Asm("bset %0, %a1" : : "g"(priority), "g"(rq->bitmap) : "memory");
#else /* tron */
	rq->bitmap[priority / BITMAPSZ] |= 1 << (priority % BITMAPSZ);
#endif /* tron */
}

/*
 *  レディキュー rq のビットマップ領域中の，優先度 priority に対応する
 *  ビットをクリアする．
 */
Inline void
bitmap_clear(RDYQUE *rq, INT priority)
{
#ifdef tron
	Asm("bclr %0, %a1" : : "g"(priority), "g"(rq->bitmap) : "memory");
#else /* tron */
	rq->bitmap[priority / BITMAPSZ] &= ~(1 << (priority % BITMAPSZ));
#endif /* tron */
}

/*
 *  以下の _ffs関数は，標準の ffs関数と違い，i が 0 の場合は考慮してお
 *  らず，返す値も 0 をベースにしている．また，TRON仕様チップ用のコード
 *  では，サーチする方向が逆になっている．
 *
 *  標準ライブラリに ffs があるなら，次のように定義して，標準ライブラリ
 *  を使った方が効率が良い可能性が高い．
 *	#define _ffs(i) (ffs(i) - 1)
 *  TRON仕様チップの標準設定では，任意長ビットフィールド操作命令を使う
 *  ので _ffs関数は使われないが，bvsch 命令がない場合にはこの関数を使う
 *  必要がある．
 */
Inline INT
_ffs(INT i)
{
#ifdef tron
	Asm("bsch/1/f %0, %2" : "=g"(i) : "0"(i), "rm"(0));
	return(i);
#else /* tron */
	INT	j = 0;

	assert(i != 0);
	while ((i & 1) == 0) {
		i >>= 1;
		j++;
	}
	return(j);
#endif /* tron */
}

/*
 *  レディキューの初期化
 */
Inline void
ready_queue_initialize(RDYQUE *rq)
{
	INT	i;

	rq->top_priority = NUM_PRI;
	for (i = 0; i < NUM_PRI; i++) {
		queue_initialize(&rq->tskque[i]);
	}
	rq->null = (TCB *) 0;
	memset(rq->bitmap, 0, sizeof(rq->bitmap));
}

/*
 *  レディキュー中の最高優先度のタスクを参照する．
 *
 *  レディキューが空の時は NULL を返す．
 */
Inline TCB *
ready_queue_top(RDYQUE *rq)
{
	return((TCB *)(rq->tskque[rq->top_priority].next));
}

/*
 *  レディキュー中の最高優先度のタスクの優先度を参照する．
 *
 *  レディキューが空の時には呼ばれない．
 */
Inline INT
ready_queue_top_priority(RDYQUE *rq)
{
	return(rq->top_priority);
}

/*
 *  タスクをレディキューに挿入する．
 *
 *  tcb で指すタスクの優先度と同じ優先度を持つタスクの中での最後に入れ
 *  る．
 *  ビットマップ領域の該当するビットをセットし，必要なら top_priority 
 *  を更新する．top_priority を更新した時は 1，そうでない場合は 0 を返
 *  す (インライン関数なので，返り値が不要な場合でも無駄なコードは出な
 *  い)．
 */
Inline BOOL
ready_queue_insert(RDYQUE *rq, TCB *tcb)
{
	INT	priority = tcb->priority;

	queue_insert(&(tcb->tskque), &(rq->tskque[priority]));
	bitmap_set(rq, priority);
	if (priority < rq->top_priority) {
		rq->top_priority = priority;
		return(1);
	}
	return(0);
}

/*
 *  タスクをレディキューに挿入する．
 *
 *  tcb で指すタスクの優先度と同じ優先度を持つタスクの中での先頭に入れ
 *  る．RUN状態のタスクをレディキューから外しておく実装で，タスクがプ
 *  リエンプトされた場合に使う．
 *  ビットマップ領域の該当するビットをセットし，必要なら top_priority 
 *  を更新する．
 */
Inline void
ready_queue_insert_top(RDYQUE *rq, TCB *tcb)
{
	INT	priority = tcb->priority;

	queue_insert(&(tcb->tskque), rq->tskque[priority].next);
	bitmap_set(rq, priority);
	if (priority < rq->top_priority) {
		rq->top_priority = priority;
	}
}

/*
 *  タスクをレディキューから削除する．
 *
 *  TCB を該当する優先度のタスクキューからはずし，それによってタスクキュー
 *  が空になった場合には，ビットマップ領域の該当するビットをクリアする．
 *  さらに，削除したタスクが最高優先度であった時は，top_priority を更新
 *  する．この際に，次に優先度の高いタスクをサーチするために，ビットマッ
 *  プ領域を使う．
 */
Inline void
ready_queue_delete(RDYQUE *rq, TCB *tcb)
{
#if defined(tron) && !defined(TRON_LEVEL1)
   Asm("qdel %a1, r0	\n"	/* タスクキューからの削除 */
"	bne 1f		\n"	/* キューが空にならない時はこれで終了 */
"	mov %2 ,r0	\n"
"	mov %3, r1	\n"
"	bclr r1, @r0	\n"	/* 該当ビットのクリア */
"	cmp %4, r1	\n"
"	bne 1f		\n"	/* より高優先度のタスクがある時は終了 */
"	mov %5, r2	\n"
"	sub r1, r2	\n"	/* ビットサーチするビット幅を計算 */
"	bvsch/1/f	\n"
"	mov r1, %0	\n"	/* top_priority を更新 */
"1:			"
:	"=g"(rq->top_priority)
:	"g"(tcb->tskque.prev), "g"(rq->bitmap), "g"(tcb->priority),
	"g"(rq->top_priority), "g"(NUM_PRI)
:	"r0", "r1", "r2", "memory");
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	INT	priority = tcb->priority;
	INT	i;

	queue_delete(&(tcb->tskque));
	if (!queue_empty_p(&(rq->tskque[priority]))) {
		return;
	}
	bitmap_clear(rq, priority);
	if (priority != rq->top_priority) {
		return;
	}
	for (i = priority / BITMAPSZ; i < NUM_BITMAP; i++) {
		if (rq->bitmap[i]) {
			rq->top_priority = i * BITMAPSZ + _ffs(rq->bitmap[i]);
			return;
		}
	}
	rq->top_priority = NUM_PRI;
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
}

/*
 *  レディキューの優先度レベル priority のキューの先頭のタスクを，キュー
 *  最後へ移動する．キューが空の場合は，何もしない．
 *
 *  ready_queue_delete と ready_queue_insert を続けて呼んでも同じことが
 *  実現できるが，ready_queue_rotate の場合はビットマップ領域の操作は必
 *  要ないため，効率をよくするために別の関数にしている．
 */
Inline void
ready_queue_rotate(RDYQUE *rq, INT priority)
{
#ifdef tron
   Asm("qdel %a0, r0	\n"
"	bvs 1f		\n"
"	qins @r0, %a0	\n"
"1:			"
:	/* no output */
:	"g"(&(rq->tskque[priority]))
:	"r0", "memory");
#else /* tron */
	QUEUE	*entry, *queue;

	if (!queue_empty_p(queue = &(rq->tskque[priority]))) {
		entry = queue_delete_next(queue);
		queue_insert(entry, queue);
	}
#endif /* tron */
}

#endif /* _READY_QUEUE_ */
