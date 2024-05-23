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
 *  @(#) $Id: sys_serial.h,v 1.4 1996/02/17 09:44:00 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  MCUBE�p �჌�x���V���A��I/O �֘A�̒�`
 */

#include "mcube.h"

/*
 *  �V���A���|�[�g�̃n�[�h�E�F�A�ˑ����̒�`
 */
typedef struct raw_serial_port_descripter {
	BOOL	*initflag;	/* �������σt���O�ւ̃|�C���^ */
	IOREG	*data;		/* �f�[�^���W�X�^�̔Ԓn */
	IOREG	*cntrl;		/* �R���g���[�����W�X�^�̔Ԓn */

	byte	com_reset;	/* �|�[�g���Z�b�g�R�}���h */
	byte	mode3_def;	/* �f�t�H�[���g�̃��[�h�ݒ�l (WR3) */
	byte	mode4_def;	/* �f�t�H�[���g�̃��[�h�ݒ�l (WR4) */
	byte	mode5_def;	/* �f�t�H�[���g�̃��[�h�ݒ�l (WR5) */
	byte	mode12_def;	/* �f�t�H�[���g�̃��[�h�ݒ�l (WR12) */
	byte	mode13_def;	/* �f�t�H�[���g�̃��[�h�ݒ�l (WR13) */
} RPORT;

/*
 *  SCC�R���g���[�����W�X�^�̐ݒ�l
 */

#define MODE4_DEF	0x44		/* �X�g�b�v�r�b�g 1bit, �p���e�B�Ȃ� */
#define MODE3_DEF	0xc1		/* �f�[�^ 8bit, ��M�C�l�[�u�� */
#define MODE3_INIT_MASK	0xfe		/* ��M�f�B�X�G�[�u�� */
#define MODE5_DEF	0xea		/* �f�[�^ 8bit, ���M�C�l�[�u�� */
#define MODE5_INIT_MASK	0x75		/* ���M�f�B�X�G�[�u�� */

#define MODE10_DEF	0x00		/* NRZ */
#define MODE11_DEF	0x56		/* �N���b�N�Ɋւ���ݒ� */
#define MODE12_DEF	10		/* 9600bps */
#define MODE13_DEF	0		/* 9600bps */
#define MODE14_DEF	0x03		/* �{�[���[�g�W�F�l���[�^ */

#define MODE1_DEF	0x13		/* �e�����݂����� */
#define MODE1_DOWN	0x00		/* �S�����݂��֎~ */
#define MODE15_DEF	0x00		/* ���̑��̊����݂̋֎~ */
#define MODE9_INIT	0x02		/* �}�X�^�[�����݋֎~ */
#define MODE9_DEF	0x0a		/* �}�X�^�[�����݋��� */
#define MODE9_DOWN	0x02		/* �}�X�^�[�����݋֎~ */

#define MODE4_KBD	0x45		/* �X�g�b�v�r�b�g 1bit, ��p���e�B */
#define MODE3_KBD	0xc1		/* �f�[�^ 8bit, ��M�C�l�[�u�� */
#define MODE5_KBD	0x68		/* �f�[�^ 8bit, ���M�C�l�[�u�� */
#define MODE12_KBD	4		/* 19200bps */
#define MODE13_KBD	0		/* 19200bps */

/*
 *  �����݃x�N�^�ƃn���h���A�h���X�̎��o��
 */
#define raw_int_vector(p)	IRC1_VECTOR(3)
#define raw_int_handler(p)	int_handler_scc

/*
 *  �����݃n���h���̃G���g�� (�O���錾)
 */
static void	int_handler_scc(void);

static void	serial_int_handler(int portid);

/*
 *  �჌�x���|�[�g���Ǘ��u���b�N�̏����l
 */

#define NUM_PORT	2	/* �T�|�[�g����V���A���|�[�g�̐� */

static int	initflag[2] = { -1, 0 } ;	/* �������σt���O */

#define RAWPORT1	{ &initflag[0], SCC_DATAB, SCC_CNTRLB,		\
			  0x40, MODE3_DEF, MODE4_DEF, MODE5_DEF,	\
			  MODE12_DEF, MODE13_DEF }
