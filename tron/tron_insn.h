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
 *  @(#) $Id: tron_insn.h,v 1.4 1997/01/10 12:34:59 hiro Exp $
 */

#ifndef	_TRON_INSN_
#define	_TRON_INSN_

/*
 *  ���䃌�W�X�^�̑���֐�
 */

/*
 *  PSW �̌��ݒl�̓ǂݏo��
 */
Inline VW
current_psw(void)
{
	VW	psw;

	Asm("stc %1, %0" : "=g"(psw) : "m"(CREG(PSW)));
	return(psw);
}

/*
 *  PSW �̌��ݒl�̕ύX
 */
Inline void
set_psw(VW psw)
{
	Asm("ldc %0, %1" : : "g"(psw), "m"(CREG(PSW)));
}

/*
 *  IMASK �̌��ݒl�̓ǂݏo��
 *
 *  IMASK �̒l�Ƃ��āC0 �ȏ� 15 �ȉ��̒l���Ԃ�D
 */
#define imask_psw(psw)	((psw >> 16) & 15)
#define current_imask() (imask_psw(current_psw()))

/*
 *  IMASK �̌��ݒl�̕ύX
 *
 *  imask �́C0 �ȏ� 15 �ȉ��ł��邱�ƁD
 */
Inline void
set_imask(INT imask)
{
	Asm("ldc %0, %1" : : "g"(imask << 16), "m"(CREG(IMASK)));
}

/*
 *  �����݃}�X�N��Ԃ�����\���ŕԂ��D
 */
Inline INT
current_intmask(void)
{
	INT	intmask;

	Asm("stc %1, %0" : "=g"(intmask) : "m"(CREG(IMASK)));
	return(intmask);
}

/*
 *  NMI ���������ׂĂ̊����݂��֎~����D
 */
Inline void
disint(void)
{
	Asm("ldc #0, %0" : : "m"(CREG(IMASK)));
}

/*
 *  current_intmask ���Ԃ��������݃}�X�N��Ԃ�n���āC�����݋֎~�O�̏�
 *  �Ԃɖ߂��D
 *
 *  clobber���X�g�� memory �����Ă���̂́Cenaint �̒���Ƀ^�X�N�f�B
 *  �X�p�b�`���N����C�\�����Ȃ��������G���A��������������\������
 *  �邱�Ƃ��R���p�C���ɒm�点�邽�߂ł���D��̓I�ɂ́C�G���[�R�[�h��
 *  �����ϐ������ɂȂ�D
 */
Inline void
enaint(INT intmask)
{
	Asm("ldc %0, %1" : : "g"(intmask), "m"(CREG(IMASK)) : "memory");
}

/*
 *  DIR (�x�������ݗv�����W�X�^) �̌��ݒl�̓ǂݏo��
 */
Inline VW
current_dir(void)
{
	VW	dir;

	Asm("stc %1, %0" : "=g"(dir) : "m"(CREG(DIR)));
	return(dir);
}

/*
 *  DIR (�x�������ݗv�����W�X�^) �̌��ݒl�̕ύX
 *
 *  dir �́C0 �ȏ� 15 �ȉ��ł��邱�ƁD
 */
Inline void
set_dir(INT dir)
{
	Asm("ldc %0, %1" : : "g"(dir), "m"(CREG(DIR)));
}

/*
 *  EIT�֘A�̒�`
 */

#define EITVEC_BUSERR	0x11		/* �o�X�G���[�̃x�N�^�ԍ� */
#define EITVEC_TRAPA1	0x21		/* TRAPA #1 �̃x�N�^�ԍ� */
#define EITVEC_TRAPA2	0x22		/* TRAPA #2 �̃x�N�^�ԍ� */
#define EITVEC_TRAPA3	0x23		/* TRAPA #3 �̃x�N�^�ԍ� */
#define EITVEC_TRAPA4	0x24		/* TRAPA #4 �̃x�N�^�ԍ� */
#define EITVEC_DI14	0x5e		/* DI=14 �̃x�N�^�ԍ� */

#define EITATR(smode, imask)	(((smode) << 31) + ((imask) << 16))

/*
 *  EIT�x�N�^�e�[�u���̍\���̒�`
 */
typedef struct eit_vector_entry {
	UINT	eitatr;			/* EIT���� */
	FP	eithdr;			/* EIT�n���h���̃A�h���X */
} EITVE;

/*
 *  EIT�x�N�^�e�[�u���x�[�X (EITVB) �̐ݒ�
 *
 *  �����������̒��Ŏg�����߂̊֐��D
 */
Inline void
set_eitvb(EITVE *eitvb)
{
	Asm("ldc %0, %1" : : "r"(eitvb), "m"(CREG(EITVB)));
}

/*
 *  EIT�x�N�^�e�[�u���x�[�X (EITVB) �̌��ݒl�̓ǂݏo��
 *
 *  EITVB_ITIS ����`����Ă��鎞�́C�����������̒��� EITVB���W�X�^��
 *  EITVB_ITIS �ɐݒ肷��̂ŁCEITVB_ITIS ��Ԃ��΂悢�D�����łȂ��ꍇ
 *  �́CCPU �� EITVB���W�X�^��ǂݏo���D
 */
#ifdef EITVB_ITIS
#define current_eitvb()	((EITVE *) EITVB_ITIS)
#else /* EITVB_ITIS */

Inline EITVE
*current_eitvb(void)
{
	EITVE	*eitvb;

	Asm("stc %1, %0" : "=r"(eitvb) : "m"(CREG(EITVB)));
	return(eitvb);
}

#endif /* EITVB_ITIS */

/*
 *  EIT�n���h���̐ݒ�
 *
 *  �x�N�g���ԍ� eitvec �� EIT�x�N�^�e�[�u���G���g���́CEIT���� (EIT�n
 *  ���h�����s���́C�X�^�b�N���[�h�C�A�h���X�ϊ����[�h�C�f�o�b�O���[�h�C
 *  �����݃}�X�N�����߂�) �� eitatr�CEIT�n���h���J�n�Ԓn�� eithdr �ɐ�
 *  �肷��D
 */
Inline void
define_eit(INT eitvec, UINT eitatr, FP eithdr)
{
	EITVE	*eitvb = current_eitvb();

	eitvb[eitvec].eitatr = eitatr;
	eitvb[eitvec].eithdr = eithdr;
}

/*
 *  �������u���b�N���색�C�u����
 *
 *  �֐��̎d�l�́CANSI C ���C�u�����̎d�l�Ɠ����D�W�����C�u�����̂��̂�
 *  �g���������������ǂ��\��������D
 */

Inline VP
memcpy(VP dest, VP src, UINT len)
{
#ifdef tron
	if (len > 0) {
		register VP	src_r0 asm("r0") = src;
		register VP	dest_r1 asm("r1") = dest;
		register UINT	len_r2 asm("r2") = len;

		asm("smov/f.b"
		  : /* no output */
		  : "g"(src_r0), "g"(dest_r1), "g"(len_r2)
		  : "r0", "r1", "r2", "memory");
	}
#else /* tron */
	VB	*d = dest;
	VB	*s = src;

	while (len-- > 0) {
		*d++ = *s++;
	}
#endif /* tron */
	return(dest);
}

Inline VP
memset(VP str, INT ch, UINT len)
{
#ifdef tron
	if (len > 0) {
		register VP	str_r1 asm("r1") = str;
		register UINT	len_r2 asm("r2") = len;
		register INT	ch_r3 asm("r3") = ch;

		asm("sstr.b"
		  : /* no output */
		  : "g"(str_r1), "g"(len_r2), "g"(ch_r3)
		  : "r1", "r2", "memory");
	}
#else /* tron */
	VB	*s = str;

	while (len-- > 0) {
		*s++ = ch;
	}
#endif /* tron */
	return(str);
}

#endif /* _TRON_INSN_ */
