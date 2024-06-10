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
 *  @(#) $Id: queue.h,v 1.6 1996/02/17 09:33:28 hiro Exp $
 */

#ifndef	_QUEUE_
#define	_QUEUE_

/*
 *  ダブルリンクキュー
 * 
 *  ダブルリンクキューの構造は，TRON仕様チップのキュー操作命令が扱う形
 *  式にあわせてある．
 */
typedef struct queue {
	struct queue *next;
	struct queue *prev;
} QUEUE;

/*
 *  キュー操作ライブラリ
 */

/*
 *  キューの初期化
 */
Inline void
queue_initialize(QUEUE *queue)
{
	queue->prev = queue->next = queue;
}

/*
 *  キューへエントリを挿入
 *
 *  queue の直前に entry を挿入する．queue がキューヘッダを指す場合には，
 *  キューの最後尾に entry を挿入することになる．
 */
Inline void
queue_insert(QUEUE *entry, QUEUE *queue)
{
#ifdef tron
	Asm("qins %a0, %a1" : : "g"(entry), "g"(queue) : "memory");
#else /* tron */
	entry->prev = queue->prev;
	entry->next = queue;
	queue->prev->next = entry;
	queue->prev = entry;
#endif /* tron */
}

/*
 *  キューからエントリを削除
 *
 *  entry をキューから削除する．
 */
Inline void
queue_delete(QUEUE *entry)
{
#ifdef tron
	Asm("qdel %a0, r0" : : "g"(entry->prev) : "r0", "memory");
#else /* tron */
	if (entry->next != entry) {
		entry->prev->next = entry->next;
		entry->next->prev = entry->prev;
	}
#endif /* tron */
}

/*
 *  キューの先頭のエントリの取り出し
 *
 *  queue の直後のエントリをキューから削除し，削除したエントリを返す．
 *  queue がキューヘッダを指す場合には，キューの先頭のエントリを取り出
 *  すことになる．
 */
Inline QUEUE *
queue_delete_next(QUEUE *queue)
{
	QUEUE	*entry;

	assert(queue->next != queue);
#ifdef tron
	Asm("qdel %a1, %0" : "r="(entry) : "g"(queue) : "memory");
#else /* tron */
	entry = queue->next;
	queue->next = entry->next;
	entry->next->prev = queue;
#endif /* tron */
	return(entry);
}

/*
 *  キュー中のエントリのサーチ
 *
 *  queue で示すキューから，offset で示すフィールドが val より大きいの
 *  エントリをサーチし，そのエントリを返す．該当するエントリがない場合
 *  は，queue を返す．
 */
Inline QUEUE *
queue_search_gt(QUEUE *queue, INT val, INT offset)
{
#if defined(tron) && !defined(TRON_LEVEL1)
	register QUEUE	*r0 asm("r0") = queue;
	register QUEUE	*r2 asm("r2") = queue;
	register INT	r3 asm("r3") = val + 1;
	register INT	r5 asm("r5") = offset;

	Asm("qsch/ge/f"
	  : "g="(r0)
	  : "0"(r0), "g"(r2), "g"(r3), "g"(r5)
	  : "r1");
	return(r0);
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	QUEUE	*entry;

	for (entry = queue->next; entry != queue; entry = entry->next) {
		if (*((INT *)(((VB *) entry) + offset)) > val) {
			break;
		}
	}
	return(entry);
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
}

/*
 *  キューが空かどうかのチェック
 */
Inline BOOL
queue_empty_p(QUEUE *queue)
{
	if (queue->next == queue) {
		assert(queue->prev == queue);
		return(1);
	}
	return(0);
}

#endif /* _QUEUE_ */
