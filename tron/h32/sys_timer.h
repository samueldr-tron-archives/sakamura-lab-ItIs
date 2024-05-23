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
 *  @(#) $Id: sys_timer.h,v 1.4 1996/02/17 09:43:45 hiro Exp $
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

#include "h32sbc.h"

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
 *  H32SBC�V�X�e�� CPU�{�[�h�ł́C�^�C�}�� 4��sec ���ɃJ�E���g�A�b�v��
 *  ��D
 */
#define TIMER_TICK	250
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
#define GET_TOLERANCE	(TIMER_TICK / 5)

/*
 *  �ݒ�ł���ő�̃^�C�}���� (�P�ʂ͓����\��)
 *
 *  �����\���� 0xffffff �͖� 67�b�D
 */
#define MAX_TICK	((TICK) 0xffffff)

/*
 *  PIT �^�C�}�J�E���^���W�X�^�̐ݒ�l
 */
#define PIT_CYCLIC	0xe0

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
	TICK	t = TO_TICK(TIMER_PERIOD) - 1;

	/*
	 *  �����݃n���h���̒�`
	 */
	define_eit(INT_VECTOR(2), EITATR(0, 15), timer_handler_startup);

	/*
	 *  IRC �̐ݒ�
	 */
	irc_assign(IRC_LMR(2), LIR6_BIT);
	irc_or_assign(IRC_VMR, LIR6_BIT);
	irc_or_assign(IRC_TMR, LIR6_BIT);	/* �G�b�W�g���K���[�h */
	irc_and_assign(IRC_IMR, ~LIR6_BIT);	/* �����݃}�X�N���� */

	/*
	 *  PIT �̐ݒ�
	 */
	io_assign(PIT_TCR, PIT_CYCLIC);
	assert(t <= MAX_TICK);			/* �^�C�}����l�̃`�F�b�N */
	io_assign(PIT_CPRH, (t >> 16) & 0xff);
	io_assign(PIT_CPRM, (t >> 8) & 0xff);
	io_assign(PIT_CPRL, t & 0xff);
	io_assign(PIT_TSR, 0x01);		/* �[�����o���N���A */
	io_or_assign(PIT_TCR, 0x01);		/* �^�C�}�[���X�^�[�g������ */

	irc_assign(IRC_IRR, LIR6_BIT);		/* �����ݗv�����N���A */
}

/*
 *  �^�C�}�����݂̃N���A
 */
Inline void
clear_hw_timer_interrupt(void)
{
	irc_assign(IRC_IRR, LIR6_BIT);		/* �����ݗv�����N���A */
	io_assign(PIT_TSR, 0x01);		/* �[�����o���N���A */
}

/*
 *  �^�C�}�̒�~����
 */
Inline void
terminate_hw_timer(void)
{
	io_and_assign(PIT_TCR, ~0x01);		/* �^�C�}�[���~�߂� */
	io_assign(PIT_TSR, 0x01);		/* �[�����o���N���A */
	irc_or_assign(IRC_IMR, LIR6_BIT);	/* �����݂��}�X�N */
}

/*
 *  �^�C�}�̌��ݒl�̓ǂ݂���
 *
 *  �����݋֎~��Ԓ��ŌĂяo�����ƁD
 */
Inline TICK
get_current_hw_time(void)
{
#if TO_TICK(TIMER_PERIOD) < 256
	return(TO_TICK(TIMER_PERIOD) - 1 - *PIT_CNTRL);
#else
#error \
  get_current_hw_time is not implemented when TO_TICK(TIMER_PERIOD) >= 256.
#endif
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(irc_read(IRC_IRR) & LIR6_BIT);
}

#endif /* _SYS_TIMER_ */
