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
 *  @(#) $Id: sys_support.S,v 1.4 1998/01/30 09:57:05 hiro Exp $
 */

#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"
#include "isysconf.h"

#include "tron_defs.h"

/*
 *  MCUBE用スタートアップモジュール
 */
	.text
	.align 1
	.globl ___sys_init
___sys_init:
mcube_start:
	mov r0, r13			/* 戻り番地を R13 に保存 */
	stc @PSW, r0
	and #0x1fffffff, r0		/* SMRNG を 0 に設定 */
	ldc r0, @PSW
	mov #STACKTOP, sp		/* スタックポインタ (SPI) の初期化 */
	mov #0, fp			/* フレームポインタの初期化 */
	bsr ___rawio_init		/* ライブラリの初期化 */
	jmp @r13

/*
 *  低レベルメモリエリア確保ルーチン
 *
 *  void *sbrk(int size)
 */
	.text
	.align 1
	.globl _sbrk
_sbrk:
	mov @_break, r1			/* _break の現在値を取り出す */
	add r1, r0
	bxs sbrk_overflow		/* メモリフルのチェック */
	cmpu #HEAPLIMIT, r0
	bgt sbrk_overflow
	mov r0, @_break			/* _break の更新 */
	mov r1, r0			/* 更新前の _break を返す */
	rts
sbrk_overflow:
	mov #-1, r0			/* オーバーフローの場合は NADR */
	rts

	.data
	.align 2
_break:
	.int HEAPTOP
