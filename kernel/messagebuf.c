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
 *  @(#) $Id: messagebuf.c,v 1.17 1997/01/10 13:36:26 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_MBF

/*
 *  ���b�Z�[�W�o�b�t�@�Ǘ��u���b�N�̒�`
 *
 *  1�̃��b�Z�[�W�o�b�t�@�ɑ΂��āC��M�҂� (TTW_MBF) �̃^�X�N�Ƒ��M
 *  �҂� (TTW_SMBF) �̃^�X�N�������ɑ��݂��邱�Ƃ͂Ȃ����߁C�҂��L���[
 *  �����p�ł���\�������邪�C���b�Z�[�W�o�b�t�@�̃T�C�Y�� 0 �̎��ɁC
 *  �҂��L���[���ǂ���̖ړI�Ŏg���Ă��邩���ʂ���̂��ʓ|�ɂȂ邽�߁C
 *  ���̕��@�͍̗p���Ȃ��D
 */

typedef struct messagebuffer_control_block {
	QUEUE	wait_queue;	/* ���b�Z�[�W�o�b�t�@��M�҂��L���[ */
	ID	mbfid;		/* ���b�Z�[�W�o�b�t�@ID */
	VP	exinf;		/* �g����� */
	ATR	mbfatr;		/* ���b�Z�[�W�o�b�t�@���� */
	QUEUE	send_queue;	/* ���b�Z�[�W�o�b�t�@���M�҂��L���[ */
	INT	bufsz;		/* ���b�Z�[�W�o�b�t�@�̃T�C�Y */
	INT	maxmsz;		/* ���b�Z�[�W�̍ő咷 */
	INT	frbufsz;	/* �󂫃o�b�t�@�̃T�C�Y */
	INT	head;		/* �ŏ��̃��b�Z�[�W�̊i�[�ꏊ */
	INT	tail;		/* �Ō�̃��b�Z�[�W�̊i�[�ꏊ�̎� */
	VB	*buffer;	/* ���b�Z�[�W�o�b�t�@�̔Ԓn */
} MBFCB;

static MBFCB	mbfcb_table[NUM_MBFID];

#define get_mbfcb(id)	(&(mbfcb_table[INDEX_MBF(id)]))

/*
 *  ���g�p�̃��b�Z�[�W�o�b�t�@�Ǘ��u���b�N�̃��X�g
 */
#ifndef _i_vcre_mbf
QUEUE	free_mbfcb;
#endif /* _i_vcre_mbf */

/* 
 *  ���b�Z�[�W�o�b�t�@�Ǘ��u���b�N�̏�����
 */
void
messagebuffer_initialize()
{
	INT	i;
	MBFCB	*mbfcb;
	ID	mbfid;

#ifndef _i_vcre_mbf
	queue_initialize(&free_mbfcb);
#endif /* _i_vcre_mbf */

	for(mbfcb = mbfcb_table, i = 0; i < NUM_MBFID; mbfcb++, i++) {
		mbfid = ID_MBF(i);
		mbfcb->mbfid = mbfid;
		mbfcb->mbfatr = OBJ_NONEXIST;
#ifndef _i_vcre_mbf
		if (!SYS_MBFID(mbfid)) {
			queue_insert(&(mbfcb->wait_queue), &free_mbfcb);
		}
#endif /* _i_vcre_mbf */
	}
}

/*
 *  ���b�Z�[�W�o�b�t�@���샋�[�`��
 */

typedef INT		HEADER;
#define HEADERSZ	sizeof(HEADER)

#define ROUNDSIZE	sizeof(HEADER)
#define	ROUNDSZ(sz)	(((sz) + (ROUNDSIZE-1)) & ~(ROUNDSIZE-1))

/*
 *  �T�C�Y�� msgsz ���b�Z�[�W���C���b�Z�[�W�o�b�t�@�ɓ��邩�ǂ����`�F�b
 *  �N����D
 *
 *  �{���� msgsz �ł͂Ȃ� ROUNDSZ(msgsz) �Ƃ��ׂ��ł��邪�CHEADERSZ �� 
 *  mbfcb->frbufsz �� ROUNDSZ ����Ă��邽�߁C�������ƂɂȂ�D
 */
Inline BOOL
mbf_free(MBFCB* mbfcb, INT msgsz)
{
	return(HEADERSZ + msgsz <= mbfcb->frbufsz);
}

/*
 *  ���b�Z�[�W�o�b�t�@���󂩂ǂ����`�F�b�N����D
 */
Inline BOOL
mbf_empty(MBFCB* mbfcb)
{
	return(mbfcb->frbufsz == mbfcb->bufsz);
}

/*
 *  ���b�Z�[�W���C���b�Z�[�W�o�b�t�@�֒ǉ�����D
 */
static void
msg_to_mbf(MBFCB* mbfcb, VP msg, INT msgsz)
{
	INT	tail = mbfcb->tail;
	VB	*buffer = mbfcb->buffer;
	INT	remsz;

	mbfcb->frbufsz -= HEADERSZ + ROUNDSZ(msgsz);
	*((HEADER *) &(buffer[tail])) = msgsz;
	tail += HEADERSZ;
	if (tail >= mbfcb->bufsz) {
		tail = 0;
	}

	if ((remsz = mbfcb->bufsz - tail) < msgsz) {
		memcpy(&(buffer[tail]), msg, remsz);
		((VB *) msg) += remsz;
		msgsz -= remsz;
		tail = 0;
	}
	memcpy(&(buffer[tail]), msg, msgsz);
	tail += ROUNDSZ(msgsz);
	if (tail >= mbfcb->bufsz) {
		tail = 0;
	}
	mbfcb->tail = tail;
}

/*
 *  ���b�Z�[�W�o�b�t�@����C���b�Z�[�W�����o���D
 */
static INT
mbf_to_msg(MBFCB* mbfcb, VP msg)
{
	INT	head = mbfcb->head;
	VB	*buffer = mbfcb->buffer;
	INT	msgsz, copysz;
	INT	remsz;

	msgsz = *((HEADER *) &(buffer[head]));
	head += HEADERSZ;
	if (head >= mbfcb->bufsz) {
		head = 0;
	}
	mbfcb->frbufsz += HEADERSZ + ROUNDSZ(msgsz);

	copysz = msgsz;
	if ((remsz = mbfcb->bufsz - head) < copysz) {
		memcpy(msg, &(buffer[head]), remsz);
		((VB *) msg) += remsz;
		copysz -= remsz;
		head = 0;
	}
	memcpy(msg, &(buffer[head]), copysz);
	head += ROUNDSZ(copysz);
	if (head >= mbfcb->bufsz) {
		head = 0;
	}
	mbfcb->head = head;
	return(msgsz);
}

/*
 *  ���b�Z�[�W�o�b�t�@�҂��d�l�̒�`
 */
static WSPEC wspec_mbf_tfifo = { TTW_MBF, 0, 0 };
static WSPEC wspec_mbf_tpri = { TTW_MBF, obj_chg_pri, 0 };
static WSPEC wspec_smbf = { TTW_SMBF, 0, 0 };

/*
 *  ���b�Z�[�W�o�b�t�@�Ǘ��@�\
 */

#if !defined(_i_cre_mbf) || !defined(_i_vcre_mbf)

static ER
_cre_mbf(MBFCB *mbfcb, T_CMBF *pk_cmbf, INT bufsz)
{
	if (bufsz > 0 && (mbfcb->buffer = sys_get_blk(bufsz)) == 0) {
		return(E_NOMEM);
	}

#ifndef _i_vcre_mbf
	if (!SYS_MBFID(mbfcb->mbfid)) {
		queue_delete(&(mbfcb->wait_queue));
	}
#endif /* _i_vcre_mbf */

	queue_initialize(&mbfcb->wait_queue);
	mbfcb->exinf = pk_cmbf->exinf;
	mbfcb->mbfatr = pk_cmbf->mbfatr;
	queue_initialize(&mbfcb->send_queue);
	mbfcb->bufsz = mbfcb->frbufsz = bufsz;
	mbfcb->maxmsz = pk_cmbf->maxmsz;
	mbfcb->head = mbfcb->tail = 0;
	return(E_OK);
}

#endif /* !defined(_i_cre_mbf) || !defined(_i_vcre_mbf) */
#ifndef _i_cre_mbf

SYSCALL ER
i_cre_mbf(ID mbfid, T_CMBF *pk_cmbf)
{
	MBFCB	*mbfcb;
	INT	bufsz;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_MBFACV(mbfid);
	CHECK_RSATR(pk_cmbf->mbfatr, TA_TPRI);
	CHECK_PAR(pk_cmbf->bufsz >= 0);
	CHECK_PAR(pk_cmbf->maxmsz > 0);
	bufsz = ROUNDSZ(pk_cmbf->bufsz);
	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if (mbfcb->mbfatr != OBJ_NONEXIST) {
		ercd = E_OBJ;
	}
	else {
		ercd = _cre_mbf(mbfcb, pk_cmbf, bufsz);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_mbf */
#ifndef _i_vcre_mbf

SYSCALL ER
i_vcre_mbf(T_CMBF *pk_cmbf)
{
	MBFCB	*mbfcb;
	INT	bufsz;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cmbf->mbfatr, TA_TPRI);
	CHECK_PAR(pk_cmbf->bufsz >= 0);
	CHECK_PAR(pk_cmbf->maxmsz > 0);
	bufsz = ROUNDSZ(pk_cmbf->bufsz);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_mbfcb)) {
		ercd =  EV_FULL;
	}
	else {
		mbfcb = (MBFCB *)(free_mbfcb.next);
		ercd = _cre_mbf(mbfcb, pk_cmbf, bufsz);
		if (ercd == E_OK) {
			ercd = (ER)(mbfcb->mbfid);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_mbf */
#ifndef _i_del_mbf

SYSCALL ER
i_del_mbf(ID mbfid)
{
	MBFCB	*mbfcb;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_MBFACV(mbfid);
	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if (mbfcb->mbfatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		if (mbfcb->bufsz > 0) {
			sys_rel_blk(mbfcb->buffer);
		}
		wait_delete(&(mbfcb->wait_queue));
		wait_delete(&(mbfcb->send_queue));
#ifndef _i_vcre_mbf
		if (!SYS_MBFID(mbfcb->mbfid)) {
			queue_insert(&(mbfcb->wait_queue), &free_mbfcb);
		}
#endif /* _i_vcre_mbf */
		mbfcb->mbfatr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_mbf */
#ifndef _i_snd_mbf

SYSCALL ER
i_snd_mbf(ID mbfid, VP msg, INT msgsz)
{
	return(i_tsnd_mbf(mbfid, msg, msgsz, TMO_FEVR));
}

#endif /* _i_snd_mbf */
#ifndef _i_psnd_mbf

SYSCALL ER
i_psnd_mbf(ID mbfid, VP msg, INT msgsz)
{
	return(i_tsnd_mbf(mbfid, msg, msgsz, TMO_POL));
}

#endif /* _i_psnd_mbf */
#if !defined(_i_snd_mbf) || !defined(_i_psnd_mbf) || !defined(_i_tsnd_mbf)

SYSCALL ER
i_tsnd_mbf(ID mbfid, VP msg, INT msgsz, TMO tmout)
{
	MBFCB	*mbfcb;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_MBFACV(mbfid);
	CHECK_PAR(msgsz > 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH_POL(tmout);
	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if (mbfcb->mbfatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
#ifdef CHK_PAR
	else if (msgsz > mbfcb->maxmsz) {
		ercd = E_PAR;
	}
#endif /* CHK_PAR */
	else if (!queue_empty_p(&(mbfcb->wait_queue))) {
		tcb = (TCB *)(mbfcb->wait_queue.next);
		memcpy(tcb->winfo.mbf.msg, msg, msgsz);
		*(tcb->winfo.mbf.p_msgsz) = msgsz;
		wait_release_ok(tcb);
	}
	else if (mbf_free(mbfcb, msgsz)) {
		msg_to_mbf(mbfcb, msg, msgsz);
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = &wspec_smbf;
			ctxtsk->wgcb = (GCB *) mbfcb;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.smbf.msg = msg;
			ctxtsk->winfo.smbf.msgsz = msgsz;
			make_wait(tmout);
			queue_insert(&(ctxtsk->tskque), &(mbfcb->send_queue));
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_tsnd_mbf */
#ifndef _i_rcv_mbf

SYSCALL ER
i_rcv_mbf(VP msg, INT *p_msgsz, ID mbfid)
{
	return(i_trcv_mbf(msg, p_msgsz, mbfid, TMO_FEVR));
}

#endif /* _i_rcv_mbf */
#ifndef _i_prcv_mbf

SYSCALL ER
i_prcv_mbf(VP msg, INT *p_msgsz, ID mbfid)
{
	return(i_trcv_mbf(msg, p_msgsz, mbfid, TMO_POL));
}

#endif /* _i_prcv_mbf */
#if !defined(_i_rcv_mbf) || !defined(_i_prcv_mbf) || !defined(_i_trcv_mbf)

SYSCALL ER
i_trcv_mbf(VP msg, INT *p_msgsz, ID mbfid, TMO tmout)
{
	MBFCB	*mbfcb;
	QUEUE	*queue;
	TCB	*tcb;
	INT	msgsz;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_MBFACV(mbfid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	mbfcb = get_mbfcb(mbfid);
    
	BEGIN_CRITICAL_SECTION;
	if (mbfcb->mbfatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!mbf_empty(mbfcb)) {
		*p_msgsz = mbf_to_msg(mbfcb, msg);
		queue = mbfcb->send_queue.next;
		while (queue != &(mbfcb->send_queue)) {
			tcb = (TCB *) queue;
			queue = queue->next;
			msgsz = tcb->winfo.smbf.msgsz;
			if (mbf_free(mbfcb, msgsz)) {
				msg_to_mbf(mbfcb, tcb->winfo.smbf.msg, msgsz);
				wait_release_ok(tcb);
			}
		}
	}
	else if (!queue_empty_p(&(mbfcb->send_queue))) {
		tcb = (TCB *)(mbfcb->send_queue.next);
		*p_msgsz = tcb->winfo.smbf.msgsz;
		memcpy(msg, tcb->winfo.smbf.msg, *p_msgsz);
		wait_release_ok(tcb);
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = (mbfcb->mbfatr & TA_TPRI) ?
					&wspec_mbf_tpri : &wspec_mbf_tfifo;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.mbf.msg = msg;
			ctxtsk->winfo.mbf.p_msgsz = p_msgsz;
			gcb_make_wait((GCB *) mbfcb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_trcv_mbf */
#ifndef _i_ref_mbf

SYSCALL ER
i_ref_mbf(T_RMBF *pk_rmbf, ID mbfid)
{
	MBFCB	*mbfcb;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_MBFACV(mbfid);
	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if (mbfcb->mbfatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rmbf->exinf = mbfcb->exinf;
		pk_rmbf->wtsk = wait_tskid(&(mbfcb->wait_queue));
		pk_rmbf->stsk = wait_tskid(&(mbfcb->send_queue));
		pk_rmbf->msgsz = mbf_empty(mbfcb) ? FALSE :
				*((HEADER *) &(mbfcb->buffer[mbfcb->head]));
		pk_rmbf->frbufsz = mbfcb->frbufsz;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_mbf */

/*
 *  �V�X�e�����O�p���b�Z�[�W�o�b�t�@�ւ̑��M
 */

#ifdef USE_TMBF_OS

#define log_mbfcb	get_mbfcb(TMBF_OS)

ER
log_snd_mbf(VP msg, INT msgsz)
{
	TCB	*tcb;
	ER	ercd = E_OK;

	BEGIN_CRITICAL_SECTION;
	if (log_mbfcb->mbfatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!queue_empty_p(&(log_mbfcb->wait_queue))) {
		tcb = (TCB *)(log_mbfcb->wait_queue.next);
		memcpy(tcb->winfo.mbf.msg, msg, msgsz);
		*(tcb->winfo.mbf.p_msgsz) = msgsz;
		wait_release_ok(tcb);
	}
	else if (mbf_free(log_mbfcb, msgsz)) {
		msg_to_mbf(log_mbfcb, msg, msgsz);
	}
	else {
		ercd = E_TMOUT;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* USE_TMBF_OS */
#endif /* USE_MBF */
