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
 *  @(#) $Id: bsd_nbio.c,v 1.6 1998/07/06 14:21:26 hiro Exp $
 */

/*
 *  BSD UNIX用 ノンブロッキングI/Oライブラリ
 */

#include "systask.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <bsd_sigio.h>

/*
 *  errno がプロセス全体で共有される変数なので，システムコールを呼び出
 *  した後，errno を読むまでは割込みを禁止している．
 *
 *  ノンブロッキングモードであることと，すべてのシグナルをマスクしてい
 *  ることから，EINTR は返らないはずなので，対処していない．
 */

/*
 *  nb_open: ノンブロッキングコール互換の open
 *
 *  open 後にノンブロッキングモードに設定する．open ではブロックしない．
 */

int
nb_open(char *path, int flags, mode_t mode)
{
	int	fd;

	fd = open(path, (flags | O_NDELAY), mode);
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC|FNDELAY);
	return(fd);
}

/*
 *  nb_close: ノンブロッキングコール互換の close
 *
 *  close ではブロックしないため，標準の close と同じ．
 */

int
nb_close(int fd)
{
	return(close(fd));
}

/*
 *  nb_read: ノンブロッキングの read
 */

typedef struct nb_read_args {
	ID	tskid;		/* nb_read を呼び出したタスクの ID */
	int	*p_result;	/* read の返り値を入れる変数へのポインタ */
	int	fd;		/* read の第1引数 */
	void	*buf;		/* read の第2引数 */
	size_t	nbytes;		/* read の第3引数 */
} READ_ARGS;

static BOOL
nb_read_callback(READ_ARGS *args)
{
	int	result, my_errno;

	syscall(loc_cpu());
	result = read(args->fd, args->buf, args->nbytes);
	my_errno = errno;
	syscall(unl_cpu());

	if (result < 0 && my_errno == EWOULDBLOCK) {
		return(0);
	}
	else {
		*(args->p_result) = result;
		syscall(wup_tsk(args->tskid));
		return(1);
	}
}

int
nb_read(int fd, void *buf, size_t nbytes)
{
	int		result;
	SIGIOEB		sigioeb;
	READ_ARGS	args;

	syscall(get_tid(&(args.tskid)));
	args.p_result = &result;
	args.fd = fd;
	args.buf = buf;
	args.nbytes = nbytes;
	if (!nb_read_callback(&args)) {
		sigioeb.callback = (SIGIO_CBACK) nb_read_callback;
		sigioeb.arg = &args;
		syscall(enqueue_sigioeb(&sigioeb));
	}
	syscall(slp_tsk());
	return(result);
}

/*
 *  nb_write: ノンブロッキングの write
 */

typedef struct nb_write_args {
	ID	tskid;		/* nb_write を呼び出したタスクの ID */
	int	*p_result;	/* write の返り値を入れる変数へのポインタ */
	int	fd;		/* write の第1引数 */
	void	*buf;		/* write の第2引数 */
	size_t	nbytes;		/* write の第3引数 */
} WRITE_ARGS;

static BOOL
nb_write_callback(WRITE_ARGS *args)
{
	int	result, my_errno;

	syscall(loc_cpu());
	result = write(args->fd, args->buf, args->nbytes);
	my_errno = errno;
	syscall(unl_cpu());

	if (result < 0 && my_errno == EWOULDBLOCK) {
		return(0);
	}
	else {
		*(args->p_result) = result;
		syscall(wup_tsk(args->tskid));
		return(1);
	}
}

int
nb_write(int fd, void *buf, size_t nbytes)
{
	int		result;
	SIGIOEB		sigioeb;
	WRITE_ARGS	args;

	syscall(get_tid(&(args.tskid)));
	args.p_result = &result;
	args.fd = fd;
	args.buf = buf;
	args.nbytes = nbytes;
	if (!nb_write_callback(&args)) {
		sigioeb.callback = (SIGIO_CBACK) nb_write_callback;
		sigioeb.arg = &args;
		syscall(enqueue_sigioeb(&sigioeb));
	}
	syscall(slp_tsk());
	return(result);
}
