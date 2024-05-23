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
 *  @(#) $Id: itron.h,v 1.10 1998/01/30 07:09:02 hiro Exp $
 */

#ifndef _ITRON_
#define _ITRON_

/*
 *  �ėp�I�ȃf�[�^�^�C�v
 */

typedef	char		B;		/* �����t�� 8�r�b�g���� */
typedef	short		H;		/* �����t�� 16�r�b�g���� */
typedef	int		W;		/* �����t�� 32�r�b�g���� */
typedef	unsigned char	UB;		/* �������� 8�r�b�g���� */
typedef	unsigned short  UH;		/* �������� 16�r�b�g���� */
typedef	unsigned int	UW;		/* �������� 32�r�b�g���� */

typedef	char		VB;		/* �^����肵�Ȃ� 8�r�b�g�̃f�[�^ */
typedef	short		VH;		/* �^����肵�Ȃ� 16�r�b�g�̃f�[�^ */
typedef	int		VW;		/* �^����肵�Ȃ� 32�r�b�g�̃f�[�^ */
typedef	void		*VP;		/* �^����肵�Ȃ��f�[�^�ւ̃|�C���^ */

typedef	void		(*FP)();	/* �v���O�����X�^�[�g�A�h���X��� */

/*
 *  ITRON �Ɉˑ������f�[�^�^�C�v
 */

typedef int		INT;		/* �����t������ */
typedef unsigned int	UINT;		/* ������������ */

typedef INT		BOOL;		/* �u�[���l */
typedef INT		FN;		/* �@�\�R�[�h */
	/* ���̒�`�́C��ITRON3.0�d�l�ɂ����Ă��Ȃ��D*/
typedef	INT		ID;		/* �I�u�W�F�N�g��ID�ԍ� */
typedef	INT		BOOL_ID;	/* �u�[���l�܂��� ID�ԍ� */
typedef	INT		HNO;		/* �n���h���ԍ� */
typedef	INT		RNO;		/* �����f�u�ԍ� */
typedef	INT		NODE;		/* �m�[�h�ԍ� */
typedef	UINT		ATR;		/* �I�u�W�F�N�g/�n���h������ */
typedef	INT		ER;		/* �G���[�R�[�h */
typedef	INT		PRI;		/* �^�X�N�D��x */
typedef	INT		TMO;		/* �^�C���A�E�g�w�� */
typedef long long	SYSTIME;	/* �V�X�e���N���b�N */
typedef SYSTIME		CYCTIME;	/* �����N���n���h���N������ */
typedef SYSTIME		ALMTIME;	/* �A���[���n���h���N������ */
typedef INT 		DLYTIME;	/* �^�X�N�x������ */
/* ItIs �Ǝ��̌^ */
typedef long long	SYSUTIME;	/* �V�X�e������ (���\�]���p) */

/*
 *  ���ʒ萔
 */

#define NADR		((VP) -1)	/* �A�h���X��|�C���^�l������ */
#define TRUE		1		/* �^ */
#define FALSE		0		/* �U */

#define TA_NULL		0		/* ���ʂȑ������w�肵�Ȃ� */

#define TA_ASM		0x00000000	/* �A�Z���u���ɂ��v���O���� */
#define TA_HLNG		0x00000001	/* ��������ɂ��v���O���� */

#define TA_TFIFO	0x00000000	/* �҂��^�X�N�� FIFO �ŊǗ� */
#define TA_TPRI		0x00000001	/* �҂��^�X�N��D��x���ŊǗ� */

#define TMO_POL		0		/* �|�[�����O */
#define TMO_FEVR	(-1)		/* �i�v�҂� */

/*
 *  �^�X�N�Ǘ��֌W
 */

/* cre_tsk */
typedef struct t_ctsk {
	VP	exinf;		/* �g����� */
	ATR	tskatr;		/* �^�X�N���� */
	FP	task;		/* �^�X�N�N���A�h���X */
	PRI	itskpri;	/* �^�X�N�N�����D��x */
	INT	stksz;		/* ���[�U�X�^�b�N�T�C�Y */
	/* ItIs �Ǝ��̃t�B�[���h */
	INT	sstksz;		/* �V�X�e���X�^�b�N�T�C�Y */
} T_CTSK;

#define TA_COP0		0x00008000	/* ID=0 �̃R�v���Z�b�T���g�p */
#define TA_COP1		0x00004000	/* ID=1 �̃R�v���Z�b�T���g�p */
#define TA_COP2		0x00002000	/* ID=2 �̃R�v���Z�b�T���g�p */
#define TA_COP3		0x00001000	/* ID=3 �̃R�v���Z�b�T���g�p */
#define TA_COP4		0x00000800	/* ID=4 �̃R�v���Z�b�T���g�p */
#define TA_COP5		0x00000400	/* ID=5 �̃R�v���Z�b�T���g�p */
#define TA_COP6		0x00000200	/* ID=6 �̃R�v���Z�b�T���g�p */
#define TA_COP7		0x00000100	/* ID=7 �̃R�v���Z�b�T���g�p */

/* ItIs �Ǝ��̑��� */
#define TA_SSTKSZ	0x00010000	/* �V�X�e���X�^�b�N�T�C�Y���w�� */

/* ItIs �Ǝ��̑��� (TRON�d�l�`�b�v) */
#define TA_RNG0		0x00000000	/* �����O���x��0 �Ŏ��s */
#define TA_RNG1		0x01000000	/* �����O���x��1 �Ŏ��s */
#define TA_RNG2		0x02000000	/* �����O���x��2 �Ŏ��s */
#define TA_RNG3		0x03000000	/* �����O���x��3 �Ŏ��s */

/* ItIs �Ǝ��̑��� (68040) */
#define TA_SMODE	0x00000000	/* �X�[�p�o�C�U���[�h�Ŏ��s */
#define TA_UMODE	0x01000000	/* ���[�U���[�h�Ŏ��s */

#define TSK_SELF	0		/* ���^�X�N�w�� */

#define TPRI_INI	0	/* �^�X�N�N�����D��x���w�� (chg_pri) */
#define TPRI_RUN	0	/* ���s���̍ō��D��x���w�� (rot_rdq) */

/* ref_tsk */
typedef	struct t_rtsk {
	VP	exinf;		/* �g����� */
	PRI	tskpri;		/* ���݂̗D��x */
	UINT	tskstat;	/* �^�X�N��� */
	UINT	tskwait;	/* �҂��v�� */
	ID	wid;		/* �҂��I�u�W�F�N�gID */
	INT	wupcnt;		/* �N���v���L���[�C���O�� */
	INT	suscnt;		/* SUSPEND�v���l�X�g�� */
} T_RTSK;

#define TTS_RUN		0x00000001	/* RUN */
#define TTS_RDY		0x00000002	/* READY */
#define TTS_WAI		0x00000004	/* WAIT */
#define TTS_SUS		0x00000008	/* SUSPEND */
#define TTS_WAS		0x0000000c	/* WAIT-SUSPEND */
#define TTS_DMT		0x00000010	/* DORMANT */

#define TTW_SLP		0x00000001	/* �N���҂��ɂ��҂� */
#define TTW_DLY		0x00000002	/* �^�X�N�̒x���ɂ��҂� */
#define TTW_NOD		0x00000008	/* �ڑ��@�\�����҂� */
#define TTW_FLG		0x00000010	/* �C�x���g�t���O�҂� */
#define TTW_SEM		0x00000020	/* �Z�}�t�H�҂� */
#define TTW_MBX		0x00000040	/* ���C���{�b�N�X�҂� */
#define TTW_SMBF	0x00000080	/* ���b�Z�[�W�o�b�t�@���M�҂� */
#define TTW_MBF		0x00000100	/* ���b�Z�[�W�o�b�t�@��M�҂� */
#define TTW_CAL		0x00000200	/* �����f�u�ďo�҂� */
#define TTW_ACP		0x00000400	/* �����f�u��t�҂� */
#define TTW_RDV		0x00000800	/* �����f�u�I���҂� */
#define TTW_MPL		0x00001000	/* �ϒ��������v�[���҂� */
#define TTW_MPF		0x00002000	/* �Œ蒷�������v�[���҂� */
/* ItIs �Ǝ��̒�` */
#define TTW_PIS		0x00010000	/* �D��x�p���Z�}�t�H�҂� */
#define TTW_TMB		0x00020000	/* �^�X�N�t�����C���{�b�N�X�҂� */

/*
 *  �Z�}�t�H�֌W
 */

/* cre_sem */
typedef	struct t_csem {
	VP	exinf;		/* �g����� */
	ATR	sematr;		/* �Z�}�t�H���� */
	INT	isemcnt;	/* �Z�}�t�H�̏����J�E���g�l */
	INT	maxsem;		/* �Z�}�t�H�̍ő�J�E���g�l */
} T_CSEM;

/* ref_sem */
typedef	struct t_rsem {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	INT	semcnt;		/* ���݂̃Z�}�t�H�J�E���g�l */
} T_RSEM;

/*
 *  �C�x���g�t���O�֌W
 */

/* cre_flg */
typedef	struct t_cflg {
	VP	exinf;		/* �g����� */
	ATR	flgatr;		/* �C�x���g�t���O���� */
	UINT	iflgptn;	/* �C�x���g�t���O�̏����p�^�[�� */
} T_CFLG;

#define TA_WSGL		0x00000000	/* �����^�X�N�̑҂��������Ȃ� */
#define TA_WMUL		0x00000008	/* �����^�X�N�̑҂������� */

#define TWF_ANDW	0x00000000	/* AND�҂� */
#define TWF_ORW		0x00000002	/* OR�҂� */
#define TWF_CLR		0x00000001	/* �N���A�w�� */

/* ref_flg */
typedef	struct t_rflg {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	UINT	flgptn;		/* ���݂̃C�x���g�t���O�p�^�[�� */
} T_RFLG;

/*
 *  ���C���{�b�N�X�֌W
 */

/* cre_mbx */
typedef	struct t_cmbx {
	VP	exinf;		/* �g����� */
	ATR	mbxatr;		/* ���C���{�b�N�X���� */
} T_CMBX;

#define TA_MFIFO	0x00000000	/* ���b�Z�[�W�� FIFO �ŊǗ� */
#define TA_MPRI		0x00000002	/* ���b�Z�[�W��D��x���ŊǗ� */

typedef struct t_msg {
	VP	msgque[1];	/* ���b�Z�[�W�L���[�̂��߂̃G���A */
	PRI	msgpri;		/* ���b�Z�[�W�D��x */
	VB	msgcont[0];	/* ���b�Z�[�W�{�� */
} T_MSG;

/* ref_mbx */
typedef	struct t_rmbx {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	T_MSG	*pk_msg;	/* ���Ɏ�M����郁�b�Z�[�W */
} T_RMBX;

/*
 *  ���b�Z�[�W�o�b�t�@�֌W
 */

/* cre_mbf */
typedef	struct t_cmbf {
	VP	exinf;		/* �g����� */
	ATR	mbfatr;		/* ���b�Z�[�W�o�b�t�@���� */
	INT	bufsz;		/* ���b�Z�[�W�o�b�t�@�̃T�C�Y */
	INT	maxmsz;		/* ���b�Z�[�W�̍ő咷 */
} T_CMBF;

#define TMBF_OS		(-4)	/* OS �̃G���[���O�p�̃��b�Z�[�W�o�b�t�@ */
#define TMBF_DB		(-3)	/* �f�o�b�O�p�̃��b�Z�[�W�o�b�t�@ */

/* ref_mbf */
typedef struct t_rmbf {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* ��M�҂��^�X�N�̗L�� */
	BOOL_ID	stsk;		/* ���M�҂��^�X�N�̗L�� */
	INT	msgsz;		/* ���Ɏ�M����郁�b�Z�[�W�̃T�C�Y */
	INT	frbufsz;	/* �󂫃o�b�t�@�̃T�C�Y */
} T_RMBF;

/*
 *  �����f�u�C�����f�u�p�|�[�g�֌W
 */

/* cre_por */
typedef	struct t_cpor {
	VP	exinf;		/* �g����� */
	ATR	poratr;		/* �|�[�g���� */
	INT	maxcmsz;	/* �ďo���b�Z�[�W�̍ő咷 */
	INT	maxrmsz;	/* �ԓ����b�Z�[�W�̍ő咷 */
} T_CPOR;

/* ref_por */
typedef struct t_rpor {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �ďo�҂��^�X�N�̗L�� */
	BOOL_ID	atsk;		/* ��t�҂��^�X�N�̗L�� */
} T_RPOR;

/*
 *  �D��x�p���Z�}�t�H�֌W (ItIs �Ǝ��̒�`)
 */

/* vcre_pis */
typedef	struct t_cpis {
	VP	exinf;		/* �g����� */
	ATR	pisatr;		/* �D��x�p���Z�}�t�H���� */
} T_CPIS;

/* vref_pis */
typedef	struct t_rpis {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	BOOL_ID	pistsk;		/* �Z�}�t�H�擾�^�X�N�̗L�� */
} T_RPIS;

/*
 *  �^�X�N�t�����C���{�b�N�X�֌W (ItIs �Ǝ��̒�`)
 */

/* vref_tmb */
typedef	struct t_rtmb {
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	T_MSG	*pk_msg;	/* ���Ɏ�M����郁�b�Z�[�W */
} T_RTMB;

/*
 *  �����݊Ǘ��֌W
 */

/* def_int */
typedef struct t_dint {
	ATR	intatr;		/* �����݃n���h������ */
	FP	inthdr;		/* �����݃n���h���A�h���X */
	/* ItIs �Ǝ��̃t�B�[���h (TRON�d�l�`�b�v) */
	UINT	eitatr;		/* EIT���� */
} T_DINT;

/* ItIs �Ǝ��̑��� (TRON�d�l�`�b�v) */
#define TA_EITATR	0x00010000	/* EIT�������w�� */

/*
 *  �������v�[���֌W
 */

/* cre_mpl */
typedef	struct t_cmpl {
	VP	exinf;		/* �g����� */
	ATR	mplatr;		/* �������v�[������ */
	INT	mplsz;		/* �������v�[���S�̂̃T�C�Y */
} T_CMPL;

/* ItIs �Ǝ��̑��� */
#define TA_HFIT		0x00000000	/* half-fit �A���S���Y�� */
#define TA_FFIT		0x00010000	/* first-fit �A���S���Y�� */

#define TMPL_OS		(-4)		/* OS�p�������v�[�� */

/* ref_mpl */
typedef struct t_rmpl {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	INT	frsz;		/* �󂫗̈�̍��v�T�C�Y */
	INT	maxsz;		/* �ő�̘A���󂫗̈�̃T�C�Y */
} T_RMPL;

/* cre_mpf */
typedef	struct t_cmpf {
	VP	exinf;		/* �g����� */
	ATR	mpfatr;		/* �������v�[������ */
	INT	mpfcnt;		/* �������v�[���S�̂̃u���b�N�� */
	INT	blfsz;		/* �Œ蒷�������u���b�N�̃T�C�Y */
} T_CMPF;

/* ref_mpf */
typedef struct t_rmpf {
	VP	exinf;		/* �g����� */
	BOOL_ID	wtsk;		/* �҂��^�X�N�̗L�� */
	INT	frbcnt;		/* �󂫗̈�̃u���b�N�� */
} T_RMPF;

/*
 *  ���ԊǗ��֌W
 */

/* def_cyc */
typedef struct t_dcyc {
	VP	exinf;		/* �g����� */
	ATR	cycatr;		/* �����N���n���h������ */
	FP	cychdr;		/* �����N���n���h���A�h���X */
	UINT	cycact;		/* �����N���n���h��������� */
	CYCTIME	cyctim;		/* �����N�����ԊԊu */
} T_DCYC;

#define TCY_OFF		0x00000000	/* �����N���n���h�����N�����Ȃ� */
#define TCY_ON		0x00000001	/* �����N���n���h�����N������ */
#define TCY_INI		0x00000002	/* �����̃J�E���g������������ */

/* ref_cyc */
typedef struct t_rcyc {
	VP	exinf;		/* �g����� */
	CYCTIME	lfttim;		/* ���̃n���h���N���܂ł̎c�莞�� */
	UINT	cycact;		/* �����N���n���h��������� */
} T_RCYC;

/* def_alm */
typedef struct t_dalm {
	VP	exinf;		/* �g����� */
	ATR	almatr;		/* �A���[���n���h������ */
	FP	almhdr;		/* �A���[���n���h���A�h���X */
	UINT	tmmode;		/* �N�������w�胂�[�h */
	ALMTIME	almtim;		/* �n���h���N������ */
} T_DALM;

#define TTM_ABS		0x00000000	/* ��Ύ����ł̎w�� */
#define TTM_REL		0x00000001	/* ���Ύ����ł̎w�� */

/* ref_alm */
typedef struct t_ralm {
	VP	exinf;		/* �g����� */
	ALMTIME	lfttim;		/* �n���h���N���܂ł̎c�莞�� */
} T_RALM;

/*
 *  �V�X�e���Ǘ��֌W
 */

/* get_ver */
typedef struct t_ver {
	UH	maker;		/* ���[�J�[ */
	UH	id;		/* �`���ԍ� */
	UH	spver;		/* �d�l���o�[�W���� */
	UH	prver;		/* ���i�o�[�W���� */
	UH	prno[4];	/* ���i�Ǘ���� */
	UH	cpu;		/* CPU��� */
	UH	var;		/* �o���G�[�V�����L�q�q */
} T_VER;

/* ref_sys */
typedef struct t_rsys {
	INT	sysstat;	/* �V�X�e����� */
	ID	runtskid;	/* ���s��Ԃɂ���^�X�N�� ID */
	ID	schedtskid;	/* ���s��Ԃɂ��ׂ��^�X�N�� ID */
} T_RSYS;

#define TTS_TSK		0	/* �^�X�N�����s�� */
#define TTS_DDSP	1	/* �f�B�X�p�b�`�֎~�� */
#define TTS_LOC		3	/* �����݂���уf�B�X�p�b�`�֎~�� */
#define TTS_INDP	4	/* �^�X�N�Ɨ������s�� */
/* ItIs �Ǝ��̒�` */
#define TTS_QTSK	8	/* ���^�X�N�����s�� */

/* ref_cfg */
typedef struct t_rcfg {
	/* ItIs �Ǝ��̃t�B�[���h */
} T_RCFG;

/* def_svc */
typedef struct t_dsvc {
	ATR	svcatr;		/* �g��SVC�n���h������ */
	FP	svchdr;		/* �g��SVC�n���h���A�h���X */
} T_DSVC;

/* def_exc */
typedef struct t_dexc {
	ATR	excatr;		/* ��O�n���h������ */
	FP	exchdr;		/* ��O�n���h���A�h���X */
} T_DEXC;

/*
 *  �l�b�g���[�N�Ǘ��֌W
 */

#define TND_SELF	0	/* ���m�[�h�w�� */
#define TND_OTHR	(-1)	/* �f�t�H���g�̑��m�[�h�w�� */

#endif /* _ITRON_ */
