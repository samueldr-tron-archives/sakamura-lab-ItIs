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
 *  @(#) $Id: bsd_serial.c,v 1.6 1996/02/17 09:38:05 hiro Exp $
 */

/*
 *  BSD UNIX用 疑似シリアルドライバ
 */

#define _BSD_SERIAL_
#include "systask.h"
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <itis_ioctl.h>

#define	assert(exp)

/*
 *  コンソール用に用いるシリアルポート番号
 */
static int	console_portid;

/*
 *  シリアルインタフェースドライバの起動
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

	syscall(cre_tsk(TSK_BSD_SERIAL, &TSK_BSD_SERIAL_CTSK));
	syscall(sta_tsk(TSK_BSD_SERIAL, 0));

	syslog(LOG_NOTICE, "Serial driver service starts on port %d.\r",
		 portid);
}

/*
 *  シリアルポートの低レベル定義
 */

typedef struct raw_serial_port_descripter {
	char		*path;		/* UNIX 上でのファイル名 */
	int		fd;		/* ファイルディスクリプタ */
	struct sgttyb	current_ttyb;	/* 端末制御情報 */
	struct sgttyb	saved_ttyb;
} RPORT;

#define NUM_PORT	1

#define RAWPORT1	{ "/dev/tty" }

/*
 *  UNIX レベルのポート初期化/シャットダウン処理
 */

Inline void
raw_port_init(RPORT *p)
{
	p->fd = open(p->path, O_RDWR|O_NDELAY);
	fcntl(p->fd, F_SETOWN, getpid());
	fcntl(p->fd, F_SETFL, FASYNC|FNDELAY);
	ioctl(p->fd, TIOCGETP, &(p->saved_ttyb));
	p->current_ttyb = p->saved_ttyb;
	set_rawmode(&(p->current_ttyb));
	ioctl(p->fd, TIOCSETP, &(p->current_ttyb));
}

Inline void
raw_port_shutdown(RPORT *p)
{
	ioctl(p->fd, TIOCSETP, &(p->saved_ttyb));
	fcntl(p->fd, F_SETFL, 0);
	close(p->fd);
}

/*
 *  シリアルポート管理ブロックの定義
 */

typedef struct ioctl_descripter {
	int	echo;
	int	input;
	int	newline;
	int	flowc;
} IOCTL;

#define	SERIAL_BUFSZ	256	/* シリアルインタフェース用バッファのサイズ */

#define	inc(x)		(((x)+1 < SERIAL_BUFSZ) ? (x)+1 : 0)
#define	INC(x)		((++(x) < SERIAL_BUFSZ) ? (x) : ((x) = 0))

typedef struct serial_port_control_block {
	int	init_flag;	/* 初期化済か？ */
	RPORT	rawport;	/* ハードウェア依存情報 */
	char	*in_buffer;	/* 受信バッファエリアの先頭 */
	ID	in_sem_id;	/* 受信バッファ管理用セマフォの ID */
	int	in_read_ptr;	/* 受信バッファ読み出しポインタ */
	int	in_write_ptr;	/* 受信バッファ書き込みポインタ */
	char	*out_buffer;	/* 送信バッファエリアの先頭 */
	ID	out_sem_id;	/* 送信バッファ管理用セマフォの ID */
	int	out_read_ptr;	/* 送信バッファ読み出しポインタ */
	int	out_write_ptr;	/* 送信バッファ書き込みポインタ */
	IOCTL	ctl;		/* ioctl による設定内容 */
	BOOL	send_enabled;	/* 送信をイネーブルしてあるか？ (使わない) */
	BOOL	ixon_stopped;	/* STOP を受け取った状態か？ */
	BOOL	ixoff_stopped;	/* 相手に STOP を送った状態か？ */
	char	ixoff_send;	/* 相手に START/STOP を送るか？ */
} SPCB;

/*
 *  モジュール内で使う関数
 */
static void	sigint_handler(void);
static char	serial_read_one(SPCB *p);
static void	serial_write_one(SPCB *p, char c);

/*
 *  シリアルポート管理ブロックの定義と初期化
 */

SPCB spcb_table[NUM_PORT] = {
	{ 0, RAWPORT1,
	  0, SEM_SERIAL1_IN, 0, 0,
	  0, SEM_SERIAL1_OUT, 0, 0,
	 { 1, CANONICAL, 1, IXON|IXOFF },
	  0, 0, 0, 0 },
};

#define get_spcb(portid)	(&(spcb_table[(portid)-1]))
#define get_spcb_def(portid)	get_spcb((portid) ? (portid) : console_portid)

/*
 *  ポートの初期化
 */
int
serial_init(int portid)
{
	SPCB	*p;
	VP	buffer;
	T_DINT	pk_dint;
	ER	ercd = E_OK;

	if (sysstat() & TTS_INDP) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);
	}

	p = get_spcb(portid);
	if (p->init_flag) {			/* 初期化済かのチェック */
		return(E_OK);
	}

	ENTER_EXTENDED_SVC;

	/*
	 *  バッファ領域の確保 (システムメモリプールから取る)
	 */
	if (pget_blk(&buffer, TMPL_OS, SERIAL_BUFSZ * 2) != E_OK) {
		ercd = E_NOMEM;
		goto error_exit;
	}
	p->in_buffer = (char *) buffer;
	p->out_buffer = ((char *) buffer) + SERIAL_BUFSZ;

	/*
	 *  変数の初期化
	 */
	p->in_read_ptr = p->in_write_ptr = 0;
	p->out_read_ptr = p->out_write_ptr = 0;
	p->ixon_stopped = p->ixoff_stopped = 0;
	p->ixoff_send = 0;

	/*
	 *  セマフォの生成
	 */
	syscall(cre_sem(p->in_sem_id,
		&((T_CSEM) { 0, TA_TPRI, 0, SERIAL_BUFSZ-1 })));
	syscall(cre_sem(p->out_sem_id,
		&((T_CSEM) { 0, TA_TPRI, SERIAL_BUFSZ-1, SERIAL_BUFSZ-1 })));

	/*
	 *  低レベルの初期化
	 */
	syscall(loc_cpu());
	raw_port_init(&(p->rawport));

	/*
 	 *  プロセスを終了させるシグナルを捕まえる
	 */
	signal(SIGHUP, sigint_handler);
	signal(SIGINT, sigint_handler);
	signal(SIGTERM, sigint_handler);

	syscall(unl_cpu());

	p->init_flag = 1;

    error_exit:
	EXIT_EXTENDED_SVC;
	return(ercd);
}

