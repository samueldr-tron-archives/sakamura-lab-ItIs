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
 *  @(#) $Id: timer.h,v 1.4 1996/02/17 09:33:56 hiro Exp $
 */

#ifndef _TIMER_
#define _TIMER_

/*
 *  タイマモジュール
 */

#include "queue.h"

/* 
 *  タイマイベントブロックの定義
 */

typedef void	(*CBACK)(VP);	/* コールバック関数の型 */

typedef struct timer_event_block {
	QUEUE	queue;		/* タイマイベントキュー */
	SYSTIME	time;		/* イベント時刻 */
	CBACK	callback;	/* コールバック関数 */
	VP	arg;		/* コールバック関数へ渡す引数 */
} TMEB;

/*
 *  現在時刻 (ソフトウェアクロック)
 */
extern SYSTIME	current_time;

/*
 *  タイマの初期化と停止
 */
extern void	timer_initialize(void);
extern void	timer_shutdown(void);

/*
 *  タイマイベントのタイマキューへの登録
 */
extern void	timer_insert(TMEB *event, TMO tmout,
				CBACK callback, VP arg);
extern void	timer_insert_abs(TMEB *event, SYSTIME *time,
				CBACK callback, VP arg);
extern void	timer_insert_rel(TMEB *event, SYSTIME *time,
				CBACK callback, VP arg);

/*
 *  タイマキューからの削除
 */
Inline void
timer_delete(TMEB *event)
{
	queue_delete(&(event->queue));
}

#endif /* _TIMER_ */
