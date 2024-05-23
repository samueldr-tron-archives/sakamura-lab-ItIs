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
 *  @(#) $Id: serial.c,v 1.13 1996/02/17 09:38:15 hiro Exp $
 */

/*
 *  �V���A���C���^�t�F�[�X�h���C�o
 */

#include "systask.h"
#include "sys_serial.h"
#include <itis_ioctl.h>

#define	assert(exp)

/*
 *  �R���\�[���p�ɗp����V���A���|�[�g�ԍ�
 */
static int	console_portid;

/*
 *  �V���A���C���^�t�F�[�X�h���C�o�̋N��
 */
void
serial_startup(int portid)
{
	syscall(serial_init(portid));
	console_portid = portid;

#ifdef USE_EXTENDED_SVC
	syscall(def_svc(SVC_SERIAL_INIT, DSVC(serial_init)));
	syscall(def_svc(SVC_SERIAL_READ, DSVC(serial_read)));
	syscall(def_svc(SVC_SERIAL_WRITE, DSVC(serial_write)));
	syscall(def_svc(SVC_SERIAL_IOCTL, DSVC(serial_ioctl)));
	syscall(def_svc(SVC_SERIAL_SHUTDOWN, DSVC(serial_shutdown)));
#endif

	syslog(LOG_NOTICE, "Serial driver service starts on port %d.", portid);
}

/*
 *  �V���A���|�[�g�Ǘ��u���b�N�̒�`
 */

typedef struct ioctl_descripter {
	int	echo;
	int	input;
	int	newline;
	int	flowc;
} IOCTL;

#define	SERIAL_BUFSZ	256	/* �V���A���C���^�t�F�[�X�p�o�b�t�@�̃T�C�Y */

#define	inc(x)		(((x)+1 < SERIAL_BUFSZ) ? (x)+1 : 0)
#define	INC(x)		((++(x) < SERIAL_BUFSZ) ? (x) : ((x) = 0))

typedef struct serial_port_control_block {
	int	init_flag;	/* �������ς��H */
	RPORT	rawport;	/* �n�[�h�E�F�A�ˑ���� */
	char	*in_buffer;	/* ��M�o�b�t�@�G���A�̐擪 */
	ID	in_sem_id;	/* ��M�o�b�t�@�Ǘ��p�Z�}�t�H�� ID */
	int	in_read_ptr;	/* ��M�o�b�t�@�ǂݏo���|�C���^ */
	int	in_write_ptr;	/* ��M�o�b�t�@�������݃|�C���^ */
	char	*out_buffer;	/* ���M�o�b�t�@�G���A�̐擪 */
	ID	out_sem_id;	/* ���M�o�b�t�@�Ǘ��p�Z�}�t�H�� ID */
	int	out_read_ptr;	/* ���M�o�b�t�@�ǂݏo���|�C���^ */
	int	out_write_ptr;	/* ���M�o�b�t�@�������݃|�C���^ */
	IOCTL	ctl;		/* ioctl �ɂ��ݒ���e */
	BOOL	send_enabled;	/* ���M���C�l�[�u�����Ă��邩�H */
	BOOL	ixon_stopped;	/* STOP ���󂯎������Ԃ��H */
	BOOL	ixoff_stopped;	/* ����� STOP �𑗂�����Ԃ��H */
	char	ixoff_send;	/* ����� START/STOP �𑗂邩�H */
} SPCB;

/*
 *  ���W���[�����Ŏg���֐�
 */
static char	serial_read_one(SPCB *p);
static void	serial_write_one(SPCB *p, char c);

/*
 *  �V���A���|�[�g�Ǘ��u���b�N�̒�`�Ə�����
 */

SPCB spcb_table[NUM_PORT] = {
	{ 0, RAWPORT1,
	  0, SEM_SERIAL1_IN, 0, 0,
	  0, SEM_SERIAL1_OUT, 0, 0,
	 { 1, CANONICAL, 1, IXON|IXOFF },
	  0, 0, 0, 0 },
#if NUM_PORT > 1
	{ 0, RAWPORT2,
	  0, SEM_SERIAL2_IN, 0, 0,
	  0, SEM_SERIAL2_OUT, 0, 0,
#ifdef IOCTL_PORT2
	 IOCTL_PORT2,
#else
	 { 1, CANONICAL, 1, IXON|IXOFF },
#endif
	  0, 0, 0, 0 }
#endif
};

#define get_spcb(portid)	(&(spcb_table[(portid)-1]))
#define get_spcb_def(portid)	get_spcb((portid) ? (portid) : console_portid)

/*
 *  �|�[�g�̏�����
 */

int
serial_init(int portid)
{
	SPCB	*p;
	VP	buffer;
	T_DINT	pk_dint;
	ER	ercd = E_OK;

	if (sysstat() & TTS_INDP) {		/* �R���e�L�X�g�̃`�F�b�N */
		return(E_CTX);
	}
	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);
	}

	p = get_spcb(portid);
	if (p->init_flag) {			/* �������ς��̃`�F�b�N */
		return(E_OK);
	}

	ENTER_EXTENDED_SVC;

	/*
	 *  �o�b�t�@�̈�̊m�� (�V�X�e���������v�[��������)
	 */
	if (pget_blk(&buffer, TMPL_OS, SERIAL_BUFSZ * 2) != E_OK) {
		ercd = E_NOMEM;
		goto error_exit;
	}
	p->in_buffer = (char *) buffer;
	p->out_buffer = ((char *) buffer) + SERIAL_BUFSZ;

	/*
	 *  �ϐ��̏�����
	 */
	p->in_read_ptr = p->in_write_ptr = 0;
	p->out_read_ptr = p->out_write_ptr = 0;
	p->ixon_stopped = p->ixoff_stopped = 0;
	p->ixoff_send = 0;

	/*
	 *  �����݃n���h���̒�`
	 */
	pk_dint.intatr = TA_HLNG;
	pk_dint.inthdr = raw_int_handler(&(p->rawport));
	syscall(def_int(raw_int_vector(&(p->rawport)), &pk_dint));

	/*
	 *  �Z�}�t�H�̐���
	 */
	syscall(cre_sem(p->in_sem_id,
		&((T_CSEM) { 0, TA_TPRI, 0, SERIAL_BUFSZ-1 })));
	syscall(cre_sem(p->out_sem_id,
		&((T_CSEM) { 0, TA_TPRI, SERIAL_BUFSZ-1, SERIAL_BUFSZ-1 })));

	/*
	 *  �n�[�h�E�F�A�ˑ��̏�����
	 */
	syscall(loc_cpu());
	if (raw_port_init(&(p->rawport))) {
		ercd = E_SYS;
	}
	p->send_enabled = 0;
	syscall(unl_cpu());

	if (ercd == E_OK) {
		p->init_flag = 1;
	}

    error_exit:
	EXIT_EXTENDED_SVC;
	return(ercd);
}

