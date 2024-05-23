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
 *  @(#) $Id: itron_errno.h,v 1.5 1996/02/17 09:26:52 hiro Exp $
 */

#ifndef _ITRON_ERRNO_
#define _ITRON_ERRNO_

/* 
 *  ITRON �̃G���[�R�[�h�̒�`
 */

#define	E_OK		0		/* ����I�� */

#define E_SYS		(-5)		/* �V�X�e���G���[ */
#define	E_NOMEM		(-10)		/* �������s�� */
#define	E_NOSPT		(-17)		/* ���T�|�[�g�@�\ */
#define	E_INOSPT	(-18)		/* ITRON/FILE �ł̖��T�|�[�g�@�\ */
#define	E_RSFN		(-20)		/* �\��@�\�ԍ� */
#define	E_RSATR		(-24)		/* �\�񑮐� */
#define	E_PAR		(-33)		/* �p�����^�[�G���[ */
#define	E_ID		(-35)		/* �s��ID�ԍ� */
#define	E_NOEXS		(-52)		/* �I�u�W�F�N�g�����݂��Ă��Ȃ� */
#define	E_OBJ		(-63)		/* �I�u�W�F�N�g�̏�Ԃ��s�� */
#define	E_MACV		(-65)		/* �������A�N�Z�X�s�\/�A�N�Z�X���ᔽ */
#define	E_OACV		(-66)		/* �I�u�W�F�N�g�A�N�Z�X���ᔽ */
#define	E_CTX		(-69)		/* �R���e�L�X�g�G���[ */
#define	E_QOVR		(-73)		/* �L���[�C���O�̃I�[�o�[�t���[ */
#define	E_DLT		(-81)		/* �҂��I�u�W�F�N�g���폜���ꂽ */
#define	E_TMOUT		(-85)		/* �|�[�����O���s/�^�C���A�E�g */
#define	E_RLWAI		(-86)		/* �҂���ԋ������� */

#define	EN_NOND		(-113)		/* �Ώۃm�[�h�����݂��Ȃ� */
#define	EN_OBJNO	(-114)		/* �A�N�Z�X�ł��Ȃ��I�u�W�F�N�g�ԍ� */
#define	EN_PROTO	(-115)		/* ���T�|�[�g�v���g�R�� */
#define	EN_RSFN		(-116)		/* ���T�|�[�g�V�X�e���R�[�� */
#define	EN_COMM		(-117)		/* �Ώۃm�[�h����̉������Ȃ� */
#define	EN_RLWAI	(-118)		/* �ڑ��@�\�Ɋւ��҂���ԋ������� */
#define	EN_PAR		(-119)		/* �v�����̃p�����[�^�G���[ */
#define	EN_RPAR		(-120)		/* ���^�[�����̃p�����[�^�G���[ */
#define	EN_CTXID	(-121)		/* �ڑ��@�\���g���Ȃ��R���e�L�X�g */
#define	EN_EXEC		(-122)		/* �Ώۃm�[�h�ɂ����鎑���s�� */
#define	EN_NOSPT	(-123)		/* ���T�|�[�g�ڑ��@�\ */

#define EV_FULL		(-225)		/* �I�u�W�F�N�g�e�[�u���t�� */

#endif /* _ITRON_ERRNO_ */
