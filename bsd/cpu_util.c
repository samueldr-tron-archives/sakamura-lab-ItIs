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
 *  @(#) $Id: cpu_util.c,v 1.6 1996/02/17 09:23:40 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"

/*
 *  タスクディスパッチャ
 * 
 *  実行中のタスク (ctxtsk) のコンテキストを TCB に保存し，実行すべき
 *  タスク (schedtsk) を新しい実行タスクとして，そのコンテキストを TCB 
 *  から復帰する．タスクディスパッチャから戻る際に，シグナルマスクは以
 *  前の状態に戻される．
 */
void
dispatch_handler()
{
	if (!ctxtsk || (ctxtsk != schedtsk
				&& _setjmp(ctxtsk->tskctxb.env) == 0)) {
		sigsetmask(SIGMASK_LOC);
		while (!(ctxtsk = schedtsk)) {
			sigpause(SIGMASK_DDSP);
			sigsetmask(SIGMASK_LOC);
		}
		_longjmp(ctxtsk->tskctxb.env, 1);
	}
}

/*
 *  ターゲットCPU依存の初期化ルーチン
 */
void
cpu_initialize(void)
{
	/*
	 *  シグナルスタックを，プロセススタック上に取る．シグナルスタッ
	 *  クを使っていると設定するのは，スタートアップルーチンをタス
	 *  ク独立部と判定させるため．
	 */
	{
#ifdef USE_SIGSTACK
	       	struct sigstack	ss;

		ss.ss_sp = (void *)(((INT) &ss) - SIGSTACK_MERGIN);
		ss.ss_onstack = 1;
		sigstack(&ss, 0);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
		struct sigaltstack	ss;

		ss.ss_sp = (void *)(((INT) &ss) - SIGSTACK_MERGIN - SIGSTKSZ);
		ss.ss_size = SIGSTKSZ;
		ss.ss_flags |= SA_ONSTACK;
		sigaltstack(&ss, 0);
#endif /* USE_SIGALTSTACK */
	}

	/*
	 *  ディスパッチ用のシグナルハンドラを設定．
	 */
	{
		struct sigvec	vec;

		vec.sv_handler = dispatch_handler;
		vec.sv_mask = 0;
		vec.sv_flags = SV_INTERRUPT;
		sigvec(SIGUSR2, &vec, 0);
	}
}

/*
 *  ターゲットCPU依存の終了処理ルーチン
 */
void
cpu_shutdown(void)
{
}

/*
 *  タスク起動ルーチン
 *
 *  BSD UNIX上では，カーネルとタスクが同じ特権レベルで動作するため，タ
 *  スクへの分岐は関数呼び出しで実現できる．
 */
void
task_start()
{
	/*
	 *  シグナルマスクを設定して，タスクを起動する．
	 */
	sigsetmask(SIGMASK_TASK);
	(*ctxtsk->task)(ctxtsk->tskctxb.stacd, ctxtsk->exinf);

	/*
	 *  タスクの終了時に ext_tsk を呼ぶので，ここへは戻らない．
	 */
	assert(0);
}

/*
 *  システムメモリプールを使わない場合
 *
 *  UNIXライブラリの malloc と free にシステムメモリプールの役割を任せ
 *  る．
 */

#ifndef USE_TMPL_OS

#include <malloc.h>

ER
sys_cre_mpl(void)
{
	return(E_OK);
}

VP
sys_get_blk(INT blksz)
{
	return(malloc(blksz));
}

void
sys_rel_blk(VP blk)
{
	free(blk);
}

#endif /* USE_TMPL_OS */

/*
 *  拡張SVC の出入口の処理
 *
 *  拡張SVC 内でシステムオブジェクトをアクセスできるようにするための処理．
 */

void
enter_extended_svc()
{
	ctxtsk->sysmode++;
}

void
exit_extended_svc()
{
	ctxtsk->sysmode--;
}
