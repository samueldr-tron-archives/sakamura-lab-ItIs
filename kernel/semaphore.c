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
 *  @(#) $Id: semaphore.c,v 1.15 1998/01/30 09:52:42 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_SEM

/*
 *  セマフォ管理ブロックの定義
 */

typedef struct semaphore_control_block {
	QUEUE	wait_queue;	/* セマフォ待ちキュー */
	ID	semid;		/* セマフォID */
	VP	exinf;		/* 拡張情報 */
	ATR	sematr;		/* セマフォ属性 */
	INT	semcnt;		/* セマフォ現在カウント値 */
	INT	maxsem;		/* セマフォ最大カウント値 */
} SEMCB;

static SEMCB	semcb_table[NUM_SEMID];

#define get_semcb(id)	(&(semcb_table[INDEX_SEM(id)]))

/*
 *  未使用のセマフォ管理ブロックのリスト
 */
#ifndef _i_vcre_sem
QUEUE	free_semcb;
#endif /* _i_vcre_sem */

/* 
 *  セマフォ管理ブロックの初期化
 */
void
semaphore_initialize(void)
{
	INT	i;
	SEMCB	*semcb;
	ID	semid;

#ifndef _i_vcre_sem
	queue_initialize(&free_semcb);
#endif /* _i_vcre_sem */

	for(semcb = semcb_table, i = 0; i < NUM_SEMID; semcb++, i++) {
		semid = ID_SEM(i);
		semcb->semid = semid;
		semcb->sematr = OBJ_NONEXIST;
#ifndef _i_vcre_sem
		if (!SYS_SEMID(semid)) {
			queue_insert(&(semcb->wait_queue), &free_semcb);
		}
#endif /* _i_vcre_sem */
	}
}

/*
 *  セマフォ待ち仕様の定義
 */
static WSPEC wspec_sem_tfifo = { TTW_SEM, 0, 0 };
static WSPEC wspec_sem_tpri = { TTW_SEM, obj_chg_pri, 0 };

/*
 *  セマフォ管理機能
 */

#if !defined(_i_cre_sem) || !defined(_i_vcre_sem)

static void
_cre_sem(SEMCB *semcb, T_CSEM *pk_csem)
{
#ifndef _i_vcre_sem
	if (!SYS_SEMID(semcb->semid)) {
		queue_delete(&(semcb->wait_queue));
	}
#endif /* _i_vcre_sem */

	queue_initialize(&(semcb->wait_queue));
	semcb->exinf = pk_csem->exinf;
	semcb->sematr = pk_csem->sematr;
	semcb->semcnt = pk_csem->isemcnt;
	semcb->maxsem = pk_csem->maxsem;
}

#endif /* !defined(_i_cre_sem) || !defined(_i_vcre_sem) */
#ifndef _i_cre_sem

SYSCALL ER
i_cre_sem(ID semid, T_CSEM *pk_csem)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);
	CHECK_SEMACV(semid);
	CHECK_RSATR(pk_csem->sematr, TA_TPRI);
	CHECK_PAR(pk_csem->isemcnt >= 0);
	CHECK_PAR(pk_csem->maxsem >= pk_csem->isemcnt);
	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if (semcb->sematr != OBJ_NONEXIST) {
		ercd =  E_OBJ;
	}
	else {
		_cre_sem(semcb, pk_csem);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_sem */
#ifndef _i_vcre_sem

SYSCALL ER
i_vcre_sem(T_CSEM *pk_csem)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_csem->sematr, TA_TPRI);
	CHECK_PAR(pk_csem->isemcnt >= 0);
	CHECK_PAR(pk_csem->maxsem >= pk_csem->isemcnt);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_semcb)) {
		ercd =  EV_FULL;
	}
	else {
		semcb = (SEMCB *)(free_semcb.next);
		_cre_sem(semcb, pk_csem);
		ercd = (ER)(semcb->semid);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_sem */
#ifndef _i_del_sem

SYSCALL ER
i_del_sem(ID semid)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;
    
	CHECK_SEMID(semid);
	CHECK_SEMACV(semid);
	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if (semcb->sematr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		wait_delete(&(semcb->wait_queue));
#ifndef _i_vcre_sem
		if (!SYS_SEMID(semcb->semid)) {
			queue_insert(&(semcb->wait_queue), &free_semcb);
		}
#endif /* _i_vcre_sem */
		semcb->sematr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_sem */
#ifndef _i_sig_sem

SYSCALL ER
i_sig_sem(ID semid)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;
    
	CHECK_SEMID(semid);
	CHECK_SEMACV(semid);
	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if (semcb->sematr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!queue_empty_p(&(semcb->wait_queue))) {
		wait_release_ok((TCB *)(semcb->wait_queue.next));
	}
	else if (semcb->semcnt >= semcb->maxsem) {
		ercd = E_QOVR;
	}
	else {
		semcb->semcnt += 1;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_sig_sem */
#ifndef _i_wai_sem

SYSCALL ER
i_wai_sem(ID semid)
{
	return(i_twai_sem(semid, TMO_FEVR));
}

#endif /* _i_wai_sem */
#ifndef _i_preq_sem

SYSCALL ER
i_preq_sem(ID semid)
{
	return(i_twai_sem(semid, TMO_POL));
}

#endif /* _i_preq_sem */
#if !defined(_i_wai_sem) || !defined(_i_preq_sem) || !defined(_i_twai_sem)

SYSCALL ER
i_twai_sem(ID semid, TMO tmout)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);
	CHECK_SEMACV(semid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if (semcb->sematr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (semcb->semcnt >= 1) {
		semcb->semcnt -= 1;
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = (semcb->sematr & TA_TPRI) ?
					&wspec_sem_tpri : &wspec_sem_tfifo;
			ctxtsk->wercd = &ercd;
			gcb_make_wait((GCB *) semcb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_twai_sem */
#ifndef _i_ref_sem

SYSCALL ER
i_ref_sem(T_RSEM *pk_rsem, ID semid)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);
	CHECK_SEMACV(semid);
	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if (semcb->sematr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rsem->exinf = semcb->exinf;
		pk_rsem->wtsk = wait_tskid(&(semcb->wait_queue));
		pk_rsem->semcnt = semcb->semcnt;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_sem */
#endif /* USE_SEM */