/*
 *  �|�[�g�̃V���b�g�_�E��
 *
 *  flush �� TRUE �̏ꍇ�́C�V���A���|�[�g�ւ̑��M�o�b�t�@����ɂȂ��
 *  �ő҂D
 */

#define	MAX_FLUSH_LOOP	1000000

int
serial_shutdown(int portid, int flush)
{
	SPCB	*p;
	int	i;

	if (sysstat() & TTS_INDP) {		/* �R���e�L�X�g�̃`�F�b�N */
		return(E_CTX);
	}
	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* �|�[�g�ԍ��̃`�F�b�N */
	}

	p = get_spcb(portid);
	if (!(p->init_flag)) {			/* �������ς��̃`�F�b�N */
		return(E_OBJ);
	}

	ENTER_EXTENDED_SVC;

	/*
	 *  �o�b�t�@�̃t���b�V������
	 */
	if (flush) {
		for (i = 0; i < MAX_FLUSH_LOOP; i++) {
			if (p->out_write_ptr == p->out_read_ptr) {
				break;
			}
		}
	}

	/*
	 *  �n�[�h�E�F�A�ˑ��̃V���b�g�_�E������
	 */
	syscall(loc_cpu());
	raw_port_shutdown(&(p->rawport));
	syscall(unl_cpu());

	/*
	 *  �Z�}�t�H�̍폜
	 */
	syscall(del_sem(p->in_sem_id));
	syscall(del_sem(p->out_sem_id));

	/*
	 *  �o�b�t�@�̈�̉��
	 */
	syscall(rel_blk(TMPL_OS, p->in_buffer));

	p->init_flag = 0;

	EXIT_EXTENDED_SVC;
	return(E_OK);
}

