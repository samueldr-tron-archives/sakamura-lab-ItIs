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
 *  @(#) $Id: startup.c,v 1.12 1997/01/10 13:36:23 hiro Exp $
 */

#include "itis_kernel.h"
#include "timer.h"
#include "task.h"
#include "patchlevel.h"
#include "../systask/systask.h"

/*
 *  バージョン情報 (version.c)
 */
extern char	version[];

/*
 *  ItIsカーネルの初期化と初期化タスクの生成・起動
 */
void
main()
{
	ER	ercd;

	DISABLE_INTERRUPT;
	i_syslog(LOG_KERN|LOG_ALERT,
		version, MAJOR_REL, MINOR_REL, PATCH_LEVEL);

	/*
	 *  ターゲット依存の初期化
	 */
	cpu_initialize();
	sys_initialize();

	/*
	 *  各モジュールの初期化
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
	 *  システムメモリプールの生成
	 */
	if ((ercd = sys_cre_mpl()) < 0) {
		i_syslog(LOG_KERN|LOG_EMERG,
			"Cannot alloc system memory pool: %d", ercd);
		itis_exit();
	}

	/*
	 *  初期化タスクの生成・起動
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
	 *  カーネルの動作を開始する．
	 */
	force_dispatch();
}

/*
 *  ItIs 終了処理
 */
void
itis_exit()
{
	timer_shutdown();
	cpu_shutdown();
	sys_exit();
}
