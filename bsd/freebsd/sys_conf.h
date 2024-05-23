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
 *  @(#) $Id: sys_conf.h,v 1.3 1996/02/17 09:23:57 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  OS の種類の定義
 */
#ifndef __FreeBSD__
#define	__FreeBSD__		/* 通常はコンパイラが定義する */
#endif

/*
 *  CPU に依存する定数の設定
 *
 *  JMPBUF_PC: jmp_buf 構造体中で，PC の値を入れる場所のオフセット
 *  JMPBUF_SP: jmp_buf 構造体中で，SP の値を入れる場所のオフセット
 *
 *  LONGJMP_MERGIN: longjmp した後に，スタックポインタより上の番地を 
 *  アクセスする可能性がある場合の，マージン領域のサイズ．
 *
 *  SIGSTACK_MERGIN: シグナルスタックをプロセススタック上に取る時のマー
 *  ジン．スタートアップルーチンが使うためのスタック領域．
 */

#ifdef i386

#define JMPBUF_PC		0
#define JMPBUF_SP		2
#define LONGJMP_MERGIN		0

#define SIGSTACK_MERGIN		1024

#else /* i386 */

#error not supported.

#endif /* i386 */

/*
 *  シグナルスタックの変更に sigaltstack を使う
 */
#define USE_SIGALTSTACK

/*
 *  スタックエリアの取得/返却関数を mprotect を使うものに差しかえる
 */
#define	USE_MPROTECT_STACK

#ifdef _BSD_SERIAL_

#define USE_OLD_TTY
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
