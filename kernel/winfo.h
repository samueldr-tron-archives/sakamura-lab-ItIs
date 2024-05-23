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
 *  @(#) $Id: winfo.h,v 1.9 1997/01/10 13:36:23 hiro Exp $
 */

#ifndef _WINFO_
#define _WINFO_

/*
 *  �����E�ʐM�I�u�W�F�N�g���ɕK�v�ȑ҂����̒�`
 */

/*
 *  �C�x���g�t���O�҂� (TTW_FLG)
 */
typedef struct {
	UINT	waiptn;		/* �҂��r�b�g�p�^�[�� */
	UINT	wfmode;		/* �҂����[�h */
	UINT	*p_flgptn;	/* �҂��������r�b�g�p�^�[��������A�h���X */
} WINFO_FLG;

/*
 *  ���C���{�b�N�X�҂� (TTW_MBX)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* ���b�Z�[�W�p�P�b�g�̐擪������A�h���X */
} WINFO_MBX;

/*
 *  ���b�Z�[�W�o�b�t�@��M/���M�҂� (TTW_MBF, TTW_SMBF)
 */
typedef struct {
	VP	msg;		/* ��M���b�Z�[�W������A�h���X */
	INT	*p_msgsz;	/* ��M���b�Z�[�W�̃T�C�Y������A�h���X */
} WINFO_MBF;

typedef struct {
	VP	msg;		/* ���M���b�Z�[�W�̐擪�A�h���X */
	INT	msgsz;		/* ���M���b�Z�[�W�̃T�C�Y */
} WINFO_SMBF;

/*
 *  �����f�u�ďo/��t/�I���҂� (TTW_CAL, TTW_ACP, TTW_RDV)
 */
typedef struct {
	UINT	calptn;		/* �ďo���I��������\���r�b�g�p�^�[�� */
	VP	msg;		/* ���b�Z�[�W������A�h���X */
	INT	cmsgsz;		/* �ďo���b�Z�[�W�̃T�C�Y */
	INT	*p_rmsgsz;	/* �ԓ����b�Z�[�W�̃T�C�Y������A�h���X */
} WINFO_CAL;

typedef struct {
	UINT	acpptn;		/* ��t���I��������\���r�b�g�p�^�[�� */
	VP	msg;		/* �ďo���b�Z�[�W������A�h���X */
	RNO	*p_rdvno;	/* �����f�u�ԍ�������A�h���X */
	INT	*p_cmsgsz;	/* �ďo���b�Z�[�W�̃T�C�Y������A�h���X */
} WINFO_ACP;

typedef struct {
	RNO	rdvno;		/* �����f�u�ԍ� */
	VP	msg;		/* ���b�Z�[�W������A�h���X */
	INT	maxrmsz;	/* �ԓ����b�Z�[�W�̍ő咷 */
	INT	*p_rmsgsz;	/* �ԓ����b�Z�[�W�̃T�C�Y������A�h���X */
} WINFO_RDV;

/*
 *  �^�X�N�t�����C���{�b�N�X�҂� (TTW_TMB)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* ���b�Z�[�W�p�P�b�g�̐擪������A�h���X */
} WINFO_TMB;

/*
 *  �ϒ��������v�[���҂� (TTW_MPL)
 */
typedef struct {
	INT	blksz;		/* �������u���b�N�T�C�Y */
	VP	*p_blk;		/* �������u���b�N�̐擪������A�h���X */
} WINFO_MPL;

/*
 *  �Œ蒷�������v�[���҂� (TTW_MPF)
 */
typedef struct {
	VP	*p_blf;		/* �������u���b�N�̐擪������A�h���X */
} WINFO_MPF;

/*
 *  �^�X�N�R���g���[���u���b�N���Ɏ��҂����̒�`
 */
typedef union {
#ifdef USE_FLG
	WINFO_FLG	flg;
#endif /* USE_FLG */
#ifdef USE_MBX
	WINFO_MBX	mbx;
#endif /* USE_MBX */
#ifdef USE_MBF
	WINFO_MBF	mbf;
	WINFO_SMBF	smbf;
#endif /* USE_MBF */
#ifdef USE_POR
	WINFO_CAL	cal;
	WINFO_ACP	acp;
	WINFO_RDV	rdv;
#endif /* USE_POR */
#ifdef USE_TASK_MAILBOX
	WINFO_TMB	tmb;
#endif /* USE_TASK_MAILBOX */
#ifdef USE_MPL
	WINFO_MPL	mpl;
#endif /* USE_MPL */
#ifdef USE_MPF
	WINFO_MPF	mpf;
#endif /* USE_MPF */
} WINFO;

/*
 *  �҂��d�l�\���̂̒�`
 */

typedef const struct {
	UINT	tskwait;			/* �҂��v�� */
	void	(*chg_pri_hook)(TCB *, INT);	/* �^�X�N�D��x�ύX���̏��� */
	void	(*rel_wai_hook)(TCB *);		/* �^�X�N�҂��������̏��� */
} WSPEC;

#endif /* _WINFO_ */
