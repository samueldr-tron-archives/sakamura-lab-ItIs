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
 *  @(#) $Id: prisem.c,v 1.12 1997/01/10 13:36:28 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_PIS

/*
 *  優先度継承セマフォ管理ブロックの定義
 */

struct prisem_control_block {
	QUEUE	wait_queue;	/* 優先度継承セマフォ待ちキュー */
	ID	pisid;		/* 優先度継承セマフォID */
	VP	exinf;		/* 拡張情報 */
	ATR	pisatr;		/* 優先度継承セマフォ属性 */
	TCB	*pistsk;	/* 優先度継承セマフォ取得タスク */
#ifdef PRISEM_SPEC1
	PISCB	*pislist;	/* 獲得優先度継承セマフォリスト */
#endif /* PRISEM_SPEC1 */
};

static PISCB	piscb_table[NUM_PISID];

#define get_piscb(id)	(&(piscb_table[INDEX_PIS(id)]))

#ifdef PRISEM_SPEC1
#define pis_waited(piscb)	(!queue_empty_p(&(piscb->wait_queue)))
#define	pis_head_pri(piscb)	(((TCB *)(piscb->wait_queue.next))->priority)
#endif /* PRISEM_SPEC1 */

/*
 *  未使用の優先度継承セマフォ管理ブロックのリスト
 */
#ifndef _i_vvcre_pis
QUEUE	free_piscb;
#endif /* _i_vvcre_pis */

/* 
 *  優先度継承セマフォ管理ブロックの初期化
 */
void
prisem_initialize(void)
{
	INT	i;
	PISCB	*piscb;
	ID	pisid;

#ifndef _i_vvcre_pis
	queue_initialize(&free_piscb);
#endif /* _i_vvcre_pis */

	for(piscb = piscb_table, i = 0; i < NUM_PISID; piscb++, i++) {
		pisid = ID_PIS(i);
		piscb->pisid = pisid;
		piscb->pisatr = OBJ_NONEXIST;
#ifndef _i_vvcre_pis
		if (!SYS_PISID(pisid)) {
			queue_insert(&(piscb->wait_queue), &free_piscb);
		}
#endif /* _i_vvcre_pis */
	}
}

/*
 *  優先度継承セマフォ (仕様1) サポートルーチン
 */

#ifdef PRISEM_SPEC1

/*
 *  優先度の設定処理
 *
 *  タスクの優先度を，そのタスクの初期優先度と獲得している優先度継承セ
 *  マフォを待っているタスクの優先度の内，最高のものに設定する．ただし，
 *  設定すべき優先度が現在の優先度と一致している場合には，何もしない．
 */
reset_priority(TCB *tcb)
{
	INT	newpri = tcb->ipriority;
	PISCB	*piscb;

	for (piscb = tcb->pislist; piscb; piscb = piscb->pislist) {
		if (pis_waited(piscb) && newpri > pis_head_pri(piscb)) {
			newpri = pis_head_pri(piscb);
		}
	}
	if (newpri != tcb->priority) {
		change_task_priority(tcb, newpri);
	}
}

/*
 *  タスクが優先度継承セマフォを解放した場合の処理
 *
 *  タスクが獲得している優先度継承セマフォのリストを更新し，必要なら 
 *  (具体的には，セマフォを解放するタスクの優先度が，セマフォの先頭で
 *  待っているタスクの優先度と同じ場合) タスクの優先度を設定しなおす．
 *  ただし，設定すべき優先度が，現在の優先度と同じか高い場合には，何も
 *  しない．
 */
static void
release_prisem(TCB *tcb, PISCB *relpiscb)
{
	PISCB	*piscb, **last_piscb;

	last_piscb = &tcb->pislist;
	if (pis_waited(relpiscb) && tcb->priority == pis_head_pri(relpiscb)) {
		/*
		 *  優先度の設定が必要な場合
		 */
		INT	newpri = tcb->ipriority;

		while (piscb = *last_piscb) {
			if (piscb == relpiscb) {
				*last_piscb = piscb->pislist;
				break;
			}
			if (pis_waited(piscb)
					&& newpri > pis_head_pri(piscb)) {
				newpri = pis_head_pri(piscb);
			}
			last_piscb = &piscb->pislist;
		}
		assert(piscb);

		while (piscb = piscb->pislist) {
			if (pis_waited(piscb)
					&& newpri > pis_head_pri(piscb)) {
				newpri = pis_head_pri(piscb);
			}
		}
		if (tcb->priority < newpri) {
			change_task_priority(tcb, newpri);
		}
	}
	else {
		/*
		 *  優先度の設定が必要ない場合
		 */
		while (piscb = *last_piscb) {
			if (piscb == relpiscb) {
				*last_piscb = piscb->pislist;
				break;
			}
			last_piscb = &piscb->pislist;
		}
		assert(piscb);
	}
}

/*
 *  タスク終了時の優先度継承セマフォ解放処理
 *
 *  タスクが獲得している優先度継承セマフォをすべて解放する．タスクは終
 *  了するので，タスクの優先度や獲得している優先度継承セマフォリストは
 *  保守する必要がない．
 */
void
signal_all_prisem(TCB *tcb)
{
	PISCB	*piscb, *next_piscb;
	TCB	*next_tcb;

	next_piscb = tcb->pislist;
	while (piscb = next_piscb) {
		next_piscb = piscb->pislist;
		if (!queue_empty_p(&(piscb->wait_queue))) {
			next_tcb = (TCB *)(piscb->wait_queue.next);
			wait_release_ok(next_tcb);
			piscb->pistsk = next_tcb;
			piscb->pislist = next_tcb->pislist;
			next_tcb->pislist = piscb;
		}
		else {
			piscb->pistsk = (TCB *) 0;
		}
	}
}

#endif /* PRISEM_SPEC1 */

/*
 *  セマフォ待ちのタスクの優先度が変更された場合の処理
 *
 *  優先度継承セマフォ待ちキューの順序を直し，次の優先度設定処理を行う．
 *  1) 優先度が変更されたタスクの変更後の優先度が，優先度継承セマフォ
 *     を獲得しているタスクよりも高い優先度を持つ場合，後者のタスクを
 *     前者の優先度に設定する．
 *  2) (仕様1 の場合のみ) 優先度が変更されたタスクの変更前の優先度が，
 *     優先度継承セマフォを獲得しているタスクの優先度と同じ場合，後者
 *     のタスクの優先度を再設定する．
 */
void
pis_chg_pri(TCB *tcb, INT oldpri)
{
	PISCB	*piscb;
	TCB	*pistsk;

	piscb = (PISCB *) tcb->wgcb;
	gcb_change_priority((GCB *) piscb, tcb);

	pistsk = piscb->pistsk;
	assert(pistsk);
	if (pistsk->priority > tcb->priority) {
		change_task_priority(pistsk, tcb->priority);
	}
#ifdef PRISEM_SPEC1
	else if (pistsk->priority == oldpri) {
		reset_priority(pistsk);
	}
#endif /* PRISEM_SPEC1 */
}

/*
 *  優先度継承セマフォ待ちのタスクが待ち解除された場合の処理 (仕様1)
 *
 *  優先度継承セマフォを獲得しているタスクの優先度が，待ち解除タスクの
 *  優先度と同じ場合に，前者のタスクの優先度を再設定する．
 */

#ifdef PRISEM_SPEC1

void
pis_rel_wai(TCB *tcb)
{
	PISCB	*piscb;
	TCB	*pistsk;

	piscb = (PISCB *) tcb->wgcb;
	pistsk = piscb->pistsk;
	assert(pistsk);
	if (pistsk->priority == tcb->priority) {
		reset_priority(pistsk);
	}
}

#endif /* PRISEM_SPEC1 */

/*
 *  優先度継承セマフォ待ち仕様の定義
 */
#ifdef PRISEM_SPEC1
static WSPEC wspec_pis = { TTW_PIS, pis_chg_pri, pis_rel_wai };
#else /* PRISEM_SPEC1 */
static WSPEC wspec_pis = { TTW_PIS, pis_chg_pri, 0 };
#endif /* PRISEM_SPEC1 */

/*
 *  優先度継承セマフォ管理機能
 */

#if !defined(_i_vcre_pis) || !defined(_i_vvcre_pis)

static void
_vcre_pis(PISCB *piscb, T_CPIS *pk_cpis)
{
#ifndef _i_vvcre_pis
	if (!SYS_PISID(piscb->pisid)) {
		queue_delete(&(piscb->wait_queue));
	}
#endif /* _i_vvcre_pis */

	queue_initialize(&(piscb->wait_queue));
	piscb->exinf = pk_cpis->exinf;
	piscb->pisatr = (pk_cpis->pisatr | TA_TPRI);
	piscb->pistsk = (TCB *) 0;
}

#endif /* !defined(_i_vcre_pis) || !defined(_i_vvcre_pis) */
#ifndef _i_vcre_pis

SYSCALL ER
i_vcre_pis(ID pisid, T_CPIS *pk_cpis)
{
	PISCB	*piscb;
	ER	ercd = E_OK;

	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	CHECK_RSATR(pk_cpis->pisatr, TA_TPRI);
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr != OBJ_NONEXIST) {
		ercd =  E_OBJ;
	}
	else {
		_vcre_pis(piscb, pk_cpis);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_pis */
#ifndef _i_vvcre_pis

SYSCALL ER
i_vvcre_pis(T_CPIS *pk_cpis)
{
	PISCB	*piscb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cpis->pisatr, TA_TPRI);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_piscb)) {
		ercd =  EV_FULL;
	}
	else {
		piscb = (PISCB *)(free_piscb.next);
		_vcre_pis(piscb, pk_cpis);
		ercd = (ER)(piscb->pisid);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vvcre_pis */
#ifndef _i_vdel_pis

SYSCALL ER
i_vdel_pis(ID pisid)
{
	PISCB	*piscb;
	ER	ercd = E_OK;
    
	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
#ifdef PRISEM_SPEC1
		/*
		 *  削除する優先度継承セマフォを獲得しているタスクがあ
		 *  る場合，そのタスクの優先度を再設定する．
		 */
		if (piscb->pistsk) {
			release_prisem(piscb->pistsk, piscb);
		}
#endif /* PRISEM_SPEC1 */
		wait_delete(&(piscb->wait_queue));
#ifndef _i_vvcre_pis
		if (!SYS_PISID(piscb->pisid)) {
			queue_insert(&(piscb->wait_queue), &free_piscb);
		}
#endif /* _i_vvcre_pis */
		piscb->pisatr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vdel_pis */
#ifndef _i_vsig_pis

SYSCALL ER
i_vsig_pis(ID pisid)
{
	PISCB	*piscb;	
	TCB	*tcb;
	ER	ercd = E_OK;
    
	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	CHECK_INTSK();
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if (piscb->pistsk != ctxtsk) {
		ercd = E_OBJ;
		goto error_exit;
	}

#ifdef PRISEM_SPEC1
	release_prisem(ctxtsk, piscb);
#else /* PRISEM_SPEC1 */
	if (ctxtsk->priority < ctxtsk->ipriority) {
		change_task_priority(ctxtsk, ctxtsk->ipriority);
	}
#endif /* PRISEM_SPEC1 */
	if (!queue_empty_p(&(piscb->wait_queue))) {
		tcb = (TCB *)(piscb->wait_queue.next);
		wait_release_ok(tcb);
		piscb->pistsk = tcb;
#ifdef PRISEM_SPEC1
		piscb->pislist = tcb->pislist;
		tcb->pislist = piscb;
#endif /* PRISEM_SPEC1 */
	}
	else {
		piscb->pistsk = (TCB *) 0;
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vsig_pis */
#ifndef _i_vwai_pis

SYSCALL ER
i_vwai_pis(ID pisid)
{
	return(i_vtwai_pis(pisid, TMO_FEVR));
}

#endif /* _i_vwai_pis */
#ifndef _i_vpreq_pis

SYSCALL ER
i_vpreq_pis(ID pisid)
{
	return(i_vtwai_pis(pisid, TMO_POL));
}

#endif /* _i_vpreq_pis */
#if !defined(_i_vwai_pis) || !defined(_i_vpreq_pis) || !defined(_i_vtwai_pis)

SYSCALL ER
i_vtwai_pis(ID pisid, TMO tmout)
{
	PISCB	*piscb;
	TCB	*pistsk;
	ER	ercd = E_OK;

	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!(pistsk = piscb->pistsk)) {
		piscb->pistsk = ctxtsk;
#ifdef PRISEM_SPEC1
		piscb->pislist = ctxtsk->pislist;
		ctxtsk->pislist = piscb;
#endif /* PRISEM_SPEC1 */
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			if (pistsk->priority > ctxtsk->priority) {
				change_task_priority(pistsk, ctxtsk->priority);
			}
			ctxtsk->wspec = &wspec_pis;
			ctxtsk->wercd = &ercd;
			gcb_make_wait((GCB *) piscb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vtwai_pis */
#ifndef _i_vref_pis

SYSCALL ER
i_vref_pis(T_RPIS *pk_rpis, ID pisid)
{
	PISCB	*piscb;
	ER	ercd = E_OK;

	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rpis->exinf = piscb->exinf;
		pk_rpis->wtsk = wait_tskid(&(piscb->wait_queue));
		pk_rpis->pistsk = piscb->pistsk ? piscb->pistsk->tskid
						: (ID) FALSE;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vref_pis */
#endif /* USE_PIS */
