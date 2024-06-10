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
 *  @(#) $Id: inittask.c,v 1.13 1998/07/06 14:21:01 hiro Exp $
 */

/*
 *  $B=i4|2=%?%9%/(B
 */

#include "systask.h"

/*
 *  $BI,MW$J%7%9%F%`%?%9%/$*$h$S=i4|5/F0%?%9%/$r5/F0$9$k!%6qBNE*$K$O!$AH(B
 *  $B$_9~$`$Y$-%7%9%F%`%?%9%/$*$h$S%I%i%$%P$H!$=i4|5/F0%?%9%/$r5/F0$9$k!%(B
 *  $B0J>e$N=hM}$r=*$($k$H!$(Bslp_tsk $B$K$h$jBT$A>uBV$KF~$k!%%7%9%F%`A4BN$r(B
 *  $B0BA4$KDd;_$5$;$k>l9g$K$O!$=i4|2=%?%9%/$r(B wup_tsk $B$G5/>2$5$;$k!%(B
 */
void
init_task(INT stacd)
{
	syslog(LOG_NOTICE, "init: I'm now starting up some tasks.....");

	/*
	 *  BSD UNIX$BMQ(B $B%N%s%V%m%C%-%s%0(BI/O $B%5%]!<%H%b%8%e!<%k$N5/F0(B
	 *
	 *  $B%7%j%"%k%$%s%?%U%'!<%9$h$j@h$K5/F0$9$k$Y$-!%(B
	 */
#if defined(TSK_BSD_SIGIO) && defined(SEM_BSD_SIGIO)
	bsd_sigio_startup();
#endif

	/*
	 *  $B%7%j%"%k%$%s%?%U%'!<%9%I%i%$%P$N5/F0(B
	 */
#ifdef CONSOLE_PORT
	serial_startup(CONSOLE_PORT);
#endif

	/*
	 *  $B%7%9%F%`%m%0%?%9%/$N5/F0(B
	 */
#ifdef LOGTASK_PORT
	logtask_startup(LOGTASK_PORT);
#endif

	/*
	 *  $B=i4|5/F0%?%9%/$N5/F0(B
	 */
#ifdef TSK_FIRST
	syscall(cre_tsk(TSK_FIRST, &TSK_FIRST_CTSK));
	syscall(sta_tsk(TSK_FIRST, stacd));
#endif

	/*
	 *  $B=i4|2==hM}40N;(B
	 */
	syslog(LOG_NOTICE, "init: initialize OK.");
	syscall(slp_tsk());

	/*
	 *  $B%7%9%F%`Dd;_=hM}(B
	 */
#ifdef CONSOLE_PORT
	serial_shutdown(CONSOLE_PORT, 1);
#endif
	itis_exit();
}
