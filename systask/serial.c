/**
 * 
 * 	ItIs - An ITRON Implementation for Research and Education
 * 
 * Copyright (C) 1989-1997 by Sakamura Laboratory, Univ. of Tokyo, JAPAN
 * Copyright (C) 1997-1998 by Embedded and Real-Time Systems Laboratory,
 * 				Toyohashi Univ. of Technology, JAPAN
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the universities nor the names of the laboratories
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITIES OR THE LABORATORIES BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: serial.c,v 1.14 1998/01/30 09:58:16 hiro Exp $
 */

/*
 *  $B%7%j%"%k%$%s%?%U%'!<%9%I%i%$%P(B
 */

#include "systask.h"
#include "sys_serial.h"
#include <itis_ioctl.h>

#define	assert(exp)

/*
 *  $B%3%s%=!<%kMQ$KMQ$$$k%7%j%"%k%]!<%HHV9f(B
 */
static int	console_portid;

/*
 *  $B%7%j%"%k%$%s%?%U%'!<%9%I%i%$%P$N5/F0(B
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
 *  $B%7%j%"%k%]!<%H4IM}%V%m%C%/$NDj5A(B
 */

typedef struct ioctl_descripter {
	int	echo;
	int	input;
	int	newline;
	int	flowc;
} IOCTL;

#define	SERIAL_BUFSZ	256	/* $B%7%j%"%k%$%s%?%U%'!<%9MQ%P%C%U%!$N%5%$%:(B */

#define	inc(x)		(((x)+1 < SERIAL_BUFSZ) ? (x)+1 : 0)
#define	INC(x)		((++(x) < SERIAL_BUFSZ) ? (x) : ((x) = 0))

typedef struct serial_port_control_block {
	int	init_flag;	/* $B=i4|2=:Q$+!)(B */
	RPORT	rawport;	/* $B%O!<%I%&%'%"0MB8>pJs(B */
	char	*in_buffer;	/* $B<u?.%P%C%U%!%(%j%"$N@hF,(B */
	ID	in_sem_id;	/* $B<u?.%P%C%U%!4IM}MQ%;%^%U%)$N(B ID */
	int	in_read_ptr;	/* $B<u?.%P%C%U%!FI$_=P$7%]%$%s%?(B */
	int	in_write_ptr;	/* $B<u?.%P%C%U%!=q$-9~$_%]%$%s%?(B */
	char	*out_buffer;	/* $BAw?.%P%C%U%!%(%j%"$N@hF,(B */
	ID	out_sem_id;	/* $BAw?.%P%C%U%!4IM}MQ%;%^%U%)$N(B ID */
	int	out_read_ptr;	/* $BAw?.%P%C%U%!FI$_=P$7%]%$%s%?(B */
	int	out_write_ptr;	/* $BAw?.%P%C%U%!=q$-9~$_%]%$%s%?(B */
	IOCTL	ctl;		/* ioctl $B$K$h$k@_DjFbMF(B */
	BOOL	send_enabled;	/* $BAw?.$r%$%M!<%V%k$7$F$"$k$+!)(B */
	BOOL	ixon_stopped;	/* STOP $B$r<u$1<h$C$?>uBV$+!)(B */
	BOOL	ixoff_stopped;	/* $BAj<j$K(B STOP $B$rAw$C$?>uBV$+!)(B */
	char	ixoff_send;	/* $BAj<j$K(B START/STOP $B$rAw$k$+!)(B */
} SPCB;

/*
 *  $B%b%8%e!<%kFb$G;H$&4X?t(B
 */
static char	serial_read_one(SPCB *p);
static void	serial_write_one(SPCB *p, char c);

/*
 *  $B%7%j%"%k%]!<%H4IM}%V%m%C%/$NDj5A$H=i4|2=(B
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
 *  $B%]!<%H$N=i4|2=(B
 */

int
serial_init(int portid)
{
	SPCB	*p;
	VP	buffer;
	T_DINT	pk_dint;
	ER	ercd = E_OK;

	if (sysstat() & TTS_INDP) {		/* $B%3%s%F%-%9%H$N%A%'%C%/(B */
		return(E_CTX);
	}
	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);
	}

	p = get_spcb(portid);
	if (p->init_flag) {			/* $B=i4|2=:Q$+$N%A%'%C%/(B */
		return(E_OK);
	}

	ENTER_EXTENDED_SVC;

	/*
	 *  $B%P%C%U%!NN0h$N3NJ](B ($B%7%9%F%`%a%b%j%W!<%k$+$i<h$k(B)
	 */
	if (pget_blk(&buffer, TMPL_OS, SERIAL_BUFSZ * 2) != E_OK) {
		ercd = E_NOMEM;
		goto error_exit;
	}
	p->in_buffer = (char *) buffer;
	p->out_buffer = ((char *) buffer) + SERIAL_BUFSZ;

	/*
	 *  $BJQ?t$N=i4|2=(B
	 */
	p->in_read_ptr = p->in_write_ptr = 0;
	p->out_read_ptr = p->out_write_ptr = 0;
	p->ixon_stopped = p->ixoff_stopped = 0;
	p->ixoff_send = 0;

	/*
	 *  $B3d9~$_%O%s%I%i$NDj5A(B
	 */
	pk_dint.intatr = TA_HLNG;
	pk_dint.inthdr = raw_int_handler(&(p->rawport));
	syscall(def_int(raw_int_vector(&(p->rawport)), &pk_dint));

	/*
	 *  $B%;%^%U%)$N@8@.(B
	 */
	syscall(cre_sem(p->in_sem_id,
		&((T_CSEM) { 0, TA_TPRI, 0, SERIAL_BUFSZ-1 })));
	syscall(cre_sem(p->out_sem_id,
		&((T_CSEM) { 0, TA_TPRI, SERIAL_BUFSZ-1, SERIAL_BUFSZ-1 })));

	/*
	 *  $B%O!<%I%&%'%"0MB8$N=i4|2=(B
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
 *  $B%]!<%H$N%7%c%C%H%@%&%s(B
 *
 *  flush $B$,(B TRUE $B$N>l9g$O!$%7%j%"%k%]!<%H$X$NAw?.%P%C%U%!$,6u$K$J$k$^(B
 *  $B$GBT$D!%(B
 */

