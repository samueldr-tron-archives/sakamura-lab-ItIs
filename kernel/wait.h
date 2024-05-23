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
 *  @(#) $Id: wait.h,v 1.10 1997/01/10 13:15:29 hiro Exp $
 */

#ifndef _WAIT_
#define _WAIT_

/*
 *  �^�X�N�ԓ����E�ʐM�I�u�W�F�N�g�ėp���[�`��
 */

#include "queue.h"
#include "timer.h"
#include "task.h"

/*
 *  �^�X�N�̑҂���Ԃ���������D
 *
 *  �^�X�N���^�C�}�L���[����ё҂��L���[����͂����C�^�X�N��Ԃ��X�V��
 *  ��Dwait_release_ok �́C�҂��������ꂽ�^�X�N�� E_OK ��n���D
 *  wait_release_tmout �́C�^�C�}�L���[����͂������������Ȃ��D�^�C��
 *  �A�E�g�����̎��ɗp����D
 */
extern void	wait_release(TCB *tcb);
extern void	wait_release_ok(TCB *tcb);
extern void	wait_release_tmout(TCB *tcb);

/*
 *  �^�X�N�̑҂���Ԃ��L�����Z������D
 *
 *  �^�X�N���^�C�}�L���[����ё҂��L���[����͂����D�^�X�N��Ԃ͍X�V��
 *  �Ȃ��D
 */
Inline void
wait_cancel(TCB *tcb)
{
	timer_delete(&(tcb->wtmeb));
	queue_delete(&(tcb->tskque));
}

/*
 *  ���s���̃^�X�N��҂���ԂɈڍs�����C�^�C�}�C�x���g�L���[�ɂȂ��D
 */
extern void	make_wait(TMO tmout);

/*
 *  �҂��L���[�ɂȂ����Ă���^�X�N�̑҂���Ԃ����ׂĉ������CE_DLT�G���[
 *  �Ƃ���D
 *
 *  �^�X�N�ԓ����E�ʐM�I�u�W�F�N�g���폜���ꂽ���Ɏg���D
 */
extern void	wait_delete(QUEUE *wait_queue);

/*
 *  �҂��L���[�̐擪�̃^�X�N�� ID �����o���D
 */
extern ID	wait_tskid(QUEUE *wait_queue);

/*
 *  �R���g���[���u���b�N���ʕ������샋�[�`��
 *
 *  �^�X�N�ԓ����E�ʐM�I�u�W�F�N�g�͂�������C�R���g���[���u���b�N�̐�
 *  �����������ʂɂȂ��Ă���D�ȉ��́C���̋��ʕ������������߂̔ėp���[
 *  �`���ł���D���ʕ������CGCB (�ėp�R���g���[���u���b�N) �Ƃ����^�ɂ�
 *  ��D�����̑҂��L���[�����I�u�W�F�N�g�̏ꍇ�C2�߈ȍ~�̑҂��L���[
 *  �𑀍삷��ꍇ�ɂ́C�����̃��[�`���͎g���Ȃ��D�܂��C�I�u�W�F�N�g
 *  ������ TA_TPRI�r�b�g���Q�Ƃ���̂ŁC���̃r�b�g�𑼂̖ړI�Ɏg���Ă�
 *  ��ꍇ���C�����̃��[�`���͎g���Ȃ��D�I�u�W�F�N�g����������Ă���
 *  ���ꍇ�ɂ́C�I�u�W�F�N�g������ OBJ_NONEXIST �ɂ��� (���̂��߁C�I�u
 *  �W�F�N�g�����̂��ׂẴr�b�g��Ɨ��Ɏg�����Ƃ��ł��Ȃ��Ƃ���������
 *  ����)�D
 */

struct generic_control_block {
	QUEUE	wait_queue;	/* �҂��L���[ */
	ID	objid;		/* �I�u�W�F�N�gID */
	VP	exinf;		/* �g����� */
	ATR	objatr;		/* �I�u�W�F�N�g���� */
	/*  ����ȍ~�ɑ��̃t�B�[���h�������Ă��悢���C */
	/*  �ėp���샋�[�`���ł͈����Ȃ��D */
};

#define OBJ_NONEXIST	(-1)

/*
 *  ���s���̃^�X�N��҂���ԂɈڍs�����C�^�C�}�C�x���g�L���[����уI�u
 *  �W�F�N�g�̑҂��L���[�ɂȂ��D�܂��Cctxtsk �� wid ��ݒ肷��D
 */
extern void	gcb_make_wait(GCB *gcb, TMO tmout);

/*
 *  �^�X�N�̗D��x���ς�����ۂɁC�҂��L���[�̒��ł̃^�X�N�̈ʒu���C��
 *  ����D�I�u�W�F�N�g������ TA_TPRI ���w�肳��Ă���ꍇ�ɂ̂݁C�Ăяo
 *  �����D
 */
extern void	gcb_change_priority(GCB *gcb, TCB *tcb);
extern void	obj_chg_pri(TCB *tcb, INT oldpri);

#endif /* _WAIT_ */
