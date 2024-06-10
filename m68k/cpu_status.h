/**
 * 
 * 	ItIs - An ITRON Implementation for Research and Education
 * 
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
 * 3. Neither the name of the university nor the name of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: cpu_status.h,v 1.2 1998/06/18 07:45:20 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  m68k 依存定義
 */

/*
 *  m68k 低レベル定義
 */
#include "m68k_insn.h"

/*
 *  クリティカルセクション定義用マクロ
 *
 *  クリティカルセクション終了時に，ディスパッチが必要かどうかをチェッ
 *  クし，必要ならディスパッチャを起動する．
 */

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask);			\
				if (!intmask_lock(intmask)		\
					    && !dispatch_disabled	\
					    && ctxtsk != schedtsk) {	\
					dispatch();			\
				} }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 *  システム状態判別用マクロ
 */

/*
 *  in_indp(): システムコールが，タスク独立部から呼ばれたかを判別するた
 *  めのマクロ．
 *
 *  システムコール呼出しの TRAP では，SR中の Mビットは変化しないので，
 *  in_indp が呼ばれた時点の Mビットが 0 ならタスク独立部から呼ばれたと
 *  判別できる．
 */
#define in_indp()	((current_sr() & 0x1000) == 0)

/*
 *  in_loc(): システムコールが CPUロック中に呼ばれたかを判別するための
 *  マクロ．タスク独立部も，CPUロック中に含む．
 *
 *  システムコール呼び出しの TRAP では IPM は変化しないので，in_loc が
 *  呼ばれた時点の IPM が 0 より大きければ，CPUロック中に呼ばれたと考
 *  える．実際には，loc_cpu システムコールを使うと，IPM は 7 になる．
 *  タスク独立部で IPM が 0 になるケースは考えない．
 */
#define in_loc()	(current_intmask() > 0)

/*
 *  in_ddsp(): システムコールがディスパッチ禁止中に呼ばれたかを判別する
 *  ためのマクロ．CPUロック中も，ディスパッチ禁止中に含む．
 *
 *  CPUロック中は，in_loc でチェックする．ディスパッチのみ禁止された状
 *  態は，dispatch_disabled でチェックする．
 */
#define in_ddsp()	(in_loc() || dispatch_disabled)

/*
 *  in_sysmode(): システムコールがシステムリソースをアクセスできる状態か
 *  ら呼ばれたかを判別するためのマクロ．E_OACVエラーのチェックに用いる．
 *
 *  SR中の動作モードは，システムコール呼び出しの TRAP により変化するた
 *  め，in_sysmode が呼ばれた時点の Sビットを見て判別することはできず，
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
 *  タスクディスパッチャの起動要求を出す．
 */
Inline void
dispatch_request(void)
{
}

/*
 *  現在のタスクのコンテキストを捨てて，次に実行すべきタスクへ強制的に
 *  ディスパッチする．
 *
 *  システム起動時および ext_tsk, exd_tsk の処理で用いる．
 *
 *  処理内容としては，dispatch_to_schedtsk へ分岐するだけである．分岐後
 *  はタスクのコンテキストは捨てられるので関数呼出しでも基本的には問題
 *  ないが，ISP を使っているモードで呼んだ場合 (正常に使っていれば，シ
 *  ステム起動時のみ) に，bra 命令で分岐した方が関数呼出しを行うよりも
 *  返り番地のスタック領域分が (わずか 4バイトだけではあるが) 節約でき
 *  る．
 */
Inline void
force_dispatch(void)
{
	Asm("bra _dispatch_to_schedtsk");
}

/*
 *  タスクディスパッチャを起動する．
 */
Inline void
dispatch()
{
	Asm("trap #7" : : : "memory");
}

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct {
	VP	msp;		/* MSP (Master Stack Pointer) */
	VP	usp;		/* USP (User Stack Pointer) */
} CTXB;

#endif /* _CPU_STATUS_ */
