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
 *  @(#) $Id: bsd_nbeio.c,v 1.4 1996/02/17 09:35:29 hiro Exp $
 */

/*
 *  BSD UNIX用 ノンブロッキングI/Oライブラリ (errno を返すバージョン)
 */

#include <stdarg.h>
#include "systask.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
 *  errno がプロセス全体で共有される変数なので，システムコールを呼び出
 *  した後，errno を読むまでは割込みを禁止している．
 *
 *  ノンブロッキングモードであることと，すべてのシグナルをマスクしてい
 *  ることから，EINTR は返らないはずなので，対処していない．
 */

#define NONBLOCK(p_errno, func) \
{								\
	UINT	flgptn;						\
								\
	syscall(loc_cpu());					\
	while ((func) < 0 && errno == EWOULDBLOCK) {		\
		syscall(unl_cpu());				\
		syscall(wai_flg(&flgptn, FLG_BSD_SIGIO, 1, 0));	\
		syscall(loc_cpu());				\
	}							\
	*p_errno = errno;					\
	syscall(unl_cpu());					\
}

int
nbe_open(int *p_errno, char *path, int flags, ...)
{
	mode_t	mode;
	int	fd;

	if (flags & O_CREAT) {
		va_list arglist;
		va_start(arglist, flags);
		mode = va_arg(arglist, mode_t);
		va_end(arglist);
	}
	else {
		mode = 0;
	}
	NONBLOCK(p_errno, fd = open(path, (flags | O_NDELAY), mode));
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC|FNDELAY);
	return(fd);
}

int
nbe_close(int *p_errno, int fd)
{
	int	result;

	syscall(loc_cpu());
	result = close(fd);
	*p_errno = errno;
	syscall(unl_cpu());
	return(result);
}

int
nbe_socket(int *p_errno, int domain, int type, int protocol)
{
	int	fd;

	syscall(loc_cpu());
	fd = socket(domain, type, protocol);
	*p_errno = errno;
	syscall(unl_cpu());
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC|FNDELAY);
	return(fd);
}

int
nbe_accept(int *p_errno, int s, struct sockaddr *addr, int *addrlen)
{
	int	fd;

	NONBLOCK(p_errno, fd = accept(s, addr, addrlen));
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC|FNDELAY);
	return(fd);
}

int
nbe_read(int *p_errno, int fd, char *buf, int len)
{
	int	n;
	UINT	flgptn;

	NONBLOCK(p_errno, n = read(fd, buf, len));
	return(n);
}

int
nbe_write(int *p_errno, int fd, char *buf, int len)
{
	int	n;
	UINT	flgptn;

	NONBLOCK(p_errno, n = write(fd, buf, len));
	return(n);
}
