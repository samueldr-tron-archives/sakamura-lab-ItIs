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
 *  @(#) $Id: sys_util.c,v 1.3 1996/02/17 09:44:06 hiro Exp $
 */

#include "itis_kernel.h"
#include "mcube.h"

/*
 *  �^�[�Q�b�g�V�X�e���ˑ��̏��������[�`��
 */
void
sys_initialize(void)
{
	/*
	 *  IRC �̏�����
	 */
	irc_assign(IRC1_IMR, 0x7e);	/* ABORT�ȊO�̑S�����݂��}�X�N */
	irc_assign(IRC1_BMR, 0x7f);
	irc_assign(IRC2_IMR, 0x7f);

	irc_assign(IRC1_VMR, 0x7f);	/* �I�[�g�x�N�^���[�h�ɐݒ� */
	irc_assign(IRC1_VNR, IRC2_VECTOR_BASE);
	irc_assign(IRC2_VMR, 0x00);	/* IRC�x�N�^���[�h�ɐݒ� */
	irc_assign(IRC2_VSR, 0x00);
	irc_assign(IRC2_VNR, IRC2_VECTOR_BASE);
}

/*
 *  �^�[�Q�b�g�V�X�e���̏I�����[�`��
 *
 *  ItIs �̃V�X�e�����I�����鎞�Ɏg���D�ʏ�̓��j�^�Ăяo���Ŏ�������D
 */
void
sys_exit(void)
{
	Asm("ldc #0, %0" : : "m"(CREG(PSW)));
	mcube_exit();
}

/*
 *  �^�[�Q�b�g�V�X�e���̕����o�̓��[�`��
 *
 *  ���O�^�X�N�������Ă��Ȃ����ɁC�V�X�e������̃��b�Z�[�W���o�͂��邽
 *  �߂Ɏg���D�ʏ�̓��j�^�Ăяo���Ŏ�������D
 */
int
sys_write(const char *buf, unsigned int len)
{
	int	i;

	for (i = 0; i < len; i++) {
		mcube_putc(*buf++);
	}
	return(len);
}
