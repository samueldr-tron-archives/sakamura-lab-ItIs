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
 *  @(#) $Id: cpu_status.h,v 1.8 1998/06/08 10:52:07 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  BSD UNIX 依存定義
 */

/* 
 *  BSD UNIX 標準インクルードファイル
 */
#include <signal.h>
#include <setjmp.h>

/*
 *  シグナルマスクの定義
 *
 *  タスクディスパッチャ起動用に SIGUSR2 を使う．
 *
 *  通常のタスク部実行中はシグナルマスクは 0 (すべてのシグナルマスクが 
 *  解除された状態) になっている．ディスパッチ禁止状態では SIGUSR2 を 
 *  マスクし，割込みおよびディスパッチ禁止状態ではすべてのシグナルをマ
 *  スクする．タスク独立部実行中は，少なくとも SIGUSR2 をマスクしなけ
 *  ればならない．
 */
#define SIGMASK_TASK	(0)
#define SIGMASK_DDSP	(sigmask(SIGUSR2))
#define SIGMASK_LOC	(~sigmask(SIGTRAP))
#define SIGMASK_INDP	(sigmask(SIGUSR2))

/*
 *  現在使用中のスタックを調べる
 *
 *  タスク独立部実行中は，シグナルスタックを使うことにして，シグナルス
 *  タックを使っているかどうかで，タスク独立部を判別する．
 */
Inline int
current_stack()
{
#ifdef USE_SIGSTACK
	struct sigstack	ss;

	sigstack(0, &ss);
	return(ss.ss_onstack);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
	struct sigaltstack	ss;

	sigaltstack(0, &ss);
	return(ss.ss_flags & SS_ONSTACK);
#endif /* USE_SIGALTSTACK */
}

/*
 *  クリティカルセクション定義用マクロ
 */

#define BEGIN_CRITICAL_SECTION	{ int mask = sigblock(SIGMASK_LOC);
#define END_CRITICAL_SECTION	sigsetmask(mask); }

#define DISABLE_INTERRUPT	{ sigblock(SIGMASK_LOC); }

/*
 *  システム状態判別用マクロ
 */

/*
 *  in_indp(): システムコールが，タスク独立部から呼ばれたかを判別するた
 *  めのマクロ．
 *
 *  シグナルスタックを使っているかどうかで判断する．
 */
#define in_indp()	(current_stack())

/*
 *  in_ddsp(): システムコールがディスパッチ禁止中に呼ばれたかを判別する
 *  ためのマクロ．タスク独立部も，ディスパッチ禁止中に含む．
 *
 *  SIGUSR2 がマスクされているかどうかで判断する．
 */
#define in_ddsp()	(sigblock(0) & SIGMASK_DDSP)

/*
 *  in_loc(): システムコールが CPUロック中に呼ばれたかを判別するための
 *  マクロ．タスク独立部も，CPUロック中に含む．
 *
 *  SIGUSR2 以外のいずれかのシグナルがマスクされている場合に，CPUロッ
 *  ク中であると判断する．タスク独立部では，SIGUSR2 以外のいずれかのシ
 *  グナルがマスクされているはず．
 */
#define in_loc()	(sigblock(0) & ~SIGMASK_DDSP)

/*
 *  in_sysmode(): システムコールがシステムリソースをアクセスできる状態か
 *  ら呼ばれたかを判別するためのマクロ．E_OACVエラーのチェックに用いる．
 *
 *  ctxtsk の sysmodeフィールドを見て判別する必要がある．
 */
#define	in_sysmode()	(in_indp() || ctxtsk->sysmode)

/*
 *  in_qtsk(): システムコールが，準タスク部を実行中から呼ばれたかを判別
 *  するためのマクロ．タスク独立部とは区別しないので，in_indp() が FALSE 
 *  の時にのみ使うこと．
 *
 *  この判別を可能にするために，TCB 中に isysmodeフィールドを用意する．
 *  USE_QTSK_PORTION が定義されていない場合には，準タスク部は使われない
 *  ので，常に FALSE とする．
 */
#ifdef USE_QTSK_PORTION
#define in_qtsk()	(ctxtsk->sysmode > ctxtsk->isysmode)
#else /* USE_QTSK_PORTION */
#define in_qtsk()	0
#endif /* USE_QTSK_PORTION */

/*
 *  タスクディスパッチャ起動ルーチン
 */

/*
 *  タスクディスパッチャ
 */
extern void dispatch_handler();

/*
 *  タスクディスパッチャの起動要求を出す．
 */
Inline void
dispatch_request(void)
{
	kill(getpid(), SIGUSR2);
}

/*
 *  現在のタスクのコンテキストを捨てて，次に実行すべきタスクへ強制的に
 *  ディスパッチする．
 *
 *  システム起動時および ext_tsk, ext_tsk の処理で用いる．ctxtsk が前
 *  方参照になるため，force_dispatch 本体は，マクロとして定義する．
 */
Inline void
_force_dispatch(void)
{
#ifdef USE_SIGSTACK
	struct sigstack	ss;

	sigstack(0, &ss);
	ss.ss_onstack = 0;
	sigstack(&ss, 0);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
	struct sigaltstack	ss;

	sigaltstack(0, &ss);
	ss.ss_flags &= ~SS_ONSTACK;
	sigaltstack(&ss, 0);
#endif /* USE_SIGALTSTACK */
	dispatch_handler();
}

#define	force_dispatch() \
{					\
	ctxtsk = 0;			\
	_force_dispatch();		\
}

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct {
	INT	stacd;		/* タスク起動コード */
	jmp_buf	env;		/* CPUコンテキスト */
} CTXB;

#endif /* _CPU_STATUS_ */
