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
 *  @(#) $Id: timer.c,v 1.7 1996/07/28 00:28:38 hiro Exp $
 */

#include "itis_kernel.h"
#include "timer.h"
#include "task.h"
#include "sys_timer.h"

/*
 *  ���ݎ��� (�\�t�g�E�F�A�N���b�N)
 */
SYSTIME	current_time;

/* 
 *  �^�C�}�L���[
 */
static QUEUE	timer_queue;

/*
 *  �^�C�}���W���[���̏�����
 */
void
timer_initialize(void)
{
	current_time = 0;
	queue_initialize(&timer_queue);
	start_hw_timer();
}

/*
 *  �^�C�}�̒�~
 */
void
timer_shutdown(void)
{
	terminate_hw_timer();
}

/*
 *  �^�C�}�C�x���g���^�C�}�L���[�֑}��
 */
static void
enqueue_tmeb(TMEB *event)
{
	QUEUE	*q;

	for (q = timer_queue.next; q != &timer_queue; q = q->next) {
		if (event->time <= ((TMEB *) q)->time) {
			break;
		}
	}
	queue_insert(&(event->queue), q);
}

/*
 *  �^�C�}�C�x���g event ���C�^�C���A�E�g���� tmout ��ɋN�������悤
 *  �Ƀ^�C�}�L���[�֓o�^����D�^�C���A�E�g���Ԃ�����ƁC�R�[���o�b�N�� 
 *  �� callback �Ɉ��� arg ��^���ċN������Dtmout �� TMO_FEVR �̎��́C
 *  �^�C�}�L���[�ɂ͓o�^���Ȃ����C��� timer_delete ���Ă΂�Ă��悢��
 *  ���ɁC�L���[�̃G���A������������D
 */
void
timer_insert(TMEB *event, TMO tmout, CBACK callback, VP arg)
{
	event->callback = callback;
	event->arg = arg;

	if (tmout == TMO_FEVR) {
		queue_initialize(&(event->queue));
	}
	else {
		assert(tmout > 0);
		event->time = current_time + tmout;
		enqueue_tmeb(event);
	}
}

/*
 *  �^�C�}�C�x���g event ���C(���) ���� time �ɋN�������悤�Ƀ^�C�}
 *  �L���[�֓o�^����D
 */
void
timer_insert_abs(TMEB *event, SYSTIME *time, CBACK callback, VP arg)
{
	event->callback = callback;
	event->arg = arg;

	event->time = *time;
	enqueue_tmeb(event);
}

/*
 *  �^�C�}�C�x���g event ���Ctime ���Ԍ�ɋN�������悤�ɂɃ^�C�}�L���[
 *  �֓o�^����D
 */
void
timer_insert_rel(TMEB *event, SYSTIME *time, CBACK callback, VP arg)
{
	event->callback = callback;
	event->arg = arg;

	event->time = current_time + *time;
	enqueue_tmeb(event);
}

/*
 *  �^�C�}�����݃n���h��
 *
 *  �^�C�}�����݃n���h���́C�n�[�h�E�F�A�^�C�}�ɂ�� TIMER_PERIOD�~���b
 *  �̎����ŋN�������D�\�t�g�E�F�A�N���b�N���X�V���C�N�����Ԃ̗����^
 *  �C�}�C�x���g�̋N�����s���D
 */
void
timer_handler(void)
{
	TMEB	*event;

	clear_hw_timer_interrupt();		/* �^�C�}�����݂̃N���A */

	BEGIN_CRITICAL_SECTION;
	current_time += TIMER_PERIOD;

	while (!queue_empty_p(&timer_queue)) {
		event = (TMEB *)(timer_queue.next);
		if (event->time <= current_time) {
			queue_delete(&(event->queue));
			if (event->callback) {
				(*(event->callback))(event->arg);
			}
		}
		else {
			break;
		}
	}
	END_CRITICAL_SECTION;
}

/*
 *  ���\�]���p�V�X�e�������Q�Ƌ@�\
 */

#ifndef _i_vget_tim

SYSCALL ER
i_vget_tim(SYSUTIME *pk_utim)
{
	SYSTIME	ctime;
	TICK	tick;
	BOOL	ireq;

	BEGIN_CRITICAL_SECTION;
	ctime = current_time;
	tick = get_current_hw_time();
	ireq = fetch_hw_timer_interrupt();
	END_CRITICAL_SECTION;

	if (ireq && tick < (TO_TICK(TIMER_PERIOD) - GET_TOLERANCE)) {
		ctime += TIMER_PERIOD;
	}
	*pk_utim = ctime * 1000 + TO_USEC(tick);
	return(E_OK);
}

#endif /* _i_vget_tim */
