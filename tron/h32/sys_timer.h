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
 *  @(#) $Id: sys_timer.h,v 1.4 1996/02/17 09:43:45 hiro Exp $
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

#include "h32sbc.h"

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
 *  H32SBCシステム CPUボードでは，タイマは 4μsec 毎にカウントアップす
 *  る．
 */
#define TIMER_TICK	250
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
 *
 *  内部表現で 0xffffff は約 67秒．
 */
#define MAX_TICK	((TICK) 0xffffff)

/*
 *  PIT タイマカウンタレジスタの設定値
 */
#define PIT_CYCLIC	0xe0

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
	TICK	t = TO_TICK(TIMER_PERIOD) - 1;

	/*
	 *  割込みハンドラの定義
	 */
	define_eit(INT_VECTOR(2), EITATR(0, 15), timer_handler_startup);

	/*
	 *  IRC の設定
	 */
	irc_assign(IRC_LMR(2), LIR6_BIT);
	irc_or_assign(IRC_VMR, LIR6_BIT);
	irc_or_assign(IRC_TMR, LIR6_BIT);	/* エッジトリガモード */
	irc_and_assign(IRC_IMR, ~LIR6_BIT);	/* 割込みマスク解除 */

	/*
	 *  PIT の設定
	 */
	io_assign(PIT_TCR, PIT_CYCLIC);
	assert(t <= MAX_TICK);			/* タイマ上限値のチェック */
	io_assign(PIT_CPRH, (t >> 16) & 0xff);
	io_assign(PIT_CPRM, (t >> 8) & 0xff);
	io_assign(PIT_CPRL, t & 0xff);
	io_assign(PIT_TSR, 0x01);		/* ゼロ検出をクリア */
	io_or_assign(PIT_TCR, 0x01);		/* タイマーをスタートさせる */

	irc_assign(IRC_IRR, LIR6_BIT);		/* 割込み要求をクリア */
}

/*
 *  タイマ割込みのクリア
 */
Inline void
clear_hw_timer_interrupt(void)
{
	irc_assign(IRC_IRR, LIR6_BIT);		/* 割込み要求をクリア */
	io_assign(PIT_TSR, 0x01);		/* ゼロ検出をクリア */
}

/*
 *  タイマの停止処理
 */
Inline void
terminate_hw_timer(void)
{
	io_and_assign(PIT_TCR, ~0x01);		/* タイマーを止める */
	io_assign(PIT_TSR, 0x01);		/* ゼロ検出をクリア */
	irc_or_assign(IRC_IMR, LIR6_BIT);	/* 割込みをマスク */
}

/*
 *  タイマの現在値の読みだし
 *
 *  割込み禁止区間中で呼び出すこと．
 */
Inline TICK
get_current_hw_time(void)
{
#if TO_TICK(TIMER_PERIOD) < 256
	return(TO_TICK(TIMER_PERIOD) - 1 - *PIT_CNTRL);
#else
#error \
  get_current_hw_time is not implemented when TO_TICK(TIMER_PERIOD) >= 256.
#endif
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(irc_read(IRC_IRR) & LIR6_BIT);
}

#endif /* _SYS_TIMER_ */
