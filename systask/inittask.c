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
 *  @(#) $Id: inittask.c,v 1.11 1998/01/30 07:14:05 hiro Exp $
 */

/*
 *  初期化タスク
 */

#include "systask.h"

/*
 *  必要なシステムタスクおよび初期起動タスクを起動する．具体的には，組
 *  み込むべきシステムタスクおよびドライバと，初期起動タスクを起動する．
 *  以上の処理を終えると，slp_tsk により待ち状態に入る．システム全体を
 *  安全に停止させる場合には，初期化タスクを wup_tsk で起床させる．
 */
void
init_task(INT stacd)
{
	syslog(LOG_NOTICE, "init: I'm now starting up some tasks.....");

	/*
	 *  BSD UNIX用 ノンブロッキングI/O サポートモジュールの起動
	 *
	 *  シリアルインタフェースより先に起動するべき．
	 */
#ifdef FLG_BSD_SIGIO
	bsd_sigio_startup();
#endif

	/*
	 *  シリアルインタフェースドライバの起動
	 */
#ifdef CONSOLE_PORT
	serial_startup(CONSOLE_PORT);
#endif

	/*
	 *  システムログタスクの起動
	 */
#ifdef LOGTASK_PORT
	logtask_startup(LOGTASK_PORT);
#endif

	/*
	 *  初期起動タスクの起動
	 */
#ifdef TSK_FIRST
	syscall(cre_tsk(TSK_FIRST, &TSK_FIRST_CTSK));
	syscall(sta_tsk(TSK_FIRST, stacd));
#endif

	/*
	 *  初期化処理完了
	 */
	syslog(LOG_NOTICE, "init: initialize OK.");
	syscall(slp_tsk());

	/*
	 *  システム停止処理
	 */
#ifdef CONSOLE_PORT
	serial_shutdown(CONSOLE_PORT, 1);
#endif
	itis_exit();
}
