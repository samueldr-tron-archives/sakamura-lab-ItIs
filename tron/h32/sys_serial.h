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
 *  @(#) $Id: sys_serial.h,v 1.5 1996/02/17 09:43:42 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  H32SBC�V�X�e�� CPU�{�[�h�p �჌�x���V���A��I/O �֘A�̒�`
 */

#include "h32sbc.h"

/*
 *  �V���A���|�[�g�̃n�[�h�E�F�A�ˑ����̒�`
 */
typedef struct raw_serial_port_descripter {
	IOREG	*data;		/* ACI�f�[�^���W�X�^�̔Ԓn */
	IOREG	*status;	/* ACI�X�e�[�^�X���W�X�^�̔Ԓn */
	IOREG	*mode;		/* ACI���[�h���W�X�^�̔Ԓn */
	IOREG	*command;	/* ACI�R�}���h���W�X�^�̔Ԓn */

	byte	def_mode1;	/* �f�t�H�[���g�̃��[�h�ݒ�l(1) */
	byte	def_mode2;	/* �f�t�H�[���g�̃��[�h�ݒ�l(2) */

	byte	irc_bit;	/* IRC�ݒ�̃r�b�g�p�^�[�� */
	byte	int_level;	/* �����݃��x�� */
	FP	int_handler;	/* �����݃n���h�� */
} RPORT;

/*
 *  ACI�R�}���h���W�X�^�̐ݒ�l
 */
#define	COM_STOP	0x32	/* ����M�Ƃ���~ */
#define	COM_SSTOP	0x36	/* ���M��~�C��M�̓C�l�[�u�� */
#define	COM_START	0x37	/* ����M�Ƃ��C�l�[�u�� */

/*
 *  ACI���[�h���W�X�^�̃f�t�H���g�ݒ�l
 */
#define	DEF_MODE1	0x4d	/* 8bit, �X�g�b�v�r�b�g 1bit, �p���e�B�Ȃ� */
#define	DEF_MODE2	0x3e	/* 9600bps */

/*
 *  �����݃x�N�^�ƃn���h���A�h���X�̎��o��
 */
#define	raw_int_vector(p)	INT_VECTOR((p)->int_level)
#define	raw_int_handler(p)	((p)->int_handler)

/*
 *  �����݃n���h���̃G���g�� (�O���錾)
 */
static void	int_handler_aci1(void);
static void	int_handler_aci0(void);

static void	serial_int_handler(int portid);

/*
 *  �჌�x���|�[�g���Ǘ��u���b�N�̏����l
 */

#define NUM_PORT	2	/* �T�|�[�g����V���A���|�[�g�̐� */

#define	RAWPORT1	{ ACI0_DATA, ACI0_STATUS, ACI0_MODE, ACI0_COMMAND, \
			  DEF_MODE1, DEF_MODE2,				   \
			  LIR2_BIT, 4, int_handler_aci0 }
#define	RAWPORT2	{ ACI1_DATA, ACI1_STATUS, ACI1_MODE, ACI1_COMMAND, \
			  DEF_MODE1, DEF_MODE2,				   \
			  LIR3_BIT, 3, int_handler_aci1 }

/*
 *  �V���A�� I/O �|�[�g�̏�����
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  ACI �̐ݒ�
	 */
	*(p->command) = COM_STOP;
	n = *(p->command);		/* �_�~�[���[�h */
	*(p->mode) = p->def_mode1;
	*(p->mode) = p->def_mode2;
	*(p->command) = COM_SSTOP;	/* ��M�̂݃C�l�[�u�� */

	/*
	 *  IRC �̐ݒ�
	 */
	irc_assign(IRC_LMR(p->int_level), p->irc_bit);
	irc_or_assign(IRC_VMR, p->irc_bit);
	irc_and_assign(IRC_TMR, ~(p->irc_bit));	/* ���x���g���K���[�h */
	irc_and_assign(IRC_IMR, ~(p->irc_bit));	/* �����݃}�X�N���� */

	if ((n = irc_read(IRC_TMR)) & p->irc_bit) {
		/*
		 *  IRC �̏������Ɏ��s
		 *
		 *  TMR (�g���K���[�h���W�X�^) �̓ǂݏo���Ɏ��s���C����
		 *  ���ʐ����������݂�������Ȃ�����Ƃ�����Q���o�邱
		 *  �Ƃ������D���̏�Q�̓v���O�����̒u�����ʒu�Ɉˑ�
		 *  ���ċN����C�Č���������D�����͕s���D
		 */
		syslog(LOG_EMERG,
			"Serial port initialization error (%02x).", n);
		return(1);
	}
	return(0);
}

/*
 *  �V���A�� I/O �|�[�g�̃V���b�g�_�E��
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	irc_or_assign(IRC_IMR, p->irc_bit);	/* �����݂��}�X�N */
}

/*
 *  �����݃n���h���̃G���g��
 */
static void
int_handler_aci0(void)
{
	int	intmask;

	irc_assign(IRC_IRR, LIR2_BIT);	/* �����ݗv�����N���A(�{���͕s�v) */
	serial_int_handler(1);
}

static void
int_handler_aci1(void)
{
	irc_assign(IRC_IRR, LIR3_BIT);	/* �����ݗv�����N���A(�{���͕s�v) */
	serial_int_handler(2);
}

/*
 *  �����݃N���A����
 */
Inline void
raw_port_clear_int(RPORT *p)
{
}

/*
 *  ��������M�������H
 */
Inline BOOL
raw_port_getready(RPORT *p)
{
	return(*(p->status) & 0x02);
}

/*
 *  �����𑗐M�ł��邩�H
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(*(p->status) & 0x01);
}

/*
 *  ��M���������̎��o��
 */
Inline byte
raw_port_getchar(RPORT *p)
{
	return(*(p->data));
}

/*
 *  ���M���镶���̏�������
 */
Inline void
raw_port_putchar(RPORT *p, byte c)
{
	*(p->data) = c;
}

/*
 *  ���M����֐�
 */
Inline void
raw_port_sendstart(RPORT *p)
{
	*(p->command) = COM_START;
}

Inline void
raw_port_sendstop(RPORT *p)
{
	*(p->command) = COM_SSTOP;
}

/*
 *  IRC �� ACI �̐ݒ��Ԃ�\������ (�f�o�b�O�p)�D
 */
#ifdef DEBUG

Inline void
raw_port_stat(RPORT *p)
{
	syslog(LOG_NOTICE, "IRC_VMR = %x", irc_read(IRC_VMR));
	syslog(LOG_NOTICE, "IRC_TMR = %x", irc_read(IRC_TMR));
	syslog(LOG_NOTICE, "IRC_IMR = %x", irc_read(IRC_IMR));
	syslog(LOG_NOTICE, "IRC_IRR = %x", irc_read(IRC_IRR));
	syslog(LOG_NOTICE, "ACI_COMMAND = %x", *(p->command));
	syslog(LOG_NOTICE, "ACI_MODE1 = %x", *(p->mode));
	syslog(LOG_NOTICE, "ACI_MODE2 = %x", *(p->mode));
	syslog(LOG_NOTICE, "ACI_STATUS = %x", *(p->status));
	syslog(LOG_NOTICE, "ACI_DATA = %x", *(p->data));
}

#endif DEBUG

#endif /* _SYS_SERIAL_ */
