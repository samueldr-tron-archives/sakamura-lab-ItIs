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
 *  @(#) $Id: task.c,v 1.25 1997/01/10 13:36:32 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "ready_queue.h"
#include "cpu_task.h"

/*
 *  �^�X�N�f�B�X�p�b�`�֎~���
 */
#ifdef USE_DISPATCH_DISABLED
BOOL	dispatch_disabled;
#endif /* USE_DISPATCH_DISABLED */

/*
 *  ���s���̃^�X�N
 */
TCB	*ctxtsk;

/*
 *  ���s���ׂ��^�X�N
 */
TCB	*schedtsk;

/*
 *  TCB �̃G���A
 */
TCB	tcb_table[NUM_TSKID];

/*
 *  ���f�B�L���[
 */ 
RDYQUE	ready_queue;

/*
 *  ���g�p�� TCB �̃��X�g
 */
#ifndef _i_vcre_tsk
QUEUE	free_tcb;
#endif /* _i_vcre_tsk */

/*
 *  TCB �̏�����
 */
void
task_initialize(void)
{
	INT	i;
	TCB	*tcb;
	ID	tskid;

	ctxtsk = schedtsk = (TCB *) 0;
	ready_queue_initialize(&ready_queue);
#ifndef _i_vcre_tsk
	queue_initialize(&free_tcb);
#endif /* _i_vcre_tsk */

	for(tcb = tcb_table, i = 0; i < NUM_TSKID; tcb++, i++) {
		tskid = ID_TSK(i);
		tcb->tskid = tskid;
		tcb->state = TS_NONEXIST;
#ifdef USE_POR
		tcb->wrdvno = (W)(UH) tskid;
#endif /* USE_POR */
#ifndef _i_vcre_tsk
		if (!SYS_TSKID(tskid)) {
			queue_insert(&(tcb->tskque), &free_tcb);
		}
#endif /* _i_vcre_tsk */
	}
#ifdef USE_DISPATCH_DISABLED
	dispatch_disabled = 0;
#endif /* USE_DISPATCH_DISABLED */
}

/*
 *  �^�X�N�̎��s����������D
 */
void
make_dormant(TCB *tcb)
{
	/*
	 *  DORMANT��Ԃł̓��Z�b�g����Ă���ׂ��ϐ����������D
	 */
	tcb->state = TS_DORMANT;
	tcb->priority = tcb->ipriority;
	tcb->wupcnt = 0;
	tcb->suscnt = 0;
#ifdef USE_QTSK_PORTION
	tcb->sysmode = tcb->isysmode;
#endif /* USE_QTSK_PORTION */
#ifdef PRISEM_SPEC1
	tcb->pislist = (PISCB *) 0;
#endif /* PRISEM_SPEC1 */
#ifdef USE_TASK_MAILBOX
	tcb->tmq_head = (T_MSG *) 0;
#endif /* USE_TASK_MAILBOX */

	/*
	 *  �^�X�N�N���̂��߂̃R���e�L�X�g�̐ݒ�D
	 */
	setup_context(tcb);
}

/*
 *  ���s���ׂ��^�X�N��I���������D
 *
 *  schedtsk �����f�B�L���[�̐擪�̃^�X�N�ƈ�v������D
 */
Inline void
reschedule(void)
{
	TCB	*toptsk;

	if (schedtsk != (toptsk = ready_queue_top(&ready_queue))) {
		schedtsk = toptsk;
		dispatch_request();
	}
}

/*
 *  �^�X�N�����s�\��Ԃɂ���D
 *
 *  �^�X�N��Ԃ��X�V���C���f�B�L���[�ɑ}������D�K�v�Ȃ�Cschedtsk ��
 *  �X�V���C�^�X�N�f�B�X�p�b�`���̋N����v������D
 */
void
make_ready(TCB *tcb)
{
	tcb->state = TS_READY;
	if (ready_queue_insert(&ready_queue, tcb)) {
		schedtsk = tcb;
		dispatch_request();
	}
}

/*
 *  �^�X�N�����s�\�ȊO�̏�Ԃɂ���D
 *
 *  �^�X�N�����f�B�L���[����폜����D�폜�����^�X�N�� schedtsk �ł���
 *  ���ꍇ�ɂ́Cschedtsk �����f�B�L���[���̍ō��D��x�^�X�N�ɐݒ肷��D
 */
void
make_non_ready(TCB *tcb)
{
	assert(tcb->state == TS_READY);
	ready_queue_delete(&ready_queue, tcb);
	if (schedtsk == tcb) {
		schedtsk = ready_queue_top(&ready_queue);
		dispatch_request();
	}
}

/*
 *  �^�X�N�̗D��x��ύX����D
 */
void
change_task_priority(TCB *tcb, INT priority)
{
	INT	oldpri;

	if (tcb->state == TS_READY) {
		/*
		 *  �^�X�N�����f�B�L���[����폜����ۂ� TCB ��
		 *  priority �t�B�[���h�̒l���K�v�ɂȂ邽�߁C��
		 *  �f�B�L���[����̍폜�́Ctcb->priority ����
		 *  ��������O�ɍs��Ȃ���΂Ȃ�Ȃ��D
		 */
		ready_queue_delete(&ready_queue, tcb);
		tcb->priority = priority;
		ready_queue_insert(&ready_queue, tcb);
		reschedule();
	}
	else {
		oldpri = tcb->priority;
		tcb->priority = priority;
		if ((tcb->state & TS_WAIT) && tcb->wspec->chg_pri_hook) {
			(*(tcb->wspec->chg_pri_hook))(tcb, oldpri);
		}
	}
}

/*
 *  ���f�B�L���[����]������D
 */
void
rotate_ready_queue(INT priority)
{
	ready_queue_rotate(&ready_queue, priority);
	reschedule();
}

/*
 *  �ō��D��x�̃^�X�N���܂ރ��f�B�L���[����]������D
 */
void
rotate_ready_queue_run(void)
{
	if (schedtsk) {
		ready_queue_rotate(&ready_queue,
				ready_queue_top_priority(&ready_queue));
		reschedule();
	}
}
