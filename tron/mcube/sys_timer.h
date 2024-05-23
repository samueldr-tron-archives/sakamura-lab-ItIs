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
 *  @(#) $Id: sys_timer.h,v 1.4 1996/02/17 09:44:03 hiro Exp $
 */

#ifndef _SYS_TIMER_
#define _SYS_TIMER_

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
 *  void start_hw_timer() : �^�C�}�����������C�����I�ȃ^�C�}�����݂��J
 *  �n������D
 *
 *  void clear_hw_timer_interupt(void) : �^�C�}�����ݗv�����N���A����D
 *  �^�C�}�����݃n���h���̍ŏ��ŌĂ΂��D
 *
 *  void terminate_hw_timer(void) : �^�C�}�̓�����~������D�V�X�e��
 *  ��~���ɌĂ΂��D
 */

#include "mcube.h"

/*
 *  �^�C�}�����݂̎��� (�P�ʂ� msec)
 */
#define TIMER_PERIOD	1

/*
 *  �^�C�}�l�̓����\���̌^
 */
typedef unsigned int	TICK;

/*
 *  �^�C�}�l�̓����\���� msec �P�ʂƂ̕ϊ�
 *
 *  MCUBE ��{�{�[�h�ł́C�^�C�} (�J�E���^2 ��p����) �� 1��sec ���ɃJ
 *  �E���g�A�b�v����D
 */
#define TIMER_TICK	1000
#define TO_TICK(msec)	((msec) * TIMER_TICK)

/*
 *  �^�C�}�l�̓����\���ƃ�sec �P�ʂƂ̕ϊ�
 */
#if TIMER_TICK <= 1000
#define TO_USEC(tick)	((tick) * (1000 / TIMER_TICK))
#else
#define TO_USEC(tick)	((tick) / (TIMER_TICK / 1000))
#endif

/*
 *  ���\�]���p�V�X�e�����������o���ۂ̏������Ԃ̌��ς�l (�P�ʂ͓����\��)
 */
#define GET_TOLERANCE	(TIMER_TICK / 10)

/*
 *  �ݒ�ł���ő�̃^�C�}���� (�P�ʂ͓����\��)
 *
 *  �����\���� 0xffff �͖� 65msec�D
 */
#define MAX_TICK	((TICK) 0xffff)

/*
 *  �A�Z���u�����x���̃^�C�}�n���h��
 */
extern void	timer_handler_startup(void);

/*
 *  �^�C�}�̃X�^�[�g����
 */
Inline void
start_hw_timer()
{
	TICK	t = TO_TICK(TIMER_PERIOD);

	/*
	 *  �����݃n���h���̒�`
	 */
	define_eit(IRC1_VECTOR(1), EITATR(0, 15), timer_handler_startup);

	/*
	 *  IRC �̐ݒ�
	 */
	irc_assign(IRC1_LMR(1), LIR1_BIT);
	irc_or_assign(IRC1_TMR, LIR1_BIT);	/* �G�b�W�g���K���[�h */
	irc_and_assign(IRC1_IMR, ~LIR1_BIT);	/* �����݃}�X�N���� */

	/*
	 *  �^�C�}�̐ݒ�
	 */
	tmr_write(TMR_CNTL, 0xb4);		/* �^�C�}���[�h2 */
	assert(t <= MAX_TICK);			/* �^�C�}����l�̃`�F�b�N */
	tmr_write(TMR_CNT2, t & 0xff);
	tmr_write(TMR_CNT2, (t >> 8) & 0xff);

	irc_assign(IRC1_IRR, LIR1_BIT);		/* �����ݗv�����N���A */
}

/*
 *  �^�C�}�����݂̃N���A
 */
Inline void
clear_hw_timer_interrupt(void)
{
	irc_assign(IRC1_IRR, LIR1_BIT);		/* �����ݗv�����N���A */
}

/*
 *  �^�C�}�̒�~����
 */
Inline void
terminate_hw_timer(void)
{
	irc_or_assign(IRC1_IMR, LIR1_BIT);	/* �����݃}�X�N�ݒ� */
}

/*
 *  �^�C�}�̌��ݒl�̓ǂݏo��
 *
 *  �����݋֎~��Ԓ��ŌĂяo�����ƁD
 *
 *  �{���C0 �` TO_TICK(TIMER_PERIOD)-1 �̒l���Ԃ�͂������C���ۂɂ͕p�x
 *  �͒Ⴂ�� TO_TICK(TIMER_PERIOD) ���Ԃ邱�Ƃ�����D
 */
Inline TICK
get_current_hw_time(void)
{
	TICK	t;

	tmr_write(TMR_CNTL, 0x80);		/* �^�C�}�l�ǂݏo���J�n */
	t = tmr_read(TMR_CNT2);
	t += tmr_read(TMR_CNT2) << 8;
	return(TO_TICK(TIMER_PERIOD) - t);
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(irc_read(IRC1_IRR) & LIR1_BIT);
}

#endif /* _SYS_TIMER_ */
