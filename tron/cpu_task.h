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
 *  @(#) $Id: cpu_task.h,v 1.3 1996/02/17 09:43:18 hiro Exp $
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

/*
 *  �^�X�N�N���̂��߂̒萔�̒�`
 */
#define INI_PSW_RNG0	0x800f6000	/* PSW �̏����l (�����O���x��0) */
#define INI_PSW_RNG1	0xa00f6000	/* PSW �̏����l (�����O���x��1) */
#define INI_PSW_RNG2	0xc00f6000	/* PSW �̏����l (�����O���x��2) */
#define INI_PSW_RNG3	0xe00f6000	/* PSW �̏����l (�����O���x��3) */

#define INI_CSW		0x00070000	/* CSW �̏����l */

/*
 *  CPU�ˑ��̃^�X�N�N������
 *
 *  �V�X�e���X�^�b�N��ɁCEIT�X�^�b�N�t���[�������Dmake_dormant ����
 *  �Ă΂��D
 */
Inline void
setup_context(TCB *tcb)
{
	VW	*ssp;

	ssp = (VW *)(tcb->isstack);	/* �����V�X�e���X�^�b�N�|�C���^ */
#ifndef TRON_NO_DI
	*--ssp = (VW)(tcb->task);	/* �^�X�N�N���A�h���X��ς� */
	*--ssp = DI14_EITINF;		/* EIT����ς� */
#else /* TRON_NO_DI */
	*--ssp = 0;			/* EXPC (�_�~�[) ��ς� */
	*--ssp = (VW)(tcb->task);	/* �^�X�N�N���A�h���X��ς� */
	*--ssp = TRAPA4_EITINF;		/* EIT����ς� */
#endif /* TRON_NO_DI */

	/*
	 *  PSW �̏����l���V�X�e���X�^�b�N�ɐς݁C���[�U�X�^�b�N�|�C��
	 *  �^������������D
	 */
	switch (tcb->tskatr & TA_RNG3) {
	case TA_RNG0:
		*--ssp = INI_PSW_RNG0;
		break;
	case TA_RNG1:
		tcb->tskctxb.sp1 = tcb->istack;
		*--ssp = INI_PSW_RNG1;
		break;
	case TA_RNG2:
		tcb->tskctxb.sp2 = tcb->istack;
		*--ssp = INI_PSW_RNG2;
		break;
	case TA_RNG3:
		tcb->tskctxb.sp3 = tcb->istack;
		*--ssp = INI_PSW_RNG3;
		break;
	}
	ssp -= 15;			/* R14�`R0 �̕��̃G���A����� */

	tcb->tskctxb.sp0 = ssp;
	tcb->tskctxb.csw = INI_CSW;
}

/*
 *  make_dormant �ŉ󂳂��X�^�b�N�̈�̃T�C�Y�̒�`
 *
 *  ext_tsk �̏������Ŏg����D
 */
#ifndef TRON_NO_DI
#define DORMANT_STACK_SIZE	(sizeof(VW) * 3)
#else /* TRON_NO_DI */
#define DORMANT_STACK_SIZE	(sizeof(VW) * 4)
#endif /* TRON_NO_DI */

/*
 *  �^�X�N�N���R�[�h���̐ݒ�
 *
 *  sta_tsk �̏�������Ă΂��D
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	VW	*ssp;

	ssp = tcb->tskctxb.sp0;
	*ssp = stacd;			/* �N���R�[�h��ς� (R0) */
	*(ssp+1) = (VW)(tcb->exinf);	/* �^�X�N�g������ς� (R1) */
}

#endif /* _CPU_TASK_ */
