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
 *  @(#) $Id: bsd_sigio.h,v 1.1 1998/07/06 14:17:27 hiro Exp $
 */

#ifndef	_BSD_SIGIO_
#define	_BSD_SIGIO_

/*
 *  BSD UNIX用 ノンブロッキングI/O サポートモジュール
 *
 *  ノンブロッキングI/O サポートモジュールは，SIGIO シグナルにより，ユー
 *  ザが登録したコールバック関数を呼び出す機能を持つ．
 *
 *  SIGIO シグナルによりコールバック関数を実行させたい場合には，SIGIO 
 *  通知イベントブロックを用意し，その callback フィールドにコールバッ
 *  ク関数，arg フィールドにコールバック関数へ渡す引数を設定し，
 *  eneuque_sigioeb を用いて SIGIO 通知イベントキューに登録する．
 *
 *  呼び出されたコールバック関数が 0 を返すと，関連する SIGIO 通知イベ
 *  ントブロック はキューに登録されたままとなり，続く SIGIO シグナルで
 *  再び同じコー ルバック関数が呼び出される．コールバック関数が 0 以外
 *  を返すと，SIGIO 通知イベントブロックはキューから削除され，コールバッ
 *  ク関数はそれ以降呼び出されなくなる．
 */

/*
 *  SIGIO 通知イベントブロックの定義
 */
typedef BOOL	(*SIGIO_CBACK)(VP);	/* SIGIO コールバック関数の型 */

typedef struct bsd_sigio_event_block {
	VP		queue[2];	/* SIGIO 通知イベントキューエリア */
	SIGIO_CBACK	callback;	/* SIGIO コールバック関数 */
	VP		arg;		/* コールバック関数へ渡す引数 */
} SIGIOEB;

/*
 *  SIGIO 通知イベントブロックの登録
 */
extern ER	enqueue_sigioeb(SIGIOEB *sigioeb);

#endif /* _BSD_SIGIO_ */
