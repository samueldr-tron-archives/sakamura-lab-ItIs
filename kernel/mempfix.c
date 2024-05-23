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
 *  @(#) $Id: mempfix.c,v 1.14 1997/01/10 13:36:29 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_MPF

/*
 *  �Œ蒷�������v�[���Ǘ��u���b�N�̒�`
 */

typedef struct free_list {
	struct free_list *next;
} FREEL;

typedef struct fix_memorypool_control_block {
	QUEUE	wait_queue;	/* �������v�[���҂��L���[ */
	ID	mpfid;		/* �Œ蒷�������v�[��ID */
	VP	exinf;		/* �g����� */
	ATR	mpfatr;		/* �������v�[������ */
	INT	mpfcnt;		/* �������v�[���S�̂̃u���b�N�� */
	INT	blfsz;		/* �Œ蒷�������u���b�N�̃T�C�Y */
	INT	mpfsz;		/* �������v�[���S�̂̃T�C�Y */
	INT	frbcnt;		/* �󂫗̈�̃u���b�N�� */
	VP	mempool;	/* �������v�[���̐擪�A�h���X */
	VP	unused;		/* ���g�p�̈�̐擪 */
	FREEL	*freelist;	/* �󂫃u���b�N�̃��X�g */
} MPFCB;

static MPFCB	mpfcb_table[NUM_MPFID];

#define get_mpfcb(id)	(&(mpfcb_table[INDEX_MPF(id)]))

/*
 *  ���g�p�̌Œ蒷�������v�[���Ǘ��u���b�N�̃��X�g
 */
#ifndef _i_vcre_mpf
QUEUE	free_mpfcb;
#endif /* _i_vcre_mpf */

/* 
 *  �Œ蒷�������v�[���Ǘ��u���b�N�̏�����
 */
void
fix_memorypool_initialize(void)
{
	INT	i;
	MPFCB	*mpfcb;
	ID	mpfid;

#ifndef _i_vcre_mpf
	queue_initialize(&free_mpfcb);
#endif /* _i_vcre_mpf */

	for(mpfcb = mpfcb_table, i = 0; i < NUM_MPFID; mpfcb++, i++) {
		mpfid = ID_MPF(i);
		mpfcb->mpfid = mpfid;
		mpfcb->mpfatr = OBJ_NONEXIST;
#ifndef _i_vcre_mpf
		if (!SYS_MPFID(mpfid)) {
			queue_insert(&(mpfcb->wait_queue), &free_mpfcb);
		}
#endif /* _i_vcre_mpf */
	}
}

/*
 *  �Œ蒷�������v�[���Ǘ��p���[�`��
 */

#define MINSIZE		(sizeof(FREEL))
#define	MINSZ(sz)	(((sz) + (MINSIZE-1)) & ~(MINSIZE-1))

Inline VP
mempool_end(MPFCB *mpfcb)
{
	return((VP)((VB *)(mpfcb->mempool) + mpfcb->mpfsz));
}

/*
 *  �Œ蒷�������v�[���҂��d�l�̒�`
 */
static WSPEC wspec_mpf_tfifo = { TTW_MPF, 0, 0 };
static WSPEC wspec_mpf_tpri = { TTW_MPF, obj_chg_pri, 0 };

/*
 *  �Œ蒷�������v�[���Ǘ��@�\
 */

#if !defined(_i_cre_mpf) || !defined(_i_vcre_mpf)

static ER
_cre_mpf(MPFCB *mpfcb, T_CMPF *pk_cmpf, INT blfsz, INT mpfsz)
{
	if ((mpfcb->mempool = sys_get_blk(mpfsz)) == 0) {
		return(E_NOMEM);
	}

#ifndef _i_vcre_mpf
	if (!SYS_MPFID(mpfcb->mpfid)) {
		queue_delete(&(mpfcb->wait_queue));
	}
#endif /* _i_vcre_mpf */

	queue_initialize(&(mpfcb->wait_queue));
	mpfcb->exinf = pk_cmpf->exinf;
	mpfcb->mpfatr = pk_cmpf->mpfatr;
	mpfcb->mpfcnt = mpfcb->frbcnt = pk_cmpf->mpfcnt;
	mpfcb->blfsz = blfsz;
	mpfcb->mpfsz = mpfsz;
	mpfcb->unused = mpfcb->mempool;
	mpfcb->freelist = (FREEL *) 0;
	return(E_OK);
}

#endif /* !defined(_i_cre_mpf) || !defined(_i_vcre_mpf) */
#ifndef _i_cre_mpf

SYSCALL ER
i_cre_mpf(ID mpfid, T_CMPF *pk_cmpf)
{
	MPFCB	*mpfcb;
	INT	blfsz, mpfsz;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_MPFACV(mpfid);
	CHECK_RSATR(pk_cmpf->mpfatr, TA_TPRI);
	CHECK_PAR(pk_cmpf->mpfcnt > 0);
	CHECK_PAR(pk_cmpf->blfsz > 0);
	blfsz = MINSZ(pk_cmpf->blfsz);
	mpfsz = blfsz * pk_cmpf->mpfcnt;
	mpfcb = get_mpfcb(mpfid);

	BEGIN_CRITICAL_SECTION;
	if (mpfcb->mpfid != OBJ_NONEXIST) {
		ercd = E_OBJ;
	}
	else {
		ercd = _cre_mpf(mpfcb, pk_cmpf, blfsz, mpfsz);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_mpf */
#ifndef _i_vcre_mpf

SYSCALL ER
i_vcre_mpf(T_CMPF *pk_cmpf)
{
	MPFCB	*mpfcb;
	INT	blfsz, mpfsz;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cmpf->mpfatr, TA_TPRI);
	CHECK_PAR(pk_cmpf->mpfcnt > 0);
	CHECK_PAR(pk_cmpf->blfsz > 0);
	blfsz = MINSZ(pk_cmpf->blfsz);
	mpfsz = blfsz * pk_cmpf->mpfcnt;

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_mpfcb)) {
		ercd = EV_FULL;
	}
	else {
		mpfcb = (MPFCB *)(free_mpfcb.next);
		ercd = _cre_mpf(mpfcb, pk_cmpf, blfsz, mpfsz);
		if (ercd == E_OK) {
			ercd = (ER)(mpfcb->mpfid);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_mpf */
#ifndef _i_del_mpf

SYSCALL ER
i_del_mpf(ID mpfid)
{
	MPFCB	*mpfcb;
	ER	ercd = E_OK;
    
	CHECK_MPFID(mpfid);
	CHECK_MPFACV(mpfid);
	mpfcb = get_mpfcb(mpfid);

	BEGIN_CRITICAL_SECTION;
	if (mpfcb->mpfid == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		sys_rel_blk(mpfcb->mempool);
		wait_delete(&(mpfcb->wait_queue));
#ifndef _i_vcre_mpf
		if (!SYS_MPFID(mpfcb->mpfid)) {
			queue_insert(&(mpfcb->wait_queue), &free_mpfcb);
		}
#endif /* _i_vcre_mpf */
		mpfcb->mpfid = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_mpf */
#ifndef _i_get_blf

SYSCALL ER
i_get_blf(VP *p_blf, ID mpfid)
{
	return(i_tget_blf(p_blf, mpfid, TMO_FEVR));
}

#endif /* _i_get_blf */
#ifndef _i_pget_blf

SYSCALL ER
i_pget_blf(VP *p_blf, ID mpfid)
{
	return(i_tget_blf(p_blf, mpfid, TMO_POL));
}

#endif /* _i_pget_blf */
#if !defined(_i_get_blf) || !defined(_i_pget_blf) || !defined(_i_tget_blf)

SYSCALL ER
i_tget_blf(VP* p_blf, ID mpfid, TMO tmout)
{
	MPFCB	*mpfcb;
	FREEL	*free;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_MPFACV(mpfid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH_POL(tmout);
	mpfcb = get_mpfcb(mpfid);

	BEGIN_CRITICAL_SECTION;
	if (mpfcb->mpfid == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (mpfcb->freelist) {
		free = mpfcb->freelist;
		mpfcb->freelist = free->next;
		(mpfcb->frbcnt)--;
		*p_blf = (VP) free;
	}
	else if (mpfcb->unused < mempool_end(mpfcb)) {
		*p_blf = mpfcb->unused;
		mpfcb->unused = (VP)((VB *)(mpfcb->unused) + mpfcb->blfsz);
		(mpfcb->frbcnt)--;
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = (mpfcb->mpfatr & TA_TPRI) ?
					&wspec_mpf_tpri : &wspec_mpf_tfifo;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.mpf.p_blf = p_blf;
			gcb_make_wait((GCB *) mpfcb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_tget_blf */
#ifndef _i_rel_blf

SYSCALL ER
i_rel_blf(ID mpfid, VP blf)
{
	MPFCB	*mpfcb;
	TCB	*tcb;
	FREEL	*free;
	ER	ercd = E_OK;
    
	CHECK_MPFID(mpfid);
	CHECK_MPFACV(mpfid);
	mpfcb = get_mpfcb(mpfid);

	BEGIN_CRITICAL_SECTION;
	if (mpfcb->mpfid == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
#ifdef CHK_PAR
	else if (!(mpfcb->mempool <= blf && blf < mempool_end(mpfcb))) {
		ercd = E_PAR;
	}
	else if ((((VB *) blf) - ((VB *)(mpfcb->mempool))) % mpfcb->blfsz) {
		ercd = E_PAR;
	}
#endif /* CHK_PAR */
	else if (!queue_empty_p(&(mpfcb->wait_queue))) {
		tcb = (TCB *)(mpfcb->wait_queue.next);
		*(tcb->winfo.mpf.p_blf) = blf;
		wait_release_ok(tcb);
	}
	else {
		free = (FREEL *) blf;
		free->next = mpfcb->freelist;
		mpfcb->freelist = free;
		(mpfcb->frbcnt)++;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_rel_blf */
#ifndef _i_ref_mpf

SYSCALL ER
i_ref_mpf(T_RMPF *pk_rmpf, ID mpfid)
{
	MPFCB	*mpfcb;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_MPFACV(mpfid);
	mpfcb = get_mpfcb(mpfid);

	BEGIN_CRITICAL_SECTION;
	if (mpfcb->mpfid == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rmpf->exinf = mpfcb->exinf;
		pk_rmpf->wtsk = wait_tskid(&(mpfcb->wait_queue));
		pk_rmpf->frbcnt = mpfcb->frbcnt;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_mpf */
#endif /* USE_MPF */
