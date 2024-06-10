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
 *  @(#) $Id: mempool.c,v 1.15 1998/01/30 09:52:34 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_MPL

/*
 *  可変長メモリプール管理ブロックの定義
 */

typedef QUEUE	AREAQ;		/* エリアキュー */
typedef QUEUE	FREEQ;		/* フリーブロックキュー */

typedef struct memorypool_control_block {
	QUEUE	wait_queue;	/* メモリプール待ちキュー */
	ID	mplid;		/* 可変長メモリプールID */
	VP	exinf;		/* 拡張情報 */
	ATR	mplatr;		/* メモリプール属性 */
	INT	mplsz;		/* メモリプール全体のサイズ */
	VP	mempool;	/* メモリプールの先頭アドレス */
	FREEQ	freequeue;	/* 空きブロックのキュー */
} MPLCB;

static MPLCB	mplcb_table[NUM_MPLID];

#define get_mplcb(id)	(&(mplcb_table[INDEX_MPL(id)]))

/*
 *  未使用の可変長メモリプール管理ブロックのリスト
 */
#ifndef _i_vcre_mpl
QUEUE	free_mplcb;
#endif /* _i_vcre_mpl */

/* 
 *  可変長メモリプール管理ブロックの初期化
 */
void
memorypool_initialize(void)
{
	INT	i;
	MPLCB	*mplcb;
	ID	mplid;

#ifndef _i_vcre_mpl
	queue_initialize(&free_mplcb);
#endif /* _i_vcre_mpl */

	for(mplcb = mplcb_table, i = 0; i < NUM_MPLID; mplcb++, i++) {
		mplid = ID_MPL(i);
		mplcb->mplid = mplid;
		mplcb->mplatr = OBJ_NONEXIST;
#ifndef _i_vcre_mpl
		if (!SYS_MPLID(mplid)) {
			queue_insert(&(mplcb->wait_queue), &free_mplcb);
		}
#endif /* _i_vcre_mpl */
	}
}

/*
 *  可変長メモリプール管理用ルーチン
 */

#define ROUNDSIZE	(sizeof(FREEQ))
#define	ROUNDSZ(sz)	(((sz) + (ROUNDSIZE-1)) & ~(ROUNDSIZE-1))

#define MINSIZE		(sizeof(AREAQ) + sizeof(FREEQ))
#define	MINSZ(sz)	(((sz) + (MINSIZE-1)) & ~(MINSIZE-1))

#define	_MARK(x)	((__typeof__(x)) ((UINT)(x) | 1))
#define	_UNMARK(x)	((__typeof__(x)) ((UINT)(x) & ~1))
#define	_MARKED(x)	((UINT)(x) & 1)

#define _ASSIGN(x, y)	((x) = (_MARKED(x) ? _MARK(y) : _UNMARK(y)))

#define	FIRST_AREA(area)	(_UNMARK((area)->prev) == (AREAQ *) 0)
#define	LAST_AREA(area)		(area->next == (AREAQ *) 0)

#define	USED_AREA(area)		(_MARKED((area)->prev))
#define	USE_AREA(area)		(((UW)(area)->prev) |= 1)
#define	RELEASE_AREA(area)	(((UW)(area)->prev) &= ~1)

#define	FREEQUEUE(area)		((FREEQ *)((area) + 1))
#define	AREAQUEUE(free)		(((AREAQ *)(free)) - 1)

#define	BLOCKTOP(area)		((VP)((area) + 1))
#define	AREATOP(block)		(((AREAQ *)(block)) - 1)

#define	BLOCKSIZE(area)		((VB *)((area)->next) - (VB *) BLOCKTOP(area))

Inline VP
mempool_end(MPLCB *mplcb)
{
	return((VP)(((VB *)(mplcb->mempool)) + mplcb->mplsz));
}

/*
 *  メモリプールの初期化ルーチン
 */
static void
init_mpl(VP mempool, INT mplsz)
{
	AREAQ	*area, *lastarea;

	area = (AREAQ *) mempool;
	lastarea = (AREAQ *)(((VB *) mempool) + mplsz);
	area->prev = (AREAQ *) 0;
	area->next = lastarea;
	lastarea->prev = area;
	lastarea->next = (AREAQ *) 0;
	USE_AREA(lastarea);
}

/*
 *  メモリブロックの獲得ルーチン
 */
static VP
_get_blk(FREEQ *freequeue, INT blksz)
{
	AREAQ	*area, *new;
	FREEQ	*free;
	INT	remsz;

	for (free = freequeue->next; free != freequeue; free = free->next) {
		area = AREAQUEUE(free);
		if ((remsz = BLOCKSIZE(area) - blksz) >= 0) {
			if (remsz >= MINSIZE) {
				/*
				 *  エリアを 2つに分割する
				 */
				new = (AREAQ *)(((VB *) area) + remsz);
				new->prev = area;
				new->next = area->next;
				_ASSIGN(area->next->prev, new);
				area->next = new;
			}
			else {
				queue_delete(free);
				new = area;
			}
			USE_AREA(new);
			return(BLOCKTOP(new));
		}
	}
	return((VP) 0);
}

/*
 *  メモリブロックの解放ルーチン
 */
ER
_rel_blk(FREEQ *freequeue, VP blk)
{
	AREAQ	*area;
	FREEQ	*free;
	AREAQ	*prevarea, *nextarea;

	area = AREATOP(blk);
	free = (FREEQ *) blk;
#ifdef CHK_PAR
	if (!USED_AREA(area)) {
		return(E_PAR);
	}
#endif /* CHK_PAR */
	RELEASE_AREA(area);

	if (FIRST_AREA(area) || USED_AREA(prevarea = area->prev)) {
		if (USED_AREA(nextarea = area->next)) {
			/*
			 *  マージの必要なし．
			 */
			queue_insert(free, freequeue->next);
		}
		else {
			/*
			 *  直後のエリアとマージする．
			 */
			area->next = nextarea->next;
			_ASSIGN(nextarea->next->prev, area);
			queue_delete(FREEQUEUE(nextarea));
			queue_insert(free, freequeue->next);
		}
	}
	else {
		if (USED_AREA(nextarea = area->next)) {
			/*
			 *  直前のエリアとマージする．
			 */
			prevarea->next = nextarea;
			_ASSIGN(nextarea->prev, prevarea);
		}
		else {
			/*
			 *  前後のエリアとマージする．
			 */
			prevarea->next = nextarea->next;
			_ASSIGN(nextarea->next->prev, prevarea);
			queue_delete(FREEQUEUE(nextarea));
		}
	}
	return(E_OK);
}

/*
 *  可変長メモリプール待ちのタスクをメモリがある限り起こす
 */
static void
wakeup_mpl(MPLCB *mplcb)
{
	QUEUE	*queue;
	TCB	*tcb;
	VP	blk;
	INT	blksz, maxsz;

	queue = mplcb->wait_queue.next;
	maxsz = mplcb->mplsz;
	while (queue != &(mplcb->wait_queue)) {
		tcb = (TCB *) queue;
		queue = queue->next;
		blksz = tcb->winfo.mpl.blksz;
		if (blksz <= maxsz) {
			if (blk = _get_blk(&(mplcb->freequeue), blksz)) {
				*(tcb->winfo.mpl.p_blk) = blk;
				wait_release_ok(tcb);
			}
			else if (blksz < maxsz) {
				maxsz = blksz;
			}
		}
	}
}

/*
 *  可変長メモリプール待ち仕様の定義
 */
static WSPEC wspec_mpl_tfifo = { TTW_MPL, 0, 0 };
static WSPEC wspec_mpl_tpri = { TTW_MPL, obj_chg_pri, 0 };

/*
 *  可変長メモリプール管理機能
 */

#if !defined(_i_cre_mpl) || !defined(_i_vcre_mpl)

static ER
_cre_mpl(MPLCB *mplcb, T_CMPL *pk_cmpl, INT mplsz)
{
	VP	mempool;

	if ((mempool = sys_get_blk(mplsz + sizeof(AREAQ))) == 0) {
		return(E_NOMEM);
	}

#ifndef _i_vcre_mpl
	if (!SYS_MPLID(mplcb->mplid)) {
		queue_delete(&(mplcb->wait_queue));
	}
#endif /* _i_vcre_mpl */

	queue_initialize(&(mplcb->wait_queue));
	mplcb->exinf = pk_cmpl->exinf;
	mplcb->mplatr = pk_cmpl->mplatr;
	mplcb->mplsz = mplsz;
	mplcb->mempool = mempool;
	init_mpl(mempool, mplsz);

	queue_initialize(&(mplcb->freequeue));
	queue_insert(FREEQUEUE((AREAQ *) mempool), &(mplcb->freequeue));
	return(E_OK);
}

#endif /* !defined(_i_cre_mpl) || !defined(_i_vcre_mpl) */
#ifndef _i_cre_mpl

SYSCALL ER
i_cre_mpl(ID mplid, T_CMPL *pk_cmpl)
{
	MPLCB	*mplcb;
	INT	mplsz;
	ER	ercd = E_OK;

	CHECK_MPLID_OS(mplid);
	CHECK_MPLACV(mplid);
	CHECK_RSATR(pk_cmpl->mplatr, TA_FFIT|TA_TPRI);
	CHECK_PAR(pk_cmpl->mplsz > 0);
	mplsz = MINSZ(pk_cmpl->mplsz);
	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if (mplcb->mplatr != OBJ_NONEXIST) {
		ercd = E_OBJ;
	}
	else {
		ercd = _cre_mpl(mplcb, pk_cmpl, mplsz);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_mpl */
#ifndef _i_vcre_mpl

SYSCALL ER
i_vcre_mpl(T_CMPL *pk_cmpl)
{
	MPLCB	*mplcb;
	INT	mplsz;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cmpl->mplatr, TA_FFIT|TA_TPRI);
	CHECK_PAR(pk_cmpl->mplsz > 0);
	mplsz = MINSZ(pk_cmpl->mplsz);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_mplcb)) {
		ercd = EV_FULL;
	}
	else {
		mplcb = (MPLCB *)(free_mplcb.next);
		ercd = _cre_mpl(mplcb, pk_cmpl, mplsz);
		if (ercd == E_OK) {
			ercd = (ER)(mplcb->mplid);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_mpl */
#ifndef _i_del_mpl

SYSCALL ER
i_del_mpl(ID mplid)
{
	MPLCB	*mplcb;
	ER	ercd = E_OK;
    
	CHECK_MPLID_OS(mplid);
	CHECK_MPLACV(mplid);
	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if (mplcb->mplatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		sys_rel_blk(mplcb->mempool);
		wait_delete(&(mplcb->wait_queue));
#ifndef _i_vcre_mpl
		if (!SYS_MPLID(mplcb->mplid)) {
			queue_insert(&(mplcb->wait_queue), &free_mplcb);
		}
#endif /* _i_vcre_mpl */
		mplcb->mplatr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_mpl */
#ifndef _i_get_blk

SYSCALL ER
i_get_blk(VP *p_blk, ID mplid, INT blksz)
{
	return(i_tget_blk(p_blk, mplid, blksz, TMO_FEVR));
}

#endif /* _i_get_blk */
#ifndef _i_pget_blk

SYSCALL ER
i_pget_blk(VP *p_blk, ID mplid, INT blksz)
{
	return(i_tget_blk(p_blk, mplid, blksz, TMO_POL));
}

#endif /* _i_pget_blk */
#if !defined(_i_get_blk) || !defined(_i_pget_blk) || !defined(_i_tget_blk)

SYSCALL ER
i_tget_blk(VP* p_blk, ID mplid, INT blksz, TMO tmout)
{
	MPLCB	*mplcb;
	VP	blk;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);
	CHECK_MPLACV(mplid);
	CHECK_PAR(blksz > 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH_POL(tmout);
	mplcb = get_mplcb(mplid);
	blksz = ROUNDSZ(blksz);

	BEGIN_CRITICAL_SECTION;
	if (mplcb->mplatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (blk = _get_blk(&(mplcb->freequeue), blksz)) {
		*p_blk = blk;
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = (mplcb->mplatr & TA_TPRI) ?
					&wspec_mpl_tpri : &wspec_mpl_tfifo;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.mpl.blksz = blksz;
			ctxtsk->winfo.mpl.p_blk = p_blk;
			gcb_make_wait((GCB *) mplcb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_tget_blk */
#ifndef _i_rel_blk

SYSCALL ER
i_rel_blk(ID mplid, VP blk)
{
	MPLCB	*mplcb;
	ER	ercd = E_OK;
    
	CHECK_MPLID(mplid);
	CHECK_MPLACV(mplid);
	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if (mplcb->mplatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
#ifdef CHK_PAR
	else if (!(mplcb->mempool <= blk && blk < mempool_end(mplcb))) {
		ercd = E_PAR;
	}
#endif /* CHK_PAR */
	else if ((ercd = _rel_blk(&(mplcb->freequeue), blk)) == E_OK) {
		wakeup_mpl(mplcb);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_rel_blk */
#ifndef _i_ref_mpl

SYSCALL ER
i_ref_mpl(T_RMPL *pk_rmpl, ID mplid)
{
	MPLCB	*mplcb;
	AREAQ	*area;
	INT	frsz, maxsz, blksz;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);
	CHECK_MPLACV(mplid);
	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if (mplcb->mplatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rmpl->exinf = mplcb->exinf;
		pk_rmpl->wtsk = wait_tskid(&(mplcb->wait_queue));
		frsz = maxsz = 0;
		for (area = (AREAQ *)(mplcb->mempool); !LAST_AREA(area);
							area = area->next) {
			if (!USED_AREA(area)) {
				blksz = BLOCKSIZE(area);
				frsz += blksz;
				if (blksz > maxsz) {
					maxsz = blksz;
				}
			}
		}
		pk_rmpl->frsz = frsz;
		pk_rmpl->maxsz = maxsz;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_mpl */

/*
 *  システムメモリプール管理ルーチン
 *
 *  以下の関数は，必ずクリティカルセクションの中から呼ぶこと．
 */

#ifdef USE_TMPL_OS

#define	sys_mplcb	get_mplcb(TMPL_OS)
#define sys_mplsz	MINSZ(SYS_MPL_SIZE)

ER
sys_cre_mpl(void)
{
	VP	mempool;

	if ((mempool = (VP)(sbrk(sys_mplsz + sizeof(AREAQ)))) == NADR) {
		return(E_NOMEM);
	}

	queue_initialize(&(sys_mplcb->wait_queue));
	sys_mplcb->mplid = TMPL_OS;
	sys_mplcb->exinf = 0;
	sys_mplcb->mplatr = TA_FFIT;
	sys_mplcb->mplsz = sys_mplsz;
	sys_mplcb->mempool = mempool;
	init_mpl(mempool, sys_mplsz);

	queue_initialize(&(sys_mplcb->freequeue));
	queue_insert(FREEQUEUE((AREAQ *) mempool), &(sys_mplcb->freequeue));
	return(E_OK);
}

VP
sys_get_blk(INT blksz)
{
	assert(sys_mplcb->mplid == TMPL_OS);
	assert(blksz > 0);
	return(_get_blk(&(sys_mplcb->freequeue), ROUNDSZ(blksz)));
}

void
sys_rel_blk(VP blk)
{
	ER	ercd;

	assert(sys_mplcb->mplid == TMPL_OS);
	assert(sys_mplcb->mempool <= blk && blk < mempool_end(sys_mplcb));
	ercd = _rel_blk(&(sys_mplcb->freequeue), blk);
	assert(ercd == E_OK);
	wakeup_mpl(sys_mplcb);
}

#endif /* USE_TMPL_OS */
#endif /* USE_MPL */
