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
 *  @(#) $Id: sys_conf.h,v 1.8 1998/06/08 10:52:27 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  OS の種類の定義
 */
#define	SUNOS4

/*
 *  CPU と _setjmp/_longjmp ライブラリの実装に依存する定数の設定
 *
 *  JMPBUF_PC: jmp_buf 構造体中で，PC の値を入れる場所のオフセット
 *  JMPBUF_SP: jmp_buf 構造体中で，SP の値を入れる場所のオフセット
 *
 *  STACK_MERGIN: スタックポインタの初期値を，スタック領域の上限 (確保
 *  した領域の次の番地) からど れだけ離すか．スタックポインタが指す番
 *  地から使い始めるプロセッサの の場合には，これを使ってスタックポイ
 *  ンタの初期値を上限から下げる．また，longjmp した後に，スタックポイ
 *  ンタより上の番地を アクセスする可能性がある場合には，これを使って
 *  マージン領域を確保する．
 *
 *  SIGSTACK_MERGIN: シグナルスタックをプロセススタック上に取る時のマー
 *  ジン．スタートアップルーチンが使うためのスタック領域．
 */

#ifdef sparc

#define JMPBUF_PC		3
#define JMPBUF_SP		2
#define STACK_MERGIN		256
#define SIGSTACK_MERGIN		8192

#else /* sparc */

#error not supported.

#endif /* sparc */

/*
 *  シグナルスタックの変更に sigstack を使う
 */
#define USE_SIGSTACK

/*
 *  スタックエリアの取得/返却関数を mprotect を使うものに差しかえる
 */
#define	USE_MPROTECT_STACK

/*
 *  疑似シリアルドライバのための定義
 */

#ifdef _BSD_SERIAL_

#include <sys/ioctl.h>

Inline void
set_rawmode(struct sgttyb *ttyb)
{
	ttyb->sg_flags |= (RAW | CBREAK);
	ttyb->sg_flags &= ~(ECHO);
}

#undef RAW			/* 衝突するシンボルの定義を抹消 */
#undef ECHO

#endif /* _BSD_SERIAL_ */
#endif /* _SYS_CONF_ */
