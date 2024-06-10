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
 *  @(#) $Id: glue_newlib.c,v 1.1 1998/07/06 11:03:58 hiro Exp $
 */

/*
 *  newlib 対応の低レベル入出力ルーチン
 *
 *  ItIs 用の libgloss に対応している．
 */

#ifdef LINK_KERNEL
#include "systask.h"
#else /* LINK_KERNEL */
#include <itis_services.h>
#include <svc_serial.h>
#endif /* LINK_KERNEL */

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int open(const char *buf, int flags, int mode)
{
	errno = EIO;
	return(-1);
}

int close(int fd)
{
	return(0);
}

int
read(int fd, char *buf, int len)
{
	return(serial_read(0, buf, len));
}

int
write(int fd, char *buf, int len)
{
	return(serial_write(0, buf, len));
}

int
ioctl(int fd, int req, int arg)
{
	return(serial_ioctl(0, req, arg));
}

int lseek(int fd, off_t offset, int whence)
{
	errno = ESPIPE;
	return((off_t) -1);
}

int fstat(int fd, struct stat *buf)
{
	buf->st_mode = S_IFCHR;
	buf->st_blksize = 0;
	return(0);
}

int isatty(int fd)
{
	return(1);
}

int unlink(char *path)
{
	errno = EIO;
	return(-1);
}

#ifdef NEWLIB

void
_exit(int status)
{
	wup_tsk(TSK_INIT);
}

#else /* LINK_KERNEL */

extern char _end;
static char *heap_ptr = &_end;

char *
sbrk(int nbytes)
{
	char *base;

	base = heap_ptr;
	heap_ptr += nbytes;
	return(base);
}

void
_exit(int status)
{
	ext_tsk();
}

#endif /* LINK_KERNEL */