/*
 *  ポートのシャットダウン
 *
 *  flush が TRUE の場合は，シリアルポートへの送信バッファが空になるま
 *  で待つ．
 */

#define	MAX_FLUSH_LOOP	1000000

int
serial_shutdown(int portid, int flush)
{
	SPCB	*p;
	int	i;

	if (sysstat() & TTS_INDP) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* ポート番号のチェック */
	}

	p = get_spcb(portid);
	if (!(p->init_flag)) {			/* 初期化済かのチェック */
		return(E_OBJ);
	}

	ENTER_EXTENDED_SVC;

	/*
	 *  バッファのフラッシュ処理
	 */
	if (flush) {
		for (i = 0; i < MAX_FLUSH_LOOP; i++) {
			if (p->out_write_ptr == p->out_read_ptr) {
				break;
			}
		}
	}

	/*
	 *  低レベルのシャットダウン処理
	 */
	syscall(loc_cpu());
	raw_port_shutdown(&(p->rawport));
	syscall(unl_cpu());

	/*
	 *  セマフォの削除
	 */
	syscall(del_sem(p->in_sem_id));
	syscall(del_sem(p->out_sem_id));

	/*
	 *  バッファ領域の解放
	 */
	syscall(rel_blk(TMPL_OS, p->in_buffer));

	p->init_flag = 0;

	EXIT_EXTENDED_SVC;
	return(E_OK);
}

/*
 *  プロセスを終了させるシグナルに対するハンドラ
 */
void
sigint_handler()
{
	SPCB	*p;
	int	i;

	for (i = 1; i <= NUM_PORT; i++) {
		p = get_spcb(i);
		if (p->init_flag) {
			raw_port_shutdown(&(p->rawport));
		}
	}
	exit(0);
}

/*
 *  フローコントロール関係の定義
 */
#define	STOP	'\023'		/* Control-S */
#define	START	'\021'		/* Control-Q */

#define	IXOFF_STOP	64	/* buffer area size to send STOP */
#define	IXOFF_START	128	/* buffer area size to send START */

#define	in_buf_area(p)	((p->in_read_ptr >= p->in_write_ptr) ?		\
			 (p->in_read_ptr - p->in_write_ptr) :		\
			 (p->in_read_ptr + SERIAL_BUFSZ - p->in_write_ptr))

/*
 *  ユーティリティルーチン
 */

