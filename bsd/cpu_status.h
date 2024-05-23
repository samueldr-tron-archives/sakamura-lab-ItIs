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
 *  @(#) $Id: cpu_status.h,v 1.6 1996/02/17 09:23:35 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  BSD UNIX �ˑ���`
 */

/* 
 *  BSD UNIX �W���C���N���[�h�t�@�C��
 */
#include <signal.h>
#include <setjmp.h>

/*
 *  �V�O�i���}�X�N�̒�`
 *
 *  �^�X�N�f�B�X�p�b�`���N���p�� SIGUSR2 ���g���D
 *
 *  �ʏ�̃^�X�N�����s���̓V�O�i���}�X�N�� 0 (���ׂẴV�O�i���}�X�N�� 
 *  �������ꂽ���) �ɂȂ��Ă���D�f�B�X�p�b�`�֎~��Ԃł� SIGUSR2 �� 
 *  �}�X�N���C�����݂���уf�B�X�p�b�`�֎~��Ԃł͂��ׂẴV�O�i�����}
 *  �X�N����D�^�X�N�Ɨ������s���́C���Ȃ��Ƃ� SIGUSR2 ���}�X�N���Ȃ�
 *  ��΂Ȃ�Ȃ��D
 */
#define SIGMASK_TASK	(0)
#define SIGMASK_DDSP	(sigmask(SIGUSR2))
#define SIGMASK_LOC	(~0)
#define SIGMASK_INDP	(sigmask(SIGUSR2))

/*
 *  ���ݎg�p���̃X�^�b�N�𒲂ׂ�
 *
 *  �^�X�N�Ɨ������s���́C�V�O�i���X�^�b�N���g�����Ƃɂ��āC�V�O�i���X
 *  �^�b�N���g���Ă��邩�ǂ����ŁC�^�X�N�Ɨ����𔻕ʂ���D
 */
Inline int
current_stack()
{
#ifdef USE_SIGSTACK
	struct sigstack	ss;

	sigstack(0, &ss);
	return(ss.ss_onstack);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
	struct sigaltstack	ss;

	sigaltstack(0, &ss);
	return(ss.ss_flags & SA_ONSTACK);
#endif /* USE_SIGALTSTACK */
}

/*
 *  �N���e�B�J���Z�N�V������`�p�}�N��
 */

#define BEGIN_CRITICAL_SECTION	{ int mask = sigblock(SIGMASK_LOC);
#define END_CRITICAL_SECTION	sigsetmask(mask); }

#define DISABLE_INTERRUPT	{ sigblock(SIGMASK_LOC); }

/*
 *  �V�X�e����Ԕ��ʗp�}�N��
 */

/*
 *  in_indp(): �V�X�e���R�[�����C�^�X�N�Ɨ�������Ă΂ꂽ���𔻕ʂ��邽
 *  �߂̃}�N���D
 *
 *  �V�O�i���X�^�b�N���g���Ă��邩�ǂ����Ŕ��f����D
 */
#define in_indp()	(current_stack())

/*
 *  in_ddsp(): �V�X�e���R�[�����f�B�X�p�b�`�֎~���ɌĂ΂ꂽ���𔻕ʂ���
 *  ���߂̃}�N���D�^�X�N�Ɨ������C�f�B�X�p�b�`�֎~���Ɋ܂ށD
 *
 *  SIGUSR2 ���}�X�N����Ă��邩�ǂ����Ŕ��f����D
 */
#define in_ddsp()	(sigblock(0) & SIGMASK_DDSP)

/*
 *  in_loc(): �V�X�e���R�[���� CPU���b�N���ɌĂ΂ꂽ���𔻕ʂ��邽�߂�
 *  �}�N���D�^�X�N�Ɨ������CCPU���b�N���Ɋ܂ށD
 *
 *  SIGUSR2 �ȊO�̂����ꂩ�̃V�O�i�����}�X�N����Ă���ꍇ�ɁCCPU���b
 *  �N���ł���Ɣ��f����D�^�X�N�Ɨ����ł́CSIGUSR2 �ȊO�̂����ꂩ�̃V
 *  �O�i�����}�X�N����Ă���͂��D
 */
#define in_loc()	(sigblock(0) & ~SIGMASK_DDSP)

/*
 *  in_sysmode(): �V�X�e���R�[�����V�X�e�����\�[�X���A�N�Z�X�ł����Ԃ�
 *  ��Ă΂ꂽ���𔻕ʂ��邽�߂̃}�N���DE_OACV�G���[�̃`�F�b�N�ɗp����D
 *
 *  ctxtsk �� sysmode�t�B�[���h�����Ĕ��ʂ���K�v������D
 */
#define	in_sysmode()	(in_indp() || ctxtsk->sysmode)

/*
 *  in_qtsk(): �V�X�e���R�[�����C���^�X�N�������s������Ă΂ꂽ���𔻕�
 *  ���邽�߂̃}�N���D�^�X�N�Ɨ����Ƃ͋�ʂ��Ȃ��̂ŁCin_indp() �� FALSE 
 *  �̎��ɂ̂ݎg�����ƁD
 *
 *  ���̔��ʂ��\�ɂ��邽�߂ɁCTCB ���� isysmode�t�B�[���h��p�ӂ���D
 *  USE_QTSK_PORTION ����`����Ă��Ȃ��ꍇ�ɂ́C���^�X�N���͎g���Ȃ�
 *  �̂ŁC��� FALSE �Ƃ���D
 */
#ifdef USE_QTSK_PORTION
#define in_qtsk()	(ctxtsk->sysmode > ctxtsk->isysmode)
#else /* USE_QTSK_PORTION */
#define in_qtsk()	0
#endif /* USE_QTSK_PORTION */

/*
 *  �^�X�N�f�B�X�p�b�`���N�����[�`��
 */

/*
 *  �^�X�N�f�B�X�p�b�`���̋N���v�����o���D
 */
Inline void
dispatch_request(void)
{
	kill(getpid(), SIGUSR2);
}

/*
 *  ���݂̃^�X�N�̃R���e�L�X�g���̂ĂāC���Ɏ��s���ׂ��^�X�N�֋����I��
 *  �f�B�X�p�b�`����D
 *
 *  �V�X�e���N��������� ext_tsk, ext_tsk �̏����ŗp����Dctxtsk ���O
 *  ���Q�ƂɂȂ邽�߁Cforce_dispatch �{�̂́C�}�N���Ƃ��Ē�`����D
 */
Inline void
_force_dispatch(void)
{
#ifdef USE_SIGSTACK
	struct sigstack	ss;

	sigstack(0, &ss);
	ss.ss_onstack = 0;
	sigstack(&ss, 0);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
	struct sigaltstack	ss;

	sigaltstack(0, &ss);
	ss.ss_flags &= ~SA_ONSTACK;
	sigaltstack(&ss, 0);
#endif /* USE_SIGALTSTACK */
	kill(getpid(), SIGUSR2);
	sigsetmask(SIGMASK_TASK);
}

#define	force_dispatch() \
{					\
	ctxtsk = 0;			\
	_force_dispatch();		\
}

/*
 *  �^�X�N�R���e�L�X�g�u���b�N�̒�`
 */
typedef struct {
	INT	stacd;		/* �^�X�N�N���R�[�h */
	jmp_buf	env;		/* CPU�R���e�L�X�g */
} CTXB;

#endif /* _CPU_STATUS_ */
