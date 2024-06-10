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
 *  @(#) $Id: bsd_nbeio.c,v 1.6 1998/07/06 14:21:25 hiro Exp $
 */

/*
 *  BSD UNIX$BMQ(B $B%N%s%V%m%C%-%s%0(BI/O$B%i%$%V%i%j(B (errno $B$rJV$9%P!<%8%g%s(B)
 */

#include "systask.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <bsd_sigio.h>

/*
 *  errno $B$,%W%m%;%9A4BN$G6&M-$5$l$kJQ?t$J$N$G!$%7%9%F%`%3!<%k$r8F$S=P(B
 *  $B$7$?8e!$(Berrno $B$rFI$`$^$G$O3d9~$_$r6X;_$7$F$$$k!%(B
 *
 *  $B%N%s%V%m%C%-%s%0%b!<%I$G$"$k$3$H$H!$$9$Y$F$N%7%0%J%k$r%^%9%/$7$F$$(B
 *  $B$k$3$H$+$i!$(BEINTR $B$OJV$i$J$$$O$:$J$N$G!$BP=h$7$F$$$J$$!%(B
 */

/*
 *  nbe_open: $B%N%s%V%m%C%-%s%0%3!<%k8_49$N(B open
 *
 *  open $B8e$K%N%s%V%m%C%-%s%0%b!<%I$K@_Dj$9$k!%(Bopen $B$G$O%V%m%C%/$7$J$$!%(B
 */

int
nbe_open(int *p_errno, char *path, int flags, mode_t mode)
{
	int	fd;

	syscall(loc_cpu());
	fd = open(path, (flags | O_NDELAY), mode);
	*p_errno = errno;
	syscall(unl_cpu());
	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, FASYNC|FNDELAY);
	return(fd);
}

/*
 *  nbe_close: $B%N%s%V%m%C%-%s%0%3!<%k8_49$N(B close
 *
 *  close $B$G$O%V%m%C%/$7$J$$$?$a!$(Berrno $B$rJV$9ItJ,$@$1$,I8=`$N(B close 
 *  $B$H0[$J$k!%(B
 */

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

/*
 *  nbe_read: $B%N%s%V%m%C%-%s%0$N(B read
 */

typedef struct nbe_read_args {
	ID	tskid;		/* nbe_read $B$r8F$S=P$7$?%?%9%/$N(B ID */
	int	*p_result;	/* read $B$NJV$jCM$rF~$l$kJQ?t$X$N%]%$%s%?(B */
	int	*p_errno;	/* errno $B$rF~$l$kJQ?t$X$N%]%$%s%?(B */
	int	fd;		/* read $B$NBh(B1$B0z?t(B */
	void	*buf;		/* read $B$NBh(B2$B0z?t(B */
	size_t	nbytes;		/* read $B$NBh(B3$B0z?t(B */
} READ_ARGS;

static BOOL
nbe_read_callback(READ_ARGS *args)
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
		*(args->p_errno) = my_errno;
		syscall(wup_tsk(args->tskid));
		return(1);
	}
}

int
nbe_read(int *p_errno, int fd, void *buf, size_t nbytes)
{
	int		result;
	SIGIOEB		sigioeb;
	READ_ARGS	args;

	syscall(get_tid(&(args.tskid)));
	args.p_result = &result;
	args.p_errno = p_errno;
	args.fd = fd;
	args.buf = buf;
	args.nbytes = nbytes;
	if (!nbe_read_callback(&args)) {
		sigioeb.callback = (SIGIO_CBACK) nbe_read_callback;
		sigioeb.arg = &args;
		syscall(enqueue_sigioeb(&sigioeb));
	}
	syscall(slp_tsk());
	return(result);
}

/*
 *  nbe_write: $B%N%s%V%m%C%-%s%0$N(B write
 */

typedef struct nbe_write_args {
	ID	tskid;		/* nbe_write $B$r8F$S=P$7$?%?%9%/$N(B ID */
	int	*p_result;	/* write $B$NJV$jCM$rF~$l$kJQ?t$X$N%]%$%s%?(B */
	int	*p_errno;	/* errno $B$rF~$l$kJQ?t$X$N%]%$%s%?(B */
	int	fd;		/* write $B$NBh(B1$B0z?t(B */
	void	*buf;		/* write $B$NBh(B2$B0z?t(B */
	size_t	nbytes;		/* write $B$NBh(B3$B0z?t(B */
} WRITE_ARGS;

static BOOL
nbe_write_callback(WRITE_ARGS *args)
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
		*(args->p_errno) = my_errno;
		syscall(wup_tsk(args->tskid));
		return(1);
	}
}

int
nbe_write(int *p_errno, int fd, void *buf, size_t nbytes)
{
	int		result;
	SIGIOEB		sigioeb;
	WRITE_ARGS	args;

	syscall(get_tid(&(args.tskid)));
	args.p_result = &result;
	args.p_errno = p_errno;
	args.fd = fd;
	args.buf = buf;
	args.nbytes = nbytes;
	if (!nbe_write_callback(&args)) {
		sigioeb.callback = (SIGIO_CBACK) nbe_write_callback;
		sigioeb.arg = &args;
		syscall(enqueue_sigioeb(&sigioeb));
	}
	syscall(slp_tsk());
	return(result);
}
