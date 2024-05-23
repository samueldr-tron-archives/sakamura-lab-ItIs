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
 *  @(#) $Id: cpu_status.h,v 1.5 1998/01/30 06:48:12 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  TRON�d�l�`�b�v�ˑ���`
 */

/*
 *  TRON�d�l�`�b�v �჌�x����`
 */
#include "tron_defs.h"
#include "tron_insn.h"

/*
 *  �N���e�B�J���Z�N�V������`�p�}�N��
 *
 *  �x�������݂��g��Ȃ��ꍇ�ɂ́C�N���e�B�J���Z�N�V�����I�����ɁC�f�B
 *  �X�p�b�`���K�v���ǂ������`�F�b�N���C�K�v�Ȃ�f�B�X�p�b�`�����N����
 *  ��K�v������D
 */

#ifndef TRON_NO_DI

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask); }

#else /* TRON_NO_DI */

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask);			\
				if (imask_psw(intmask) == 15		\
					    && !dispatch_disabled	\
					    && ctxtsk != schedtsk) {	\
					dispatch();			\
				} }
#endif /* TRON_NO_DI */

#define DISABLE_INTERRUPT	{ disint(); }

/*
 *  �V�X�e����Ԕ��ʗp�}�N��
 */

/*
 *  in_indp(): �V�X�e���R�[�����C�^�X�N�Ɨ�������Ă΂ꂽ���𔻕ʂ��邽
 *  �߂̃}�N���D
 *
 *  �V�X�e���R�[���ďo���� TRAPA �ł́CPSW���̃X�^�b�N���[�h�͕ω�����
 *  ���̂ŁCin_indp ���Ă΂ꂽ���_�̃X�^�b�N���[�h�� 0 �Ȃ�^�X�N�Ɨ���
 *  ����Ă΂ꂽ�Ɣ��ʂł���D
 */
#define in_indp()	((current_psw() & 0x80000000) == 0)

/*
 *  in_ddsp(): �V�X�e���R�[�����f�B�X�p�b�`�֎~���ɌĂ΂ꂽ���𔻕ʂ���
 *  ���߂̃}�N���D�^�X�N�Ɨ������C�f�B�X�p�b�`�֎~���Ɋ܂ށD
 *
 *  �V�X�e���R�[���Ăяo���� TRAPA �ł� IMASK �͕ω����Ȃ��̂ŁCin_ddsp 
 *  ���Ă΂ꂽ���_�� IMASK �� 15 �����Ȃ�C�f�B�X�p�b�`�֎~���ɌĂ΂ꂽ
 *  �Ɣ��ʂł���D�^�X�N�Ɨ����� IMASK �� 15 �ɂȂ�P�[�X�͍l���Ȃ��D
 *
 *  �x�������݂��g��Ȃ��ꍇ�ɂ́CIMASK = 14 �ɑ��������Ԃ��C
 *  dispatch_disabled �� TRUE �ɂ��邱�ƂŋL������邽�߁C
 *  dispatch_disabled �̃`�F�b�N���K�v�ł���D
 */
#ifndef TRON_NO_DI
#define in_ddsp()	(current_imask() < 15)
#else /* TRON_NO_DI */
#define in_ddsp()	(current_imask() < 15 || dispatch_disabled)
#endif /* TRON_NO_DI */

/*
 *  in_loc(): �V�X�e���R�[���� CPU���b�N���ɌĂ΂ꂽ���𔻕ʂ��邽�߂�
 *  �}�N���D�^�X�N�Ɨ������CCPU���b�N���Ɋ܂ށD
 *
 *  �V�X�e���R�[���Ăяo���� TRAPA �ł� IMASK �͕ω����Ȃ��̂ŁCin_loc 
 *  ���Ă΂ꂽ���_�� IMASK �� 14 �����Ȃ�CCPU���b�N���ɌĂ΂ꂽ�ƍl��
 *  ��D���ۂɂ́Cloc_cpu �V�X�e���R�[�����g���ƁCIMASK �� 0 �ɂȂ�D�^
 *  �X�N�Ɨ����� IMASK �� 14 �Ȃ����� 15 �ɂȂ�P�[�X�͍l���Ȃ��D
 *
 *  �x�������݂��g��Ȃ��ꍇ�ɂ́CIMASK �� 14 �ɂȂ邱�Ƃ͂Ȃ��C15 ����
 *  ���ǂ����Ń`�F�b�N����D
 */
#ifndef TRON_NO_DI
#define in_loc()	(current_imask() < 14)
#else /* TRON_NO_DI */
#define in_loc()	(current_imask() < 15)
#endif /* TRON_NO_DI */

/*
 *  in_sysmode(): �V�X�e���R�[�����V�X�e�����\�[�X���A�N�Z�X�ł����Ԃ�
 *  ��Ă΂ꂽ���𔻕ʂ��邽�߂̃}�N���DE_OACV�G���[�̃`�F�b�N�ɗp����D
 *
 *  PSW���̃����O���x���́C�V�X�e���R�[���Ăяo���� TRAPA �ɂ��ω�����
 *  ���߁Cin_sysmode ���Ă΂ꂽ���_�� RNG �����Ĕ��ʂ��邱�Ƃ͂ł����C
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
#ifndef TRON_NO_DI
	set_dir(14);
#endif /* TRON_NO_DI */
}

/*
 *  ���݂̃^�X�N�̃R���e�L�X�g���̂ĂāC���Ɏ��s���ׂ��^�X�N�֋����I��
 *  �f�B�X�p�b�`����D
 *
 *  �V�X�e���N��������� ext_tsk, exd_tsk �̏����ŗp����D
 *
 *  �������e�Ƃ��ẮCdispatch_to_schedtsk �֕��򂷂邾���ł���D�����
 *  �̓^�X�N�̃R���e�L�X�g�͎̂Ă���̂Ŋ֐��ďo���ł���{�I�ɂ͖��
 *  �Ȃ����CSPI ���g���Ă��郂�[�h�ŌĂ񂾏ꍇ (����Ɏg���Ă���΁C�V
 *  �X�e���N�����̂�) �ɁCbra ���߂ŕ��򂵂������֐��ďo�����s������
 *  �Ԃ�Ԓn�̃X�^�b�N�̈敪�� (�킸�� 4�o�C�g�����ł͂��邪) �ߖ�ł�
 *  ��D
 */
Inline void
force_dispatch(void)
{
	Asm("bra _dispatch_to_schedtsk");
}

/*
 *  �^�X�N�f�B�X�p�b�`�����N������D
 *
 *  TRON�d�l�`�b�v�ł́C�x�������݂��g��Ȃ��ꍇ�ɂ̂ݎg����D
 */
#ifdef TRON_NO_DI
Inline void
dispatch()
{
	Asm("trapa #4" : : : "memory");
}
#endif /* TRON_NO_DI */

/*
 *  �^�X�N�R���e�L�X�g�u���b�N�̒�`
 */
typedef struct {
	VW	csw;		/* CSW (Context Status Word) */
	VP	sp0;		/* SP0 */
	VP	sp1;		/* SP1 */
	VP	sp2;		/* SP2 */
	VP	sp3;		/* SP3 */
	VW	uatb;		/* UATB */
	VW	lsid;		/* LSID */
} CTXB;

#endif /* _CPU_STATUS_ */
