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
 *  @(#) $Id: sys_timer.h,v 1.2 1998/06/18 07:44:09 hiro Exp $
 */

#ifndef _SYS_TIMER_
#define _SYS_TIMER_

/*
 *  ハードウェア依存タイマモジュール
 *
 *  このモジュール内で，以下の定数および関数を定義しなければならない．
 * 
 *  TIMER_PERIOD : タイマ割込みの周期 (単位は msec)．ItIs での標準値は 
 *  1msec であるが，タイマ割込みによるオーバヘッドを小さくしたい場合に
 *  は，もっと長い周期に設定してもよい．もちろん，タイムアウト時間など
 *  の分解度は粗くなる (単位は変わらない)．
 *
 *  void start_hw_timer() : タイマを初期化し，周期的なタイマ割込みを開
 *  始させる．
 *
 *  void clear_hw_timer_interupt(void) : タイマ割込み要求をクリアする．
 *  タイマ割込みハンドラの最初で呼ばれる．
 *
 *  void terminate_hw_timer(void) : タイマの動作を停止させる．システム
 *  停止時に呼ばれる．
 */

#include "dve68k.h"

/*
 *  タイマ割込みの周期 (単位は msec)
 */
#define TIMER_PERIOD	1

/*
 *  タイマ値の内部表現の型
 */
typedef unsigned int	TICK;

/*
 *  タイマ値の内部表現と msec 単位との変換
 *
 *  DVE68K/40 CPUボードでは，タイマは 1μsec 毎にカウントアップする．
 */
#define TIMER_TICK	1000
#define TO_TICK(msec)	((msec) * TIMER_TICK)

/*
 *  タイマ値の内部表現とμsec 単位との変換
 */
#if TIMER_TICK <= 1000
#define TO_USEC(tick)	((tick) * (1000 / TIMER_TICK))
#else
#define TO_USEC(tick)	((tick) / (TIMER_TICK / 1000))
#endif

/*
 *  性能評価用システム時刻を取り出す際の処理時間の見積り値 (単位は内部表現)
 */
#define GET_TOLERANCE	(TIMER_TICK / 5)

/*
 *  設定できる最大のタイマ周期 (単位は内部表現)
 */
#define MAX_TICK	((TICK) 0xffffff)

/*
 *  アセンブラレベルのタイマハンドラ
 */
extern void	timer_handler_startup(void);

/*
 *  タイマのスタート処理
 */
Inline void
start_hw_timer()
{
	TICK	t = TO_TICK(TIMER_PERIOD);

	/*
	 *  割込みハンドラの定義
	 */
	define_exc(TT0_VEC, timer_handler_startup);

	/*
	 *  タイマ関連の設定
	 */
	assert(t <= MAX_TICK);		/* タイマ上限値のチェック */
	*DGA_CSR12 = 0x80000000 | t;	/* 上限値を設定，タイマスタート */

	/*
	 *  割込み関連の設定
	 */
	dga_set_ilv(DGA_CSR25, TT0IL_BIT, IRQ_LEVEL4);
					/* 割込みレベル設定 */
	*DGA_CSR23 = TT0_BIT;		/* 割込み要求をクリア */
	*DGA_CSR21 |= TT0_BIT;		/* 割込みマスク解除 */
}

/*
 *  タイマ割込みのクリア
 */
Inline void
clear_hw_timer_interrupt(void)
{
	*DGA_CSR23 = TT0_BIT;		/* 割込み要求をクリア */
}

/*
 *  タイマの停止処理
 */
Inline void
terminate_hw_timer(void)
{
	*DGA_CSR12 &= 0x7fffffff;		/* タイマを停止 */
	*DGA_CSR21 &= ~TT0_BIT;			/* 割込みマスク設定 */
	*DGA_CSR23 = TT0_BIT;			/* 割込み要求をクリア */
}

/*
 *  タイマの現在値の読みだし
 *
 *  割込み禁止区間中で呼び出すこと．
 */
Inline TICK
get_current_hw_time(void)
{
	return(*DGA_CSR13 & 0x00ffffff);
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(*DGA_CSR20 & TT0_BIT);
}

#endif /* _SYS_TIMER_ */
