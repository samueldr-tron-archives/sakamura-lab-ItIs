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
 *  @(#) $Id: sys_timer.h,v 1.4 1996/02/17 09:44:28 hiro Exp $
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

#include "tvme150.h"

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
 *  TVME-150 CPU�{�[�h�ł́C�^�C�}�� 0.5��sec ���ɃJ�E���g�A�b�v����D
 */
#define TIMER_TICK	2000
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
 *  �����\���� 0xffff �͖� 32msec�D
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
	define_eit(INT_VECTOR(2), EITATR(0, 15), timer_handler_startup);

	/*
	 *  interrupt handler �̐ݒ�
	 */
	scb_assign(SCB_R0, 2);
	scb_assign(SCB_R1, 0x04);		/* ���x���g���K���[�h */
	scb_or_assign(SCB_R3, LRQ2_BIT);	/* �����݃}�X�N���� */

	/*
	 *  CIO �̐ݒ�
	 */
	cio_write(CIOA_MICR, cio_read(CIOA_MICR) | 0x80);
	cio_write(CIOA_MCCR, cio_read(CIOA_MCCR) | 0x40);
	cio_write(CIOA_CTMSR1, 0x80);
	assert(t <= MAX_TICK);			/* �^�C�}����l�̃`�F�b�N */
	cio_write(CIOA_CTTCR1H, (t >> 8) & 0xff);
	cio_write(CIOA_CTTCR1L, t & 0xff);
	cio_write(CIOA_CTCSR1, 0x20);		/* �����ݗv�����N���A */
	cio_write(CIOA_CTCSR1, 0xc6);		/* �^�C�}���X�^�[�g������ */
}

/*
 *  �^�C�}�����݂̃N���A
 */
Inline void
clear_hw_timer_interrupt(void)
{
	cio_write(CIOA_CTCSR1, 0x24);		/* �����ݗv�����N���A */
}

/*
 *  �^�C�}�̒�~����
 */
Inline void
terminate_hw_timer(void)
{
	cio_write(CIOA_CTCSR1, 0xe0);		/* �����݂��֎~ */
	cio_write(CIOA_CTCSR1, 0x20);		/* �����ݗv�����N���A */
	scb_and_assign(SCB_R3, ~LRQ2_BIT);	/* �����݃}�X�N�ݒ� */
}

/*
 *  �^�C�}�̌��ݒl�̓ǂ݂���
 *
 *  �����݋֎~��Ԓ��ŌĂяo�����ƁD
 */
Inline TICK
get_current_hw_time(void)
{
	byte	ccr1h, ccr1l;

	cio_write(CIOA_CTCSR1, 0x0c);		/* �^�C�}�l�ǂݏo���J�n */
	ccr1h = cio_read(CIOA_CTCCR1H);
	ccr1l = cio_read(CIOA_CTCCR1L);
	return(TO_TICK(TIMER_PERIOD) - ((ccr1h << 8) + ccr1l));
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(*SCB_R4 & LRQ2_BIT);
}

#endif /* _SYS_TIMER_ */
