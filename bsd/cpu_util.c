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
 *  @(#) $Id: cpu_util.c,v 1.6 1996/02/17 09:23:40 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"

/*
 *  �^�X�N�f�B�X�p�b�`��
 * 
 *  ���s���̃^�X�N (ctxtsk) �̃R���e�L�X�g�� TCB �ɕۑ����C���s���ׂ�
 *  �^�X�N (schedtsk) ��V�������s�^�X�N�Ƃ��āC���̃R���e�L�X�g�� TCB 
 *  ���畜�A����D�^�X�N�f�B�X�p�b�`������߂�ۂɁC�V�O�i���}�X�N�͈�
 *  �O�̏�Ԃɖ߂����D
 */
void
dispatch_handler()
{
	if (!ctxtsk || (ctxtsk != schedtsk
				&& _setjmp(ctxtsk->tskctxb.env) == 0)) {
		sigsetmask(SIGMASK_LOC);
		while (!(ctxtsk = schedtsk)) {
			sigpause(SIGMASK_DDSP);
			sigsetmask(SIGMASK_LOC);
		}
		_longjmp(ctxtsk->tskctxb.env, 1);
	}
}

/*
 *  �^�[�Q�b�gCPU�ˑ��̏��������[�`��
 */
void
cpu_initialize(void)
{
	/*
	 *  �V�O�i���X�^�b�N���C�v���Z�X�X�^�b�N��Ɏ��D�V�O�i���X�^�b
	 *  �N���g���Ă���Ɛݒ肷��̂́C�X�^�[�g�A�b�v���[�`�����^�X
	 *  �N�Ɨ����Ɣ��肳���邽�߁D
	 */
	{
#ifdef USE_SIGSTACK
	       	struct sigstack	ss;

		ss.ss_sp = (void *)(((INT) &ss) - SIGSTACK_MERGIN);
		ss.ss_onstack = 1;
		sigstack(&ss, 0);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
		struct sigaltstack	ss;

		ss.ss_sp = (void *)(((INT) &ss) - SIGSTACK_MERGIN - SIGSTKSZ);
		ss.ss_size = SIGSTKSZ;
		ss.ss_flags |= SA_ONSTACK;
		sigaltstack(&ss, 0);
#endif /* USE_SIGALTSTACK */
	}

	/*
	 *  �f�B�X�p�b�`�p�̃V�O�i���n���h����ݒ�D
	 */
	{
		struct sigvec	vec;

		vec.sv_handler = dispatch_handler;
		vec.sv_mask = 0;
		vec.sv_flags = SV_INTERRUPT;
		sigvec(SIGUSR2, &vec, 0);
	}
}

/*
 *  �^�[�Q�b�gCPU�ˑ��̏I���������[�`��
 */
void
cpu_shutdown(void)
{
}

/*
 *  �^�X�N�N�����[�`��
 *
 *  BSD UNIX��ł́C�J�[�l���ƃ^�X�N�������������x���œ��삷�邽�߁C�^
 *  �X�N�ւ̕���͊֐��Ăяo���Ŏ����ł���D
 */
void
task_start()
{
	/*
	 *  �V�O�i���}�X�N��ݒ肵�āC�^�X�N���N������D
	 */
	sigsetmask(SIGMASK_TASK);
	(*ctxtsk->task)(ctxtsk->tskctxb.stacd, ctxtsk->exinf);

	/*
	 *  �^�X�N�̏I������ ext_tsk ���ĂԂ̂ŁC�����ւ͖߂�Ȃ��D
	 */
	assert(0);
}

/*
 *  �V�X�e���������v�[�����g��Ȃ��ꍇ
 *
 *  UNIX���C�u������ malloc �� free �ɃV�X�e���������v�[���̖�����C��
 *  ��D
 */

#ifndef USE_TMPL_OS

#include <malloc.h>

ER
sys_cre_mpl(void)
{
	return(E_OK);
}

VP
sys_get_blk(INT blksz)
{
	return(malloc(blksz));
}

void
sys_rel_blk(VP blk)
{
	free(blk);
}

#endif /* USE_TMPL_OS */

/*
 *  �g��SVC �̏o�����̏���
 *
 *  �g��SVC ���ŃV�X�e���I�u�W�F�N�g���A�N�Z�X�ł���悤�ɂ��邽�߂̏����D
 */

void
enter_extended_svc()
{
	ctxtsk->sysmode++;
}

void
exit_extended_svc()
{
	ctxtsk->sysmode--;
}
