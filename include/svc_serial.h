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
 *  @(#) $Id: svc_serial.h,v 1.8 1996/02/17 09:26:56 hiro Exp $
 */

#ifndef	_SVC_SERIAL_
#define	_SVC_SERIAL_

#include <itis_stddefs.h>
#include <itis_scint.h>
#include <svc_fncode.h>

#define CONSOLE_PORT	0	/* コンソールの I/Oポートの指定 */

Inline int
serial_init(int portid)
{
	SYS_CAL_1(SVC_SERIAL_INIT, portid);
}

Inline int
serial_read(int portid, char *buf, unsigned int len)
{
	SYS_CAL_3M(SVC_SERIAL_READ, portid, buf, len);
}

Inline int
serial_write(int portid, char *buf, unsigned int len)
{
	SYS_CAL_3(SVC_SERIAL_WRITE, portid, buf, len);
}

Inline int
serial_ioctl(int portid, int req, int arg)
{
	SYS_CAL_3M(SVC_SERIAL_IOCTL, portid, req, arg);
}

Inline int
serial_shutdown(int portid, int flush)
{
	SYS_CAL_2(SVC_SERIAL_SHUTDOWN, portid, flush);
}

#endif	/* _SVC_SERIAL_ */
