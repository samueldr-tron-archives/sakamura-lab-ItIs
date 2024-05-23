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
 *  �D��x�p���Z�}�t�H�Ǘ��u���b�N�̒�`
 */

struct prisem_control_block {
	QUEUE	wait_queue;	/* �D��x�p���Z�}�t�H�҂��L���[ */
	ID	pisid;		/* �D��x�p���Z�}�t�HID */
	VP	exinf;		/* �g����� */
	ATR	pisatr;		/* �D��x�p���Z�}�t�H���� */
	TCB	*pistsk;	/* �D��x�p���Z�}�t�H�擾�^�X�N */
#ifdef PRISEM_SPEC1
	PISCB	*pislist;	/* �l���D��x�p���Z�}�t�H���X�g */
#endif /* PRISEM_SPEC1 */
};

static PISCB	piscb_table[NUM_PISID];

#define get_piscb(id)	(&(piscb_table[INDEX_PIS(id)]))

#ifdef PRISEM_SPEC1
#define pis_waited(piscb)	(!queue_empty_p(&(piscb->wait_queue)))
#define	pis_head_pri(piscb)	(((TCB *)(piscb->wait_queue.next))->priority)
#endif /* PRISEM_SPEC1 */

/*
 *  ���g�p�̗D��x�p���Z�}�t�H�Ǘ��u���b�N�̃��X�g
 */
#ifndef _i_vvcre_pis
QUEUE	free_piscb;
#endif /* _i_vvcre_pis */

/* 
 *  �D��x�p���Z�}�t�H�Ǘ��u���b�N�̏�����
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
 *  �D��x�p���Z�}�t�H (�d�l1) �T�|�[�g���[�`��
 */

#ifdef PRISEM_SPEC1

/*
 *  �D��x�̐ݒ菈��
 *
 *  �^�X�N�̗D��x���C���̃^�X�N�̏����D��x�Ɗl�����Ă���D��x�p���Z
 *  �}�t�H��҂��Ă���^�X�N�̗D��x�̓��C�ō��̂��̂ɐݒ肷��D�������C
 *  �ݒ肷�ׂ��D��x�����݂̗D��x�ƈ�v���Ă���ꍇ�ɂ́C�������Ȃ��D
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
 *  �^�X�N���D��x�p���Z�}�t�H����������ꍇ�̏���
 *
 *  �^�X�N���l�����Ă���D��x�p���Z�}�t�H�̃��X�g���X�V���C�K�v�Ȃ� 
 *  (��̓I�ɂ́C�Z�}�t�H���������^�X�N�̗D��x���C�Z�}�t�H�̐擪��
 *  �҂��Ă���^�X�N�̗D��x�Ɠ����ꍇ) �^�X�N�̗D��x��ݒ肵�Ȃ����D
 *  �������C�ݒ肷�ׂ��D��x���C���݂̗D��x�Ɠ����������ꍇ�ɂ́C����
 *  ���Ȃ��D
 */
static void
release_prisem(TCB *tcb, PISCB *relpiscb)
{
	PISCB	*piscb, **last_piscb;

	last_piscb = &tcb->pislist;
	if (pis_waited(relpiscb) && tcb->priority == pis_head_pri(relpiscb)) {
		/*
		 *  �D��x�̐ݒ肪�K�v�ȏꍇ
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
		 *  �D��x�̐ݒ肪�K�v�Ȃ��ꍇ
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
 *  �^�X�N�I�����̗D��x�p���Z�}�t�H�������
 *
 *  �^�X�N���l�����Ă���D��x�p���Z�}�t�H�����ׂĉ������D�^�X�N�͏I
 *  ������̂ŁC�^�X�N�̗D��x��l�����Ă���D��x�p���Z�}�t�H���X�g��
 *  �ێ炷��K�v���Ȃ��D
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
 *  �Z�}�t�H�҂��̃^�X�N�̗D��x���ύX���ꂽ�ꍇ�̏���
 *
 *  �D��x�p���Z�}�t�H�҂��L���[�̏����𒼂��C���̗D��x�ݒ菈�����s���D
 *  1) �D��x���ύX���ꂽ�^�X�N�̕ύX��̗D��x���C�D��x�p���Z�}�t�H
 *     ���l�����Ă���^�X�N���������D��x�����ꍇ�C��҂̃^�X�N��
 *     �O�҂̗D��x�ɐݒ肷��D
 *  2) (�d�l1 �̏ꍇ�̂�) �D��x���ύX���ꂽ�^�X�N�̕ύX�O�̗D��x���C
 *     �D��x�p���Z�}�t�H���l�����Ă���^�X�N�̗D��x�Ɠ����ꍇ�C���
 *     �̃^�X�N�̗D��x���Đݒ肷��D
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
 *  �D��x�p���Z�}�t�H�҂��̃^�X�N���҂��������ꂽ�ꍇ�̏��� (�d�l1)
 *
 *  �D��x�p���Z�}�t�H���l�����Ă���^�X�N�̗D��x���C�҂������^�X�N��
 *  �D��x�Ɠ����ꍇ�ɁC�O�҂̃^�X�N�̗D��x���Đݒ肷��D
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
 *  �D��x�p���Z�}�t�H�҂��d�l�̒�`
 */
#ifdef PRISEM_SPEC1
static WSPEC wspec_pis = { TTW_PIS, pis_chg_pri, pis_rel_wai };
#else /* PRISEM_SPEC1 */
static WSPEC wspec_pis = { TTW_PIS, pis_chg_pri, 0 };
#endif /* PRISEM_SPEC1 */

/*
 *  �D��x�p���Z�}�t�H�Ǘ��@�\
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
		 *  �폜����D��x�p���Z�}�t�H���l�����Ă���^�X�N����
		 *  ��ꍇ�C���̃^�X�N�̗D��x���Đݒ肷��D
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
