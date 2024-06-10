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
 *  @(#) $Id: cpu_status.h,v 1.5 1998/01/30 06:48:12 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  TRON仕様チップ依存定義
 */

/*
 *  TRON仕様チップ 低レベル定義
 */
#include "tron_defs.h"
#include "tron_insn.h"

/*
 *  クリティカルセクション定義用マクロ
 *
 *  遅延割込みを使わない場合には，クリティカルセクション終了時に，ディ
 *  スパッチが必要かどうかをチェックし，必要ならディスパッチャを起動す
 *  る必要がある．
 */

#ifndef TRON_NO_DI

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask); }

#else /* TRON_NO_DI */

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask);			\
				if (imask_psw(intmask) == 15		\
					    && !dispatch_disabled	\
					    && ctxtsk != schedtsk) {	\
					dispatch();			\
				} }
#endif /* TRON_NO_DI */

#define DISABLE_INTERRUPT	{ disint(); }

/*
 *  システム状態判別用マクロ
 */

/*
 *  in_indp(): システムコールが，タスク独立部から呼ばれたかを判別するた
 *  めのマクロ．
 *
 *  システムコール呼出しの TRAPA では，PSW中のスタックモードは変化しな
 *  いので，in_indp が呼ばれた時点のスタックモードが 0 ならタスク独立部
 *  から呼ばれたと判別できる．
 */
#define in_indp()	((current_psw() & 0x80000000) == 0)

/*
 *  in_ddsp(): システムコールがディスパッチ禁止中に呼ばれたかを判別する
 *  ためのマクロ．タスク独立部も，ディスパッチ禁止中に含む．
 *
 *  システムコール呼び出しの TRAPA では IMASK は変化しないので，in_ddsp 
 *  が呼ばれた時点の IMASK が 15 未満なら，ディスパッチ禁止中に呼ばれた
 *  と判別できる．タスク独立部で IMASK が 15 になるケースは考えない．
 *
 *  遅延割込みを使わない場合には，IMASK = 14 に相当する状態が，
 *  dispatch_disabled を TRUE にすることで記憶されるため，
 *  dispatch_disabled のチェックも必要である．
 */
#ifndef TRON_NO_DI
#define in_ddsp()	(current_imask() < 15)
#else /* TRON_NO_DI */
#define in_ddsp()	(current_imask() < 15 || dispatch_disabled)
#endif /* TRON_NO_DI */

/*
 *  in_loc(): システムコールが CPUロック中に呼ばれたかを判別するための
 *  マクロ．タスク独立部も，CPUロック中に含む．
 *
 *  システムコール呼び出しの TRAPA では IMASK は変化しないので，in_loc 
 *  が呼ばれた時点の IMASK が 14 未満なら，CPUロック中に呼ばれたと考え
 *  る．実際には，loc_cpu システムコールを使うと，IMASK は 0 になる．タ
 *  スク独立部で IMASK が 14 ないしは 15 になるケースは考えない．
 *
 *  遅延割込みを使わない場合には，IMASK が 14 になることはなく，15 未満
 *  かどうかでチェックする．
 */
#ifndef TRON_NO_DI
#define in_loc()	(current_imask() < 14)
#else /* TRON_NO_DI */
#define in_loc()	(current_imask() < 15)
#endif /* TRON_NO_DI */

/*
 *  in_sysmode(): システムコールがシステムリソースをアクセスできる状態か
 *  ら呼ばれたかを判別するためのマクロ．E_OACVエラーのチェックに用いる．
 *
 *  PSW中のリングレベルは，システムコール呼び出しの TRAPA により変化する
 *  ため，in_sysmode が呼ばれた時点の RNG を見て判別することはできず，
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
#ifndef TRON_NO_DI
	set_dir(14);
#endif /* TRON_NO_DI */
}

/*
 *  現在のタスクのコンテキストを捨てて，次に実行すべきタスクへ強制的に
 *  ディスパッチする．
 *
 *  システム起動時および ext_tsk, exd_tsk の処理で用いる．
 *
 *  処理内容としては，dispatch_to_schedtsk へ分岐するだけである．分岐後
 *  はタスクのコンテキストは捨てられるので関数呼出しでも基本的には問題
 *  ないが，SPI を使っているモードで呼んだ場合 (正常に使っていれば，シ
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
 *
 *  TRON仕様チップでは，遅延割込みを使わない場合にのみ使われる．
 */
#ifdef TRON_NO_DI
Inline void
dispatch()
{
	Asm("trapa #4" : : : "memory");
}
#endif /* TRON_NO_DI */

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct {
	VW	csw;		/* CSW (Context Status Word) */
	VP	sp0;		/* SP0 */
	VP	sp1;		/* SP1 */
	VP	sp2;		/* SP2 */
	VP	sp3;		/* SP3 */
	VW	uatb;		/* UATB */
	VW	lsid;		/* LSID */
} CTXB;

#endif /* _CPU_STATUS_ */
