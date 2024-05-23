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
 *  @(#) $Id: cpu_conf.h,v 1.9 1998/01/30 06:59:08 hiro Exp $
 */

#ifndef _CPU_CONF_
#define _CPU_CONF_

/*
 *  get_ver �ŎQ�Ƃ���� CPU�R�[�h�̒�`
 */
#define	CPU_CODE	0x0000		/* CPU ����肵�Ȃ� */

/*
 *  CPU�ˑ��̃V�X�e���R�[����/�p�����[�^���̐ݒ�
 */
#define chg_iXX	chg_ims
#define ref_iXX	ref_ims
#define iXXXX	imask
#define p_iXXXX	p_imask

/*
 *  BSD UNIX��ł̓T�|�[�g����Ȃ��@�\�̒�`
 */
#undef NUM_SVC				/* �g��SVC�n���h�� */
#undef USE_QTSK_PORTION			/* ���^�X�N�� */
#undef USE_VGET_TIM			/* ���\�]���p�V�X�e�������Q�Ƌ@�\ */

#define _i_dis_int	_no_support	/* dis_int�V�X�e���R�[�� */
#define _i_ena_int	_no_support	/* ena_int�V�X�e���R�[�� */
#define _i_def_int	_no_support	/* def_int�V�X�e���R�[�� */
#define _i_set_reg	_no_support	/* set_reg�V�X�e���R�[�� */
#define _i_get_reg	_no_support	/* get_reg�V�X�e���R�[�� */

/*
 *  �V�X�e���X�^�b�N�̍ŏ��T�C�Y�̒�`
 *
 *  cre_tsk �Ń^�X�N���ɃV�X�e���X�^�b�N�T�C�Y��ݒ肷��ꍇ�̍ŏ��̃V
 *  �X�e���X�^�b�N�T�C�Y�D
 */
#ifndef MIN_SYS_STACK_SIZE
#define	MIN_SYS_STACK_SIZE	8192
#endif /* MIN_SYS_STACK_SIZE */

/*
 *  �f�t�H���g�V�X�e���X�^�b�N�T�C�Y�̒�`
 *
 *  cre_tsk �Ń^�X�N���ɃV�X�e���X�^�b�N�T�C�Y��ݒ肵�Ȃ��ꍇ�̃f�t�H
 *  ���g�̃V�X�e���X�^�b�N�T�C�Y�D
 */
#ifndef DEF_SYS_STACK_SIZE
#define DEF_SYS_STACK_SIZE	8192
#endif /* DEF_SYS_STACK_SIZE */

/*
 *  �g��SVC �̏o�����̏���
 */
#define ENTER_EXTENDED_SVC	enter_extended_svc()
#define EXIT_EXTENDED_SVC	exit_extended_svc()

/*
 *  �V�X�e���^�X�N�Ɋւ����`
 */
#define	CONSOLE_PORT	1	/* �R���\�[���p�ɗp����V���A���|�[�g�ԍ� */
#define	LOGTASK_PORT	1	/* �V�X�e�����O���o�͂���V���A���|�[�g�ԍ� */

#endif /* _CPU_CONF_ */