#define	MAX_FLUSH_LOOP	1000000

int
serial_shutdown(int portid, int flush)
{
	SPCB	*p;
	int	i;

	if (sysstat() & TTS_INDP) {		/* $B%3%s%F%-%9%H$N%A%'%C%/(B */
		return(E_CTX);
	}
	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* $B%]!<%HHV9f$N%A%'%C%/(B */
	}

	p = get_spcb(portid);
	if (!(p->init_flag)) {			/* $B=i4|2=:Q$+$N%A%'%C%/(B */
		return(E_OBJ);
	}

	ENTER_EXTENDED_SVC;

	/*
	 *  $B%P%C%U%!$N%U%i%C%7%e=hM}(B
	 */
	if (flush) {
		for (i = 0; i < MAX_FLUSH_LOOP; i++) {
			if (p->out_write_ptr == p->out_read_ptr) {
				break;
			}
		}
	}

	/*
	 *  $B%O!<%I%&%'%"0MB8$N%7%c%C%H%@%&%s=hM}(B
	 */
	syscall(loc_cpu());
	raw_port_shutdown(&(p->rawport));
	syscall(unl_cpu());

	/*
	 *  $B%;%^%U%)$N:o=|(B
	 */
	syscall(del_sem(p->in_sem_id));
	syscall(del_sem(p->out_sem_id));

	/*
	 *  $B%P%C%U%!NN0h$N2rJ|(B
	 */
	syscall(rel_blk(TMPL_OS, p->in_buffer));

	p->init_flag = 0;

	EXIT_EXTENDED_SVC;
	return(E_OK);
}

/*
 *  $B%U%m!<%3%s%H%m!<%k4X78$NDj5A(B
 */
#define	STOP	'\023'		/* Control-S */
#define	START	'\021'		/* Control-Q */

#define	IXOFF_STOP	64	/* buffer area size to send STOP */
#define	IXOFF_START	128	/* buffer area size to send START */

#define	in_buf_area(p)	((p->in_read_ptr >= p->in_write_ptr) ?		\
			 (p->in_read_ptr - p->in_write_ptr) :		\
			 (p->in_read_ptr + SERIAL_BUFSZ - p->in_write_ptr))

/*
 *  $B%f!<%F%#%j%F%#%k!<%A%s(B
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
 *  $B%7%j%"%k%]!<%H$+$i$N<u?.(B
 */

int
serial_read(int portid, char *buf, unsigned int len)
{
	SPCB	*p;
	char	c;
	int	i;

	if (sysstat() & TTS_INDP) {		/* $B%3%s%F%-%9%H$N%A%'%C%/(B */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* $B%]!<%HHV9f$N%A%'%C%/(B */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* $B=i4|2=:Q$+$N%A%'%C%/(B */
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
 *  $B%7%j%"%k%]!<%H$X$NAw?.(B
 */

int
serial_write(int portid, char *buf, unsigned int len)
{
	SPCB	*p;
	int	i;

	if (sysstat() & TTS_INDP) {		/* $B%3%s%F%-%9%H$N%A%'%C%/(B */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* $B%]!<%HHV9f$N%A%'%C%/(B */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* $B=i4|2=:Q$+$N%A%'%C%/(B */
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
 *  $B%7%j%"%k%]!<%H$N@)8f(B
 */

int
serial_ioctl(int portid, int req, int arg)
{
	SPCB	*p;
	ER	ercd = E_OK;

	if (sysstat() & TTS_INDP) {		/* $B%3%s%F%-%9%H$N%A%'%C%/(B */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* $B%]!<%HHV9f$N%A%'%C%/(B */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* $B=i4|2=:Q$+$N%A%'%C%/(B */
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
 *  $B%7%j%"%k%]!<%H3d9~$_%O%s%I%i(B
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
			/* $B%P%C%U%!%U%k$N>l9g!$<u?.$7$?J8;z$r<N$F$k!%(B*/
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
			/* $BAw?.Dd;_(B */
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