Inline BOOL
read_char(SPCB *p, char *c)
{
	int	n;

	if ((n = read(p->rawport.fd, c, 1)) == 1) {
		return(1);
	}
	assert(n < 0 && errno == EWOULDBLOCK);
	return(0);
}

Inline BOOL
write_char(SPCB *p, char c)
{
	int	n;

	if ((n = write(p->rawport.fd, &c, 1)) == 1) {
		return(1);
	}
	assert(n < 0 && errno == EWOULDBLOCK);
	return(0);
}

/*
 *  シリアルポートからの受信
 */

int
serial_read(int portid, char *buf, unsigned int len)
{
	SPCB	*p;
	char	c;
	int	i;

	if (sysstat() & TTS_INDP) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* ポート番号のチェック */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* 初期化済かのチェック */
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
	if (inc(p->in_write_ptr) == p->in_read_ptr) {
		/*
		 *  バッファフル状態が解除されたら，シリアルインタフェー
		 *  ス割込みシミュレートタスクを待ち解除する．
		 */
		syscall(rel_wai(TSK_BSD_SERIAL));
	}
	INC(p->in_read_ptr);

	if (p->ixoff_stopped && (in_buf_area(p) > IXOFF_START)) {
		if (!write_char(p, START)) {
			p->ixoff_send = START;
		}
		p->ixoff_stopped = 0;
	}

	syscall(unl_cpu());
	return(c);
}

/*
 * シリアルポートへの送信 */

int
serial_write(int portid, char *buf, unsigned int len)
{
	SPCB	*p;
	int	i;

	if (sysstat() & TTS_INDP) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* ポート番号のチェック */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* 初期化済かのチェック */
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

	if (!(p->ixon_stopped) && write_char(p, c)) {
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
 *  シリアルポートの制御
 */

int
serial_ioctl(int portid, int req, int arg)
{
	SPCB	*p;
	ER	ercd = E_OK;

	if (sysstat() & TTS_INDP) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_PAR);			/* ポート番号のチェック */
	}

	p = get_spcb_def(portid);
	if (!(p->init_flag)) {			/* 初期化済かのチェック */
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
 *  シリアルポート割込みハンドラ
 */

static BOOL
serial_int_handler(int portid)
{
	SPCB	*p;
	BOOL	flag;
	char	c;

	p = get_spcb(portid);
	flag = 0;

	/*
	 *  1文字受信処理
	 *
	 *  まず，バッファフルでない場合に，1文字読んでみる．読めれば，
	 *  それに応じた処理を行う．
	 */
	if (inc(p->in_write_ptr) != p->in_read_ptr && read_char(p, &c)) {
		if ((p->ctl.flowc & IXON) && c == STOP) {
			p->ixon_stopped = 1;
		}
		else if (((p->ctl.flowc & IXON) || p->ixon_stopped)
				&& (c == START || (p->ctl.flowc & IXANY))) {
			p->ixon_stopped = 0;
		}
		else {
			*(p->in_buffer + p->in_write_ptr) = c;
			INC(p->in_write_ptr);
			syscall(sig_sem(p->in_sem_id));

			if ((p->ctl.flowc & IXOFF) && !(p->ixoff_stopped)
					&& (in_buf_area(p) < IXOFF_STOP)) {
				p->ixoff_stopped = 1;
				p->ixoff_send = STOP;
			}
		}
		flag = 1;
	}

	/*
	 *  1文字送信処理
	 */
	if (p->ixoff_send) {
		if (write_char(p, p->ixoff_send)) {
			p->ixoff_send = 0;
			flag = 1;
		}
	}
	else if (!(p->ixon_stopped) && p->out_read_ptr != p->out_write_ptr) {
		if (write_char(p, *(p->out_buffer + p->out_read_ptr))) {
			INC(p->out_read_ptr);
			syscall(sig_sem(p->out_sem_id));
			flag = 1;
		}
	}
	return(flag);
}

/*
 *  シリアルインタフェース割込みシミュレートタスク
 */

void
bsd_serial_task(void)
{
	BOOL	flag;
	UINT	flgptn;
	ER	ercd;

	for (;;) {
		ercd = wai_flg(&flgptn, FLG_BSD_SIGIO, 1, 0);
		if (ercd < 0 && ercd != E_RLWAI) {
			itis_perror(__FILE__, __LINE__-2, "wai_flg", ercd);
		}
		do {
			syscall(loc_cpu());
			flag = serial_int_handler(1);
			syscall(unl_cpu());
		} while (flag);
	}
	ext_tsk();
}