#define RAWPORT2	{ &initflag[1], SCC_DATAA, SCC_CNTRLA,		\
			  0x80, MODE3_KBD, MODE4_KBD, MODE5_KBD,	\
			  MODE12_KBD, MODE13_KBD }

#define IOCTL_PORT2	{ 0, 0, 0, 0 }

/*
 *  �V���A�� I/O �|�[�g�̏�����
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  SCC �̐ݒ�
	 */
	io_write(p->cntrl, SCC_WR0);			/* WR0 �w�� */
	if (*(p->initflag) == 0) {
		scc_write(p->cntrl, SCC_WR9, p->com_reset);

		scc_write(p->cntrl, SCC_WR4, p->mode4_def);
		scc_write(p->cntrl, SCC_WR3, (p->mode3_def & MODE3_INIT_MASK));
		scc_write(p->cntrl, SCC_WR5, (p->mode5_def & MODE5_INIT_MASK));

		scc_write(p->cntrl, SCC_WR9, MODE9_INIT);
		scc_write(p->cntrl, SCC_WR10, MODE10_DEF);
		scc_write(p->cntrl, SCC_WR11, MODE11_DEF);
		scc_write(p->cntrl, SCC_WR12, p->mode12_def);
		scc_write(p->cntrl, SCC_WR13, p->mode13_def);
		scc_write(p->cntrl, SCC_WR14, MODE14_DEF);

		scc_write(p->cntrl, SCC_WR3, p->mode3_def);
		scc_write(p->cntrl, SCC_WR5, p->mode5_def);
	}

	/*
	 *  IRC �̐ݒ�
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		irc_assign(IRC1_LMR(3), LIR3_BIT);
		irc_and_assign(IRC1_TMR, ~LIR3_BIT);	/* ���x���g���K */
		irc_and_assign(IRC1_IMR, ~LIR3_BIT);	/* �����݃}�X�N���� */
	}

	/*
	 *  SCC �̊����݊֌W�̐ݒ�
	 */
	scc_write(p->cntrl, SCC_WR15, MODE15_DEF);
	scc_write(p->cntrl, SCC_WR1, MODE1_DEF);
	io_write(p->cntrl, 0x28);
	scc_write(p->cntrl, SCC_WR9, MODE9_DEF);

	*(p->initflag) = 1;				/* �������t���O�ݒ� */
	return(0);
}

/*
 *  �V���A�� I/O �|�[�g�̃V���b�g�_�E��
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	*(p->initflag) = -1;				/* �������t���O�ݒ� */

	/*
	 *  SCC �̊����݊֌W�̐ݒ�
	 */
	scc_write(p->cntrl, SCC_WR1, MODE1_DOWN);
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		scc_write(p->cntrl, SCC_WR9, MODE9_DOWN);
	}

	/*
	 *  IRC �̐ݒ�
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		irc_or_assign(IRC1_IMR, LIR3_BIT);	/* �����݃}�X�N�ݒ� */
	}
}

/*
 *  �����݃n���h���̃G���g��
 */
static void
int_handler_scc(void)
{
	if (initflag[0] > 0) {
		serial_int_handler(1);
	}
	if (initflag[1] > 0) {
		serial_int_handler(2);
	}
}

/*
 *  �����݃N���A����
 */
Inline void
raw_port_clear_int(RPORT *p)
{
	io_write(p->cntrl, 0x10);
}

/*
 *  ��������M�������H
 */
Inline BOOL
raw_port_getready(RPORT *p)
{
	return(scc_read(p->cntrl, SCC_RR0) & 0x01);
}

/*
 *  �����𑗐M�ł��邩�H
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(scc_read(p->cntrl, SCC_RR0) & 0x04);
}

/*
 *  ��M���������̎��o��
 */
Inline byte
raw_port_getchar(RPORT *p)
{
	return(io_read(p->data));
}

/*
 *  ���M���镶���̏�������
 */
Inline void
raw_port_putchar(RPORT *p, byte c)
{
	io_write(p->data, c);
}

/*
 *  ���M����֐�
 */
Inline void
raw_port_sendstart(RPORT *p)
{
}

Inline void
raw_port_sendstop(RPORT *p)
{
	io_write(p->cntrl, 0x28);
}

#endif /* _SYS_SERIAL_ */
