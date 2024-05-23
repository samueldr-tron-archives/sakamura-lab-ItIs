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
 *  @(#) $Id: cpu_task.h,v 1.5 1996/02/17 09:23:38 hiro Exp $
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

/*
 *  �^�X�N�N�����[�`��
 */
extern void	task_start(void);

/*
 *  CPU�ˑ��̃^�X�N�N������
 *
 *  �V�X�e���X�^�b�N��ɁCEIT�X�^�b�N�t���[�������Dmake_dormant ����
 *  �Ă΂��D
 */
Inline void
setup_context(TCB *tcb)
{
	((int *) &(tcb->tskctxb.env))[JMPBUF_PC] = (int) task_start;
	((int *) &(tcb->tskctxb.env))[JMPBUF_SP] = (int)(((VB *) tcb->isstack)
							- LONGJMP_MERGIN);
}

/*
 *  �^�X�N�N���R�[�h���̐ݒ�
 *
 *  sta_tsk �̏�������Ă΂��D
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	tcb->tskctxb.stacd = stacd;
}

/*
 *  �X�^�b�N�G���A�̎擾/�ԋp
 *
 *  USE_MPROTECT_STACK ����`����Ă���ꍇ�ɂ́C�X�^�b�N�G���A�� 2�y�[
 *  �W�����߂Ɋm�ۂ��C���̕��� 1�y�[�W�� mprotect �ŃA�N�Z�X�s�ɂ���D
 *  �ň��ł��C�m�ۂ��悤�Ƃ������̃G���A�͎g����D
 */

#ifdef USE_MPROTECT_STACK

#include <sys/mman.h>

#ifndef PROT_NONE
#define PROT_NONE	0x00		/* �y�[�W���A�N�Z�X�ł��Ȃ����� */
#endif

#define PROT_ORIG	(PROT_READ|PROT_WRITE)
					/* �y�[�W�̏�Ԃ����ɖ߂� */

#define ALIGN(addr, unit)	((((INT)(addr)) + (unit) - 1) & ~((unit) - 1))

Inline VP
sys_get_stack(INT size)
{
	VP	blk;
	INT	pagesize = getpagesize();
  
	size += pagesize * 2;
	if (blk = sys_get_blk(size)) {
		mprotect((caddr_t) ALIGN(blk, pagesize), pagesize, PROT_NONE);
	}
	blk += pagesize * 2;
	return(blk);
}

Inline void
sys_rel_stack(VP blk)
{
	INT	pagesize = getpagesize();

	blk -= pagesize * 2;
	mprotect((caddr_t) ALIGN(blk, pagesize), pagesize, PROT_ORIG);
	sys_rel_blk(blk);
}

#endif /* USE_MPROTECT_STACK */
#endif /* _CPU_TASK_ */
