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
 *  @(#) $Id: sys_timer.h,v 1.5 1998/01/30 09:57:05 hiro Exp $
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

#include "mcube.h"

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
 *  MCUBE 基本ボードでは，タイマ (カウンタ2 を用いる) は 1μsec 毎にカ
 *  ウントアップする．
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
#define GET_TOLERANCE	(TIMER_TICK / 10)

/*
 *  設定できる最大のタイマ周期 (単位は内部表現)
 *
 *  内部表現で 0xffff は約 65msec．
 */
#define MAX_TICK	((TICK) 0xffff)

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
	define_eit(IRC1_VECTOR(1), EITATR(0, 15), timer_handler_startup);

	/*
	 *  IRC の設定
	 */
	irc_assign(IRC1_LMR(1), LIR1_BIT);
	irc_or_assign(IRC1_TMR, LIR1_BIT);	/* エッジトリガモード */
	irc_and_assign(IRC1_IMR, ~LIR1_BIT);	/* 割込みマスク解除 */

	/*
	 *  タイマの設定
	 */
	tmr_write(TMR_CNTL, 0xb4);		/* タイマモード2 */
	assert(t <= MAX_TICK);			/* タイマ上限値のチェック */
	tmr_write(TMR_CNT2, t & 0xff);
	tmr_write(TMR_CNT2, (t >> 8) & 0xff);

	irc_assign(IRC1_IRR, LIR1_BIT);		/* 割込み要求をクリア */
}

/*
 *  タイマ割込みのクリア
 */
Inline void
clear_hw_timer_interrupt(void)
{
	irc_assign(IRC1_IRR, LIR1_BIT);		/* 割込み要求をクリア */
}

/*
 *  タイマの停止処理
 */
Inline void
terminate_hw_timer(void)
{
	irc_or_assign(IRC1_IMR, LIR1_BIT);	/* 割込みマスク設定 */
}

/*
 *  タイマの現在値の読み出し
 *
 *  割込み禁止区間中で呼び出すこと．
 *
 *  本来，0 〜 TO_TICK(TIMER_PERIOD)-1 の値が返るはずだが，実際には頻度
 *  は低いが TO_TICK(TIMER_PERIOD) が返ることがある．
 */
Inline TICK
get_current_hw_time(void)
{
	TICK	t;

	tmr_write(TMR_CNTL, 0x80);		/* タイマ値読み出し開始 */
	t = tmr_read(TMR_CNT2);
	t += tmr_read(TMR_CNT2) << 8;
	return(TO_TICK(TIMER_PERIOD) - t);
}

Inline BOOL
fetch_hw_timer_interrupt(void)
{
	return(irc_read(IRC1_IRR) & LIR1_BIT);
}

#endif /* _SYS_TIMER_ */
