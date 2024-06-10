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
 *  @(#) $Id: syslog.c,v 1.6 1998/01/30 09:58:32 hiro Exp $
 */

#include <stdarg.h>
#include <itis_services.h>
#include <itis_syslog.h>
#ifndef LINK_KERNEL
#include <svc_syslog.h>
#endif /* LINK_KERNEL */

/*
 *  アプリケーション用システムログ出力ライブラリ
 */

#define	FORMAT_BUFSIZ	1024

int	__logmask = LOG_UPTO(LOG_NOTICE);

void
syslog(int priority, const char *format, ...)
{
	char	buf[FORMAT_BUFSIZ];
	va_list	ap;
	int	len;

	if (LOG_MASK(priority) & __logmask) {
		va_start(ap, format);
#ifdef LINK_KERNEL
		len = itis_vsprintf(buf, format, ap);
		svc_syslog_send(buf, len);
#else /* LINK_KERNEL */
		len = vsprintf(buf, format, ap);
		syslog_send(buf, len);
#endif /* LINK_KERNEL */
		va_end(ap);
	}
}
