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
 *  @(#) $Id: sys_timer.h,v 1.3 1996/02/17 09:23:42 hiro Exp $
 */

#ifndef _SYS_TIMER_
#define _SYS_TIMER_

#include <sys/time.h>

/*
 *  �n�[�h�E�F�A�ˑ��^�C�}���W���[��
 *
 *  ���̃��W���[�����ŁC�ȉ��̒萔����ъ֐����`���Ȃ���΂Ȃ�Ȃ��D
 * 
 *  TIMER_PERIOD : �^�C�}�����݂̎��� (�P�ʂ� msec)�DItIs �ł̕W���l�� 
 *  1msec �ł��邪�C�^�C�}�����݂ɂ��I�[�o�w�b�h���������������ꍇ��
 *  �́C�����ƒ��������ɐݒ肵�Ă��悢�D�������C�^�C���A�E�g���ԂȂ�
 *  �̕���x�͑e���Ȃ� (�P�ʂ͕ς��Ȃ�)�D
 *
 *  void start_hw_timer(FP handler) : �^�C�}�����������C�����I�ȃ^�C�}
 *  �����݂��J�n������Dhandler �́C�����I�ɋN�����ׂ��n���h���̊J�n�A
 *  �h���X�D
 *
 *  void clear_hw_timer_interupt(void) : �^�C�}�����ݗv�����N���A����D
 *  �^�C�}�����݃n���h���̍ŏ��ŌĂ΂��D
 *
 *  void terminate_hw_timer(void) : �^�C�}�̓�����~������D�V�X�e��
 *  ��~���ɌĂ΂��D
 */ 

/*
 *  �^�C�}�����݂̎��� (�P�ʂ� msec)
 */
#define TIMER_PERIOD	10

/*
 *  �^�C�}�n���h��
 */
extern void	timer_handler(void);

/*
 *  �^�C�}�̃X�^�[�g����
 */
Inline void
start_hw_timer(void)
{
	struct sigvec	vec;
	struct itimerval val;

	vec.sv_handler = timer_handler;
	vec.sv_mask = SIGMASK_INDP;
	vec.sv_flags = SV_ONSTACK|SV_INTERRUPT;
	sigvec(SIGALRM, &vec, 0);

	val.it_interval.tv_sec = TIMER_PERIOD / 1000;
	val.it_interval.tv_usec = (TIMER_PERIOD % 1000) * 1000;
	val.it_value = val.it_interval;
	setitimer(ITIMER_REAL, &val, 0);
}

/*
 *  �^�C�}�����݂̃N���A
 */
Inline void
clear_hw_timer_interrupt(void)
{
}

/*
 *  �^�C�}�̒�~����
 */
Inline void
terminate_hw_timer(void)
{
	struct itimerval val;

	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = 0;
	val.it_value = val.it_interval;
	setitimer(ITIMER_REAL, &val, 0);
}

#endif /* _SYS_TIMER_ */
