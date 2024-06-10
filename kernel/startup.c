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
 *  @(#) $Id: startup.c,v 1.14 1998/07/06 11:07:49 hiro Exp $
 */

#include "itis_kernel.h"
#include "timer.h"
#include "task.h"
#include "patchlevel.h"
#include "../systask/systask.h"

/*
 *  $B%P!<%8%g%s>pJs(B (version.c)
 */
extern char	version[];

/*
 *  ItIs$B%+!<%M%k$N=i4|2=$H=i4|2=%?%9%/$N@8@.!&5/F0(B
 */
main()
{
	ER	ercd;

	DISABLE_INTERRUPT;
	i_syslog(LOG_KERN|LOG_ALERT,
		version, MAJOR_REL, MINOR_REL, PATCH_LEVEL);

	/*
	 *  $B%?!<%2%C%H0MB8$N=i4|2=(B
	 */
	cpu_initialize();
	sys_initialize();

	/*
	 *  $B3F%b%8%e!<%k$N=i4|2=(B
	 */
	task_initialize();
#ifdef USE_SEM
	semaphore_initialize();
#endif /* USE_SEM */
#ifdef USE_FLG
	eventflag_initialize();
#endif /* USE_FLG */
#ifdef USE_MBX
	mailbox_initialize();
#endif /* USE_MBX */
#ifdef USE_MBF
	messagebuffer_initialize();
#endif /* USE_MBF */
#ifdef USE_POR
	rendezvous_initialize();
#endif /* USE_POR */
#ifdef USE_PIS
	prisem_initialize();
#endif /* USE_PIS */
#ifdef USE_MPL
	memorypool_initialize();
#endif /* USE_MPL */
#ifdef USE_MPF
	fix_memorypool_initialize();
#endif /* USE_MPF */
#ifdef USE_CYC
	cyclichandler_initialize();
#endif /* USE_CYC */
#ifdef USE_ALM
	alarmhandler_initialize();
#endif /* USE_ALM */
#ifdef USE_SVC
	extended_svc_initialize();
#endif /* USE_SVC */
	timer_initialize();

	/*
	 *  $B%7%9%F%`%a%b%j%W!<%k$N@8@.(B
	 */
	if ((ercd = sys_cre_mpl()) < 0) {
		i_syslog(LOG_KERN|LOG_EMERG,
			"Cannot alloc system memory pool: %d", ercd);
		itis_exit();
	}

	/*
	 *  $B=i4|2=%?%9%/$N@8@.!&5/F0(B
	 */
	if ((ercd = i_cre_tsk(TSK_INIT, &TSK_INIT_CTSK)) < 0) {
		i_syslog(LOG_KERN|LOG_EMERG,
			"Failed to create initial task (%d).", ercd);
		itis_exit();
	}
	if ((ercd = i_sta_tsk(TSK_INIT, 0)) < 0) {
		i_syslog(LOG_KERN|LOG_EMERG,
			"Failed to startup initial task (%d).", ercd);
		itis_exit();
	}

	/*
	 *  $B%+!<%M%k$NF0:n$r3+;O$9$k!%(B
	 */
	force_dispatch();
}

/*
 *  ItIs $B=*N;=hM}(B
 */
void
itis_exit()
{
	timer_shutdown();
	cpu_shutdown();
	sys_exit();
}
