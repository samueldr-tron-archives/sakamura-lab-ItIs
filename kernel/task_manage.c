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
 *  @(#) $Id: task_manage.c,v 1.31 1998/01/30 06:55:15 hiro Exp $
 */

/*
 *  �^�X�N�Ǘ��@�\
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"
#include "cpu_task.h"

/*
 *  �X�^�b�N�G���A�̎擾/�ԋp
 *
 *  �X�^�b�N�G���A���擾/�ԋp����ۂɓ���ȑ��삪�K�v�ȏꍇ�ɂ́C�V�X
 *  �e���ˑ����� USE_MPROTECT_STACK ����� sys_get_stack/sys_rel_stack
 *  ���`���Csys_get_blk/sys_rel_blk �ɑウ�ėp����D
 */
#ifdef USE_MPROTECT_STACK
extern VP	sys_get_stack(INT size);
extern void	sys_rel_stack(VP blk);
#else /* USE_MPROTECT_STACK */
#define sys_get_stack	sys_get_blk
#define sys_rel_stack	sys_rel_blk
#endif /* USE_MPROTECT_STACK */

/*
 *  �^�X�N�̐����ƍ폜
 */

#if !defined(_i_cre_tsk) || !defined(_i_vcre_tsk)

static ER
#ifdef USE_SEPARATE_SSTACK
_cre_tsk(TCB *tcb, T_CTSK *pk_ctsk, INT stksz, INT sstksz, INT sysmode)
{
	VP	stack, sstack;
#else /* USE_SEPARATE_SSTACK */
_cre_tsk(TCB *tcb, T_CTSK *pk_ctsk, INT sstksz, INT sysmode)
{
	VP	sstack;
#endif /* USE_SEPARATE_SSTACK */

	if ((sstack = sys_get_stack(sstksz)) == 0) {
		return(E_NOMEM);
	}
#ifdef USE_SEPARATE_SSTACK
	if (stksz > 0 && (stack = sys_get_stack(stksz)) == 0) {
		sys_rel_stack(sstack);
		return(E_NOMEM);
	}
#endif /* USE_SEPARATE_SSTACK */

#ifndef _i_vcre_tsk
	if (!SYS_TSKID(tcb->tskid)) {
		queue_delete(&(tcb->tskque));
	}
#endif /* _i_vcre_tsk */

	tcb->exinf = pk_ctsk->exinf;
	tcb->tskatr = pk_ctsk->tskatr;
	tcb->task = pk_ctsk->task;
	tcb->ipriority = int_priority(pk_ctsk->itskpri);
#ifdef USE_SEPARATE_SSTACK
	tcb->stksz = stksz;
#endif /* USE_SEPARATE_SSTACK */
	tcb->sstksz = sstksz;

#ifdef USE_SEPARATE_SSTACK
	if (stksz > 0) {
		tcb->istack = (VP)(((VB *) stack) + stksz);
	}
#endif /* USE_SEPARATE_SSTACK */
	tcb->isstack = (VP)(((VB *) sstack) + sstksz);
#ifdef USE_QTSK_PORTION
	tcb->isysmode = sysmode;
#else /* USE_QTSK_PORTION */
	tcb->sysmode = sysmode;
#endif /* USE_QTSK_PORTION */

	make_dormant(tcb);
	return(E_OK);
}

#endif
#ifndef _i_cre_tsk

SYSCALL ER
i_cre_tsk(ID tskid, T_CTSK *pk_ctsk)
{
	TCB	*tcb;
#ifdef USE_SEPARATE_SSTACK
	INT	stksz, sstksz, sysmode;
#else /* USE_SEPARATE_SSTACK */
	INT	sstksz, sysmode;
#endif /* USE_SEPARATE_SSTACK */
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	if (SYS_TSKID(tskid)) {
		CHECK_RSATR(pk_ctsk->tskatr, TA_HLNG|TA_SSTKSZ);
	}
	else {
		CHECK_RSATR(pk_ctsk->tskatr, TA_HLNG|TA_SSTKSZ|TA_RNG3);
	}
	CHECK_PRI(pk_ctsk->itskpri);
	CHECK_PAR(pk_ctsk->stksz >= 0);

	if (pk_ctsk->tskatr & TA_SSTKSZ) {
		CHECK_PAR(pk_ctsk->sstksz >= MIN_SYS_STACK_SIZE);
		sstksz = pk_ctsk->sstksz;
	}
	else {
		sstksz = DEF_SYS_STACK_SIZE;
	}
	if ((pk_ctsk->tskatr & TA_RNG3) == TA_RNG0) {
		sysmode = 1;
#ifdef USE_SEPARATE_SSTACK
		sstksz += pk_ctsk->stksz;
		stksz = 0;
#endif /* USE_SEPARATE_SSTACK */
	}
	else {
		sysmode = 0;
#ifdef USE_SEPARATE_SSTACK
		stksz = pk_ctsk->stksz;
#endif /* USE_SEPARATE_SSTACK */
	}
#ifndef USE_SEPARATE_SSTACK
	sstksz += pk_ctsk->stksz;
#endif /* USE_SEPARATE_SSTACK */
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if (tcb->state != TS_NONEXIST) {
		ercd = E_OBJ;
	}
	else {
#ifdef USE_SEPARATE_SSTACK
		ercd = _cre_tsk(tcb, pk_ctsk, stksz, sstksz, sysmode);
#else /* USE_SEPARATE_SSTACK */
		ercd = _cre_tsk(tcb, pk_ctsk, sstksz, sysmode);
#endif /* USE_SEPARATE_SSTACK */
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_tsk */
#ifndef _i_vcre_tsk

SYSCALL ER
i_vcre_tsk(T_CTSK *pk_ctsk)
{
	TCB	*tcb;
#ifdef USE_SEPARATE_SSTACK
	INT	stksz, sstksz, sysmode;
#else /* USE_SEPARATE_SSTACK */
	INT	sstksz, sysmode;
#endif /* USE_SEPARATE_SSTACK */
	ER	ercd = E_OK;

	CHECK_RSATR(pk_ctsk->tskatr, TA_HLNG|TA_SSTKSZ|TA_RNG3);
	CHECK_PRI(pk_ctsk->itskpri);
	CHECK_PAR(pk_ctsk->stksz >= 0);

	if (pk_ctsk->tskatr & TA_SSTKSZ) {
		CHECK_PAR(pk_ctsk->sstksz >= MIN_SYS_STACK_SIZE);
		sstksz = pk_ctsk->sstksz;
	}
	else {
		sstksz = DEF_SYS_STACK_SIZE;
	}
	if ((pk_ctsk->tskatr & TA_RNG3) == TA_RNG0) {
		sysmode = 1;
#ifdef USE_SEPARATE_SSTACK
		sstksz += pk_ctsk->stksz;
		stksz = 0;
#endif /* USE_SEPARATE_SSTACK */
	}
	else {
		sysmode = 0;
#ifdef USE_SEPARATE_SSTACK
		stksz = pk_ctsk->stksz;
#endif /* USE_SEPARATE_SSTACK */
	}
#ifndef USE_SEPARATE_SSTACK
	sstksz += pk_ctsk->stksz;
#endif /* USE_SEPARATE_SSTACK */

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_tcb)) {
		ercd = EV_FULL;
	}
	else {
		tcb = (TCB *)(free_tcb.next);
#ifdef USE_SEPARATE_SSTACK
		ercd = _cre_tsk(tcb, pk_ctsk, stksz, sstksz, sysmode);
#else /* USE_SEPARATE_SSTACK */
		ercd = _cre_tsk(tcb, pk_ctsk, sstksz, sysmode);
#endif /* USE_SEPARATE_SSTACK */
		if (ercd == E_OK) {
			ercd = (ER)(tcb->tskid);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_tsk */
#if !defined(_i_del_tsk) || !defined(_i_exd_tsk)

static void
_del_tsk(TCB *tcb)
{
	sys_rel_stack((VP)((VB *)(tcb->isstack) - tcb->sstksz));
#ifdef USE_SEPARATE_SSTACK
	if (tcb->stksz > 0) {
		sys_rel_stack((VP)((VB *)(tcb->istack) - tcb->stksz));
	}
#endif /* USE_SEPARATE_SSTACK */
#ifndef _i_vcre_tsk
	if (!SYS_TSKID(tcb->tskid)) {
		queue_insert(&(tcb->tskque), &free_tcb);
	}
#endif /* _i_vcre_tsk */
	tcb->state = TS_NONEXIST;
}

#endif
#ifndef _i_del_tsk

SYSCALL ER
i_del_tsk(ID tskid)
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ((state = tcb->state) != TS_DORMANT) {
		ercd = (state == TS_NONEXIST) ? E_NOEXS : E_OBJ;
	}
	else {
		_del_tsk(tcb);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_tsk */

/*
 *  �^�X�N�̋N���ƏI��
 */

#ifndef _i_sta_tsk

SYSCALL ER
i_sta_tsk(ID tskid, INT stacd)
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ((state = tcb->state) != TS_DORMANT) {
		ercd = (state == TS_NONEXIST) ? E_NOEXS : E_OBJ;
	}
	else {
		setup_stacd(tcb, stacd);
		make_ready(tcb);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_sta_tsk */
#if !defined(_i_ext_tsk) || !defined(_i_exd_tsk) || !defined(_i_ter_tsk)

void
_ter_tsk(TCB *tcb)
{
	TSTAT	state;

	if ((state = tcb->state) == TS_READY) {
		make_non_ready(tcb);
	}
	else if (state & TS_WAIT) {
		wait_cancel(tcb);
#ifdef USE_REL_WAI_HOOK
		if (tcb->wspec->rel_wai_hook) {
			(*(tcb->wspec->rel_wai_hook))(tcb);
		}
#endif /* USE_REL_WAI_HOOK */
	}
#ifdef PRISEM_SPEC1
	signal_all_prisem(tcb);
#endif /* PRISEM_SPEC1 */
}

#endif
#ifndef _i_ext_tsk

SYSCALL void
i_ext_tsk(void)
{
#ifdef DORMANT_STACK_SIZE
	/*
	 *  make_dormant �ŁC�g�p���̃X�^�b�N��j�󂵂Ȃ��悤�ɁC�X�^�b
	 *  �N��Ƀ_�~�[�G���A���m�ۂ���D
	 */
	(void) alloca(DORMANT_STACK_SIZE);
#endif /* DORMANT_STACK_SIZE */

	/*
	 *  �R���e�L�X�g�G���[�̏����D
	 */
#ifdef CHK_CTX2
	if (in_indp()) {
		i_syslog(LOG_KERN|LOG_EMERG, "ext_tsk reports E_CTX.");
		itis_exit();
	}
#endif /* CHK_CTX2 */
#ifdef CHK_CTX1
	if (in_ddsp()) {
		i_syslog(LOG_KERN|LOG_ALERT, "ext_tsk reports E_CTX.");
	}
#endif /* CHK_CTX1 */

	DISABLE_INTERRUPT;
	_ter_tsk(ctxtsk);
	make_dormant(ctxtsk);

	force_dispatch();
}

#endif /* _i_ext_tsk */
#ifndef _i_exd_tsk

SYSCALL void
i_exd_tsk(void)
{
	/*
	 *  �R���e�L�X�g�G���[�̏����D
	 */
#ifdef CHK_CTX2
	if (in_indp()) {
		i_syslog(LOG_KERN|LOG_EMERG, "exd_tsk reports E_CTX.");
		itis_exit();
	}
#endif /* CHK_CTX2 */
#ifdef CHK_CTX1
	if (in_ddsp()) {
		i_syslog(LOG_KERN|LOG_ALERT, "exd_tsk reports E_CTX.");
	}
#endif /* CHK_CTX1 */

	DISABLE_INTERRUPT;
	_ter_tsk(ctxtsk);

	/*
	 *  ������ _del_tsk ���Ăԕ��@�́C�}���`�v���Z�b�T�V�X�e���̏�
	 *  ���ŁC���̃v���Z�b�T�ƃV�X�e���������v�[�������L���Ă����
	 *  ���ɂ͐������Ȃ��D
	 */
	_del_tsk(ctxtsk);

	force_dispatch();
}

#endif /* _i_exd_tsk */
#ifndef _i_ter_tsk

SYSCALL ER
i_ter_tsk(ID tskid)
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	CHECK_INTSK();
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if (!task_alive(state = tcb->state)) {
		ercd = (state == TS_NONEXIST) ? E_NOEXS : E_OBJ;
	}
	else {
		_ter_tsk(tcb);
		make_dormant(tcb);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ter_tsk */

/*
 *  �^�X�N�D��x�̕ύX
 */
#ifndef _i_chg_pri

SYSCALL ER
i_chg_pri(ID tskid, PRI tskpri)
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);
	CHECK_TSKACV(tskid);
	CHECK_PRI_INI(tskpri);
	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	if (tcb->state == TS_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (tskpri == TPRI_INI) {
		change_task_priority(tcb, tcb->ipriority);
	}
	else {
		change_task_priority(tcb, int_priority(tskpri));
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_chg_pri */

/*
 *  ���f�B�L���[�̉�]
 */
#ifndef _i_rot_rdq

SYSCALL ER
i_rot_rdq(PRI tskpri)
{
	CHECK_PRI_RUN(tskpri);

	BEGIN_CRITICAL_SECTION;
	if (tskpri == TPRI_RUN) {
		if (in_indp()) {
			rotate_ready_queue_run();
		}
		else {
			rotate_ready_queue(ctxtsk->priority);
		}
	}
	else {
		rotate_ready_queue(int_priority(tskpri));
	}
	END_CRITICAL_SECTION;
	return(E_OK);
}

#endif /* _i_rot_rdq */

/*
 *  ���^�X�N�̑҂���ԉ���
 */
#ifndef _i_rel_wai

SYSCALL ER
i_rel_wai(ID tskid)
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if (!((state = tcb->state) & TS_WAIT)) {
		ercd = (state == TS_NONEXIST) ? E_NOEXS : E_OBJ;
	}
	else {
		*(tcb->wercd) = E_RLWAI;
		wait_release(tcb);
#ifdef USE_REL_WAI_HOOK
		if (tcb->wspec->rel_wai_hook) {
			(*(tcb->wspec->rel_wai_hook))(tcb);
		}
#endif /* USE_REL_WAI_HOOK */
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_rel_wai */

/*
 *  ���^�X�N�̃^�X�NID�Q��
 */
#ifndef _i_get_tid

SYSCALL ER
i_get_tid(ID* p_tskid)
{
	*p_tskid = in_indp() ? FALSE : ctxtsk->tskid;
	return(E_OK);
}

#endif /* _i_get_tid */

/*
 *  �^�X�N��ԎQ��
 */
#ifndef _i_ref_tsk

SYSCALL ER 
i_ref_tsk(T_RTSK *pk_rtsk, ID tskid)
{
	TCB	*tcb;
	TSTAT	state;
	GCB	*wgcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);
	CHECK_TSKACV(tskid);
	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	if ((state = tcb->state) == TS_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rtsk->exinf = tcb->exinf;
		pk_rtsk->tskpri = ext_tskpri(tcb->priority);
		if (tcb == ctxtsk) {
			pk_rtsk->tskstat = TTS_RUN;
		}
		else {
			pk_rtsk->tskstat = (UW)(state << 1);
		}
		if (state & TS_WAIT) {
			pk_rtsk->tskwait = tcb->wspec->tskwait;
			if (wgcb = tcb->wgcb) {
				pk_rtsk->wid = wgcb->objid;
			}
			else {
				pk_rtsk->wid = 0;
			}
		}
		else {
			pk_rtsk->tskwait = 0;
			pk_rtsk->wid = 0;
		}
		pk_rtsk->wupcnt = tcb->wupcnt;
		pk_rtsk->suscnt = tcb->suscnt;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_tsk */
