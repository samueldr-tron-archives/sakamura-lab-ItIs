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
 *  @(#) $Id: logtask.c,v 1.10 1998/01/30 09:58:16 hiro Exp $
 */

/*
 *  $B%7%9%F%`%m%0%?%9%/(B
 */

#include <stdarg.h>
#include "systask.h"

/*
 *  $B%m%0%?%9%/$NJQ?t(B
 */
int	logtask_alive = 0;	/* $B%m%0%?%9%/$,F0$$$F$$$k$+(B */
int	log_msg_maxmsz;		/* $B%m%0%a%C%;!<%8$N:GBgD9(B */

/*
 *  $BA0J};2>H@k8@(B
 */
ER	svc_syslog_send(const char *string, int len);

/*
 *  $B%m%0%?%9%/$N5/F0$H=i4|2=!$3HD%(BSVC$B$NDj5A(B
 */
void
logtask_startup(int portid)
{
	syscall(serial_init(portid));
	syscall(cre_mbf(TMBF_OS, &MBF_LOG_CMBF));
	syscall(cre_tsk(TSK_LOG, &TSK_LOG_CTSK));
	syscall(sta_tsk(TSK_LOG, portid));

#ifdef USE_EXTENDED_SVC
	syscall(def_svc(SVC_SYSLOG_SEND, DSVC(svc_syslog_send)));
#endif

	log_msg_maxmsz = MBF_LOG_MAXMSZ;
	logtask_alive = 1;

	syslog(LOG_NOTICE, "System logging task started on port %d.", portid);
}

/*
 *  $B%m%0%?%9%/$NK\BN(B
 */

static int	logtask_portid;			/* $B%m%0=PNO%]!<%HHV9f(B */
static char	logtask_buf[MBF_LOG_MAXMSZ+1];	/* $B%m%0%?%9%/MQ%P%C%U%!(B */

void
log_task(int portid)
{
	INT	msgsz;
	ER	ercd;

	logtask_portid = portid;
	while ((ercd = rcv_mbf(logtask_buf, &msgsz, TMBF_OS)) == E_OK) {
		logtask_buf[msgsz++] = '\n';
		serial_write(logtask_portid, logtask_buf, msgsz);
	}
	logtask_alive = 0;
	perror("logtask", ercd);
	ext_tsk();
}

/*
 *  $B3HD%(BSVC$B%O%s%I%iK\BN(B
 *
 *  $B%m%0%?%9%/$,F0$$$F$$$k>l9g$O!$%m%0%a%C%;!<%8%P%C%U%!$XAw$k!%F0$$$F(B
 *  $B$$$J$$>l9g$O!$D>@\Dc%l%Y%k$NJ8;z=PNO%k!<%A%s$r;H$C$F=PNO$9$k!%(B
 */

ER
svc_syslog_send(const char *string, int len)
{
	ENTER_EXTENDED_SVC;
	if (len > 0) {
		if (logtask_alive) {
			if (len > log_msg_maxmsz) {
				len = log_msg_maxmsz;
			}
			if (psnd_mbf(TMBF_OS, (VP) string, len) != E_OK) {
				logtask_alive = 0;
			}
		}
		if (!logtask_alive) {
			sys_write(string, len);
			sys_write("\n", 1);
		}
	}
	EXIT_EXTENDED_SVC;
	return(E_OK);
}
