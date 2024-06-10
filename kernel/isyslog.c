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
 *  @(#) $Id: isyslog.c,v 1.8 1998/01/30 09:52:31 hiro Exp $
 */

#include <stdarg.h>
#include "itis_kernel.h"
#include "task.h"

/*
 *  $B%m%0%?%9%/$NJQ?t(B
 *
 *  $B%?%9%/$NJQ?t$r%+!<%M%k$,;2>H$9$k$H$$$&Nc30E*$J=hM}$K$J$C$F$$$k!%(B
 */
extern int	logtask_alive;		/* $B%m%0%?%9%/$,F0$$$F$$$k$+(B */
extern int	log_msg_maxmsz;		/* $B%m%0%a%C%;!<%8$N:GBgD9(B */

/*
 *  $B%7%9%F%`%m%0MQ%a%C%;!<%8%P%C%U%!$X$NAw?.(B (messagebuf.c)
 */
extern ER	log_snd_mbf(VP msg, INT msgsz);

/*
 *  $B%+!<%M%kMQ4J0W(B vsprintf$B4X?t(B (vsprintf.c)
 */
extern int	itis_vsprintf(char *buf, const char *format, va_list ap);

/*
 *  $B%+!<%M%kMQ%7%9%F%`%m%0=PNOMQ%i%$%V%i%j(B
 */

static int	i_logmask = LOG_UPTO(LOG_NOTICE);

#define	FORMAT_BUFSIZ	1024

static char	i_syslog_buf[FORMAT_BUFSIZ];

/*
 *  $B%m%0%a%C%;!<%8$N=PNO(B
 *
 *  $B%m%0%?%9%/$,F0$$$F$$$k>l9g$O!$%m%0%a%C%;!<%8%P%C%U%!$XAw$k!%F0$$$F(B
 *  $B$$$J$$>l9g$O!$D>@\Dc%l%Y%k$N=PNO%k!<%A%s$r;H$C$F=PNO$9$k!%(B
 */
static void
i_syslog_send(const char *string, int len)
{
	if (len > 0) {
#ifdef USE_TMBF_OS
		if (logtask_alive) {
			if (len > log_msg_maxmsz) {
				len = log_msg_maxmsz;
			}
			if (log_snd_mbf((VP) string, len) != E_OK) {
				logtask_alive = 0;
			}
		}
		if (!logtask_alive) {
			sys_write(string, len);
			sys_write("\n", 1);
		}
#else /* USE_TMBF_OS */
		sys_write(string, len);
		sys_write("\n", 1);
#endif /* USE_TMBF_OS */
	}
}

/*
 *  $B%+!<%M%kMQ(B syslog $B4X?tK\BN(B
 */
void
i_syslog(int priority, const char *format, ...)
{
	va_list	ap;
	int	len;

	if (i_logmask & LOG_MASK(priority & LOG_PRIMASK)) {
		va_start(ap, format);
		BEGIN_CRITICAL_SECTION;
		if ((priority & LOG_PRIMASK) == LOG_EMERG) {
			logtask_alive = 0;
		}
		len = itis_vsprintf(i_syslog_buf, format, ap);
		i_syslog_send(i_syslog_buf, len);
		END_CRITICAL_SECTION;
		va_end(ap);
	}
}

/*
 *  $B%+!<%M%kMQ$N(B assert$B%^%/%m$N%a%C%;!<%8=PNO(B
 */

#ifndef NDEBUG

void
i_assert_fail(const char *expr, const char *file, unsigned int line)
{
	i_syslog(LOG_KERN|LOG_EMERG, "%s:%u: Assertion `%s' failed.",
		file, line, expr);
	itis_exit();
}

#endif /* NDEBUG */
