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
 *  @(#) $Id: sys_timer.h,v 1.3 1996/02/17 09:23:42 hiro Exp $
 */

#ifndef _SYS_TIMER_
#define _SYS_TIMER_

#include <sys/time.h>

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
 *  void start_hw_timer(FP handler) : タイマを初期化し，周期的なタイマ
 *  割込みを開始させる．handler は，周期的に起動すべきハンドラの開始ア
 *  ドレス．
 *
 *  void clear_hw_timer_interupt(void) : タイマ割込み要求をクリアする．
 *  タイマ割込みハンドラの最初で呼ばれる．
 *
 *  void terminate_hw_timer(void) : タイマの動作を停止させる．システム
 *  停止時に呼ばれる．
 */ 

/*
 *  タイマ割込みの周期 (単位は msec)
 */
#define TIMER_PERIOD	10

/*
 *  タイマハンドラ
 */
extern void	timer_handler(void);

/*
 *  タイマのスタート処理
 */
Inline void
start_hw_timer(void)
{
	struct sigvec	vec;
	struct itimerval val;

	vec.sv_handler = timer_handler;
	vec.sv_mask = SIGMASK_INDP;
	vec.sv_flags = SV_ONSTACK|SV_INTERRUPT;
	sigvec(SIGALRM, &vec, 0);

	val.it_interval.tv_sec = TIMER_PERIOD / 1000;
	val.it_interval.tv_usec = (TIMER_PERIOD % 1000) * 1000;
	val.it_value = val.it_interval;
	setitimer(ITIMER_REAL, &val, 0);
}

/*
 *  タイマ割込みのクリア
 */
Inline void
clear_hw_timer_interrupt(void)
{
}

/*
 *  タイマの停止処理
 */
Inline void
terminate_hw_timer(void)
{
	struct itimerval val;

	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = 0;
	val.it_value = val.it_interval;
	setitimer(ITIMER_REAL, &val, 0);
}

#endif /* _SYS_TIMER_ */
