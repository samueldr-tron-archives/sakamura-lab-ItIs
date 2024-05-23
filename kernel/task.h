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
 *  @(#) $Id: task.h,v 1.18 1997/01/10 13:36:31 hiro Exp $
 */

#ifndef _TASK_
#define _TASK_

typedef struct task_control_block	TCB;
typedef struct generic_control_block	GCB;
typedef struct prisem_control_block	PISCB;

#include "queue.h"
#include "timer.h"
#include "winfo.h"

/*
 *  �^�X�N��Ԃ̓����\���̒�`
 *
 *  �^�X�N���҂���Ԃ��ǂ����� (state & TS_WAIT) �Ń`�F�b�N�ł���.
 *  �^�X�N�������҂���Ԃ��ǂ����� (state & TS_SUSPEND) �Ń`�F�b�N�ł���.
 */
typedef enum {
	TS_NONEXIST = 0,	/* ���o�^��� */
	TS_READY = 1,		/* ���s�܂��͎��s�\��� */
	TS_WAIT = 2,		/* �҂���� */
	TS_SUSPEND = 4,		/* �����҂���� */
	TS_WAITSUS = 6,		/* ��d�҂���� */
	TS_DORMANT = 8		/* �x�~��� */
} TSTAT;

/*
 *  �^�X�N�������Ă��邩 (NON-EXISTENT�CDORMANT�ȊO��) �̃`�F�b�N
 */
Inline BOOL
task_alive(TSTAT state)
{
	return(state & (TS_READY|TS_WAIT|TS_SUSPEND));
}

/*
 *  �^�X�N�D��x�̓���/�O���\���ϊ��}�N��
 */
#define int_priority(x)	((INT)((x) - MIN_PRI))
#define ext_tskpri(x)	((PRI)((x) + MIN_PRI))

/*
 *  �^�X�N�R���g���[���u���b�N (TCB) �̒�`
 */
struct task_control_block {
	QUEUE	tskque;		/* �^�X�N�L���[ */
	ID	tskid;		/* �^�X�NID */
	VP	exinf;		/* �g����� */
	ATR	tskatr;		/* �^�X�N���� */
	FP	task;		/* �^�X�N�N���A�h���X */
	INT	ipriority;	/* �^�X�N�N�����D��x */
#ifdef USE_SEPARATE_SSTACK
	INT	stksz;		/* ���[�U�X�^�b�N�T�C�Y */
#endif /* USE_SEPARATE_SSTACK */
	INT	sstksz;		/* �V�X�e���X�^�b�N�T�C�Y */
	INT	priority;	/* ���݂̗D��x */
	TSTAT	state;		/* �^�X�N��� (�����\��) */
	WSPEC	*wspec;		/* �҂��d�l */
	GCB	*wgcb;		/* �҂��I�u�W�F�N�g�̃R���g���[���u���b�N */
	INT	wupcnt;		/* �N���v���L���[�C���O�� */
	INT	suscnt;		/* SUSPEND�v���l�X�g�� */
	INT	sysmode;	/* �^�X�N���샂�[�h�C���^�X�N���ďo�����x�� */
#ifdef USE_QTSK_PORTION
	INT	isysmode;	/* �^�X�N���샂�[�h�����l */
#endif /* USE_QTSK_PORTION */

#ifdef USE_POR
	RNO	wrdvno;		/* �����f�u�ԍ������p */
#endif /* USE_POR */
#ifdef PRISEM_SPEC1
	PISCB	*pislist;	/* �l���D��x�p���Z�}�t�H���X�g */
#endif /* PRISEM_SPEC1 */
#ifdef USE_TASK_MAILBOX
	T_MSG	*tmq_head;	/* �^�X�N�t�����b�Z�[�W�L���[�̐擪 */
	T_MSG	*tmq_tail;	/* �^�X�N�t�����b�Z�[�W�L���[�̖��� */
#endif /* USE_TASK_MAILBOX */
	ER	*wercd;		/* �҂��G���[�R�[�h�ݒ�G���A */
	WINFO	winfo;		/* �҂���� */
	TMEB	wtmeb;		/* �҂��^�C�}�C�x���g�u���b�N */

#ifdef USE_SEPARATE_SSTACK
	VP	istack;		/* ���[�U�X�^�b�N�|�C���^�̏����l */
#endif /* USE_SEPARATE_SSTACK */
	VP	isstack;	/* �V�X�e���X�^�b�N�|�C���^�̏����l */
	CTXB	tskctxb;	/* �^�X�N�R���e�L�X�g�u���b�N */
};

/*
 *  �^�X�N�f�B�X�p�b�`�֎~���
 *
 *  dispatch_disabled �́C�^�X�N�f�B�X�p�b�`�֎~��Ԃ��L�����Ă�������
 *  �̕ϐ��D�x�������݂��g���ꍇ�Ȃǂɂ́C�K�v�Ȃ��D
 */
#ifdef USE_DISPATCH_DISABLED
extern BOOL	dispatch_disabled;
#endif /* USE_DISPATCH_DISABLED */

/*
 *  ���s���̃^�X�N
 *
 *  ctxtsk �́C���s���̃^�X�N (= CPU ���R���e�L�X�g�������Ă���^�X�N) 
 *  �� TCB ���w���ϐ��D�V�X�e���R�[���̏������ŁC�V�X�e���R�[����v����
 *  ���^�X�N�Ɋւ�������Q�Ƃ���ꍇ�́Cctxtsk ���g���Dctxtsk ������
 *  ������̂́C�^�X�N�f�B�X�p�b�`���̂݁D
 */
extern TCB	*ctxtsk;

/*
 *  ���s���ׂ��^�X�N
 *
 *  schedtsk �́C���s���ׂ��^�X�N�� TCB ���w���ϐ��D�x���f�B�X�p�b�`��
 *  �f�B�X�p�b�`�̋֎~�ɂ��f�B�X�p�b�`���x������Ă����Ԃł́C
 *  ctxtsk �ƈ�v���Ȃ��D
 */
extern TCB	*schedtsk;

/*
 *  TCB �̃G���A
 *
 *  TCB �̃G���A�́C�V�X�e���������ɐÓI�Ɋ���t���Ă���D
 */
extern TCB	tcb_table[];

/*
 *  ���g�p�� TCB �̃��X�g
 */
#ifndef _i_vcre_tsk
extern QUEUE	free_tcb;
#endif /* _i_vcre_tsk */

/*
 *  �^�X�NID ���� TCB �����o���D
 */
#define get_tcb(id)	 (&tcb_table[INDEX_TSK(id)])
#define get_tcb_self(id) ((id)==TSK_SELF ? ctxtsk : &tcb_table[INDEX_TSK(id)])

/*
 *  �^�X�N�̎��s����������D
 */
extern void	make_dormant(TCB *tcb);

/*
 *  �^�X�N�����s�\��Ԃɂ���D
 *
 *  tcb �Ŏ������^�X�N�̗D��x���C���ݎ��s���̃^�X�N�̗D��x������
 *  ���ꍇ�́C���s��Ԃɂ���D�����łȂ��ꍇ�́C�^�X�N�����f�B�L���[��
 *  �Ȃ��D
 */
extern void	make_ready(TCB *tcb);

/*
 *  �^�X�N�����s�\�ȊO�̏�Ԃɂ���D
 *
 *  tcb �Ŏ������^�X�N�����s�\�ȊO�̏�ԁC�܂�҂���ԁC�����҂�
 *  ��ԁC�x�~��Ԃֈڍs������D���̊֐����Ăяo���ۂɂ́C�^�X�N�����s
 *  �\��ԂɂȂ��Ă��邱�ƁDtcb->state �́C�Ăяo�������ŁC���̊֐���
 *  �烊�^�[����ɕύX����D
 */
extern void	make_non_ready(TCB *tcb);

/*
 *  �^�X�N�̗D��x��ύX����D
 *
 *  tcb �Ŏ������^�X�N�̗D��x�� priority �ɕύX����D����ɔ����ĕK
 *  �v�ƂȂ�^�X�N�̏�ԑJ�ڂ��N��������D
 */
extern void	change_task_priority(TCB *tcb, INT priority);

/*
 *  ���f�B�L���[����]������D
 *
 *  rotate_ready_queue �́Cpriority �Ŏw�肳�ꂽ�D��x�̃��f�B�L���[��
 *  ��]������Drotate_ready_queue_run �́C���f�B�L���[���̍ō��D��x
 *  �̃^�X�N���܂ރ��f�B�L���[����]������D
 */
extern void	rotate_ready_queue(INT priority);
extern void	rotate_ready_queue_run(void);

#endif /* _TASK_ */