/*
 *  �t���[�R���g���[���֌W�̒�`
 */
#define	STOP	'\023'		/* Control-S */
#define	START	'\021'		/* Control-Q */

#define	IXOFF_STOP	64	/* buffer area size to send STOP */
#define	IXOFF_START	128	/* buffer area size to send START */

#define	in_buf_area(p)	((p->in_read_ptr >= p->in_write_ptr) ?		\
			 (p->in_read_ptr - p->in_write_ptr) :		\
			 (p->in_read_ptr + SERIAL_BUFSZ - p->in_write_ptr))

/*
 *  ���[�e�B���e�B���[�`��
 */

Inline BOOL
enable_send(SPCB *p, char c)
{
	if (!(p->send_enabled)) {
		raw_port_sendstart(&(p->rawport));
		p->send_enabled = 1;
		if (raw_port_putready(&(p->rawport))) {
			raw_port_putchar(&(p->rawport), c);
			return(1);
		}
	}
	return(0);
}

/*
 *  �V���A���|�[�g����̎�M
 */

int
serial_read(int portid, char *buf, unsigned int len)
{
	SPCB	*p;
	char	c;
	int	i;

	if (sysstat() & TTS_INDP) {		/* �R���e�L�X�g�̃`�F�b�N */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* �|�[�g�ԍ��̃`�F�b�N */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* �������ς��̃`�F�b�N */
		return(E_OBJ);
	}

	ENTER_EXTENDED_SVC;
	for (i = 0; i < len; i++) {
		if ((c = serial_read_one(p)) == '\r' && p->ctl.input != RAW) {
			c = '\n';
		}
		if (p->ctl.echo) {
			serial_write_one(p, c);
		}
		*buf++ = c;
		if (p->ctl.input == RAW
				|| (p->ctl.input == CANONICAL && c == '\n')) {
			return(i + 1);
		}
	}
	EXIT_EXTENDED_SVC;
	return(len);
}

static char
serial_read_one(SPCB *p)
{
	char	c;

	syscall(wai_sem(p->in_sem_id));
	syscall(loc_cpu());

	assert(p->in_read_ptr != p->in_write_ptr);
	c = *(p->in_buffer + p->in_read_ptr);
	INC(p->in_read_ptr);

	if (p->ixoff_stopped && (in_buf_area(p) > IXOFF_START)) {
		if (!enable_send(p, START)) {
			p->ixoff_send = START;
		}
		p->ixoff_stopped = 0;
	}

	syscall(unl_cpu());
	return(c);
}

/*
 *  �V���A���|�[�g�ւ̑��M
 */

int
serial_write(int portid, char *buf, unsigned int len)
{
	SPCB	*p;
	int	i;

	if (sysstat() & TTS_INDP) {		/* �R���e�L�X�g�̃`�F�b�N */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* �|�[�g�ԍ��̃`�F�b�N */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* �������ς��̃`�F�b�N */
		return(E_OBJ);
	}

	ENTER_EXTENDED_SVC;
	for (i = 0; i < len; i++) {
		serial_write_one(p, *buf++);
	}
	EXIT_EXTENDED_SVC;
	return(len);
}

static void
serial_write_one(SPCB *p, char c)
{
	if (c == '\n' && p->ctl.newline) {
		serial_write_one(p, '\r');
	}

	syscall(wai_sem(p->out_sem_id));
	syscall(loc_cpu());

	if (!(p->ixon_stopped) && enable_send(p, c)) {
		syscall(sig_sem(p->out_sem_id));
	}
	else {
		*(p->out_buffer + p->out_write_ptr) = c;
		INC(p->out_write_ptr);
		assert(p->out_read_ptr != p->out_write_ptr);
	}

	syscall(unl_cpu());
}

/*
 *  �V���A���|�[�g�̐���
 */

int
serial_ioctl(int portid, int req, int arg)
{
	SPCB	*p;
	ER	ercd = E_OK;

	if (sysstat() & TTS_INDP) {		/* �R���e�L�X�g�̃`�F�b�N */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* �|�[�g�ԍ��̃`�F�b�N */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* �������ς��̃`�F�b�N */
		return(E_OBJ);
	}

	ENTER_EXTENDED_SVC;
	switch (req) {
	case ECHO:
		p->ctl.echo = arg;
		break;
	case INPUT:
		p->ctl.input = arg;
		break;
	case NEWLINE:
		p->ctl.newline = arg;
		break;
	case FLOWC:
		p->ctl.flowc = arg;
		break;
	case ECHO|GETCTL:
		return(p->ctl.echo);
	case INPUT|GETCTL:
		return(p->ctl.input);
	case NEWLINE|GETCTL:
		return(p->ctl.newline);
	case FLOWC|GETCTL:
		return(p->ctl.flowc);
	default:
		ercd = E_PAR;
	}
	EXIT_EXTENDED_SVC;
	return(ercd);
}

/*
 *  �V���A���|�[�g�����݃n���h��
 */

static void
serial_int_handler(int portid)
{
	SPCB	*p;
	char	c;

	p = get_spcb(portid);
	raw_port_clear_int(&(p->rawport));

	if (raw_port_getready(&(p->rawport))) {
		c = raw_port_getchar(&(p->rawport));
		if ((p->ctl.flowc & IXON) && c == STOP) {
			p->ixon_stopped = 1;
		}
		else if (((p->ctl.flowc & IXON) || p->ixon_stopped)
				&& (c == START || (p->ctl.flowc & IXANY))) {
			p->ixon_stopped = 0;
			if (!(p->send_enabled)) {
				raw_port_sendstart(&(p->rawport));
				p->send_enabled = 1;
			}
		}
		else if (inc(p->in_write_ptr) == p->in_read_ptr) {
			/* �o�b�t�@�t���̏ꍇ�C��M�����������̂Ă�D*/
		}
		else {
			*(p->in_buffer + p->in_write_ptr) = c;
			INC(p->in_write_ptr);
			syscall(sig_sem(p->in_sem_id));

			if ((p->ctl.flowc & IXOFF) && !(p->ixoff_stopped)
					&& (in_buf_area(p) < IXOFF_STOP)) {
				p->ixoff_stopped = 1;
				if (!(p->send_enabled)) {
					raw_port_sendstart(&(p->rawport));
					p->send_enabled = 1;
				}
				p->ixoff_send = STOP;
			}
		}
	}

	if (raw_port_putready(&(p->rawport))) {
		if (!p->send_enabled) {
			raw_port_sendstop(&(p->rawport));
		}
		else if (p->ixoff_send) {
			raw_port_putchar(&(p->rawport), p->ixoff_send);
			p->ixoff_send = 0;
		}
		else if (p->ixon_stopped
				|| p->out_read_ptr == p->out_write_ptr) {
			/* ���M��~ */
			raw_port_sendstop(&(p->rawport));
			p->send_enabled = 0;
		}
		else {
			raw_port_putchar(&(p->rawport),
					*(p->out_buffer + p->out_read_ptr));
			INC(p->out_read_ptr);
			syscall(sig_sem(p->out_sem_id));
		}
	}
}
