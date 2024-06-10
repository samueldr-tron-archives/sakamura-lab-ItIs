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
 *  @(#) $Id: sys_support.S,v 1.3 1998/06/18 07:44:09 hiro Exp $
 */

#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"
#include "isysconf.h"

/*
 *  DVE68K/40 CPUボード上のレジスタアドレスの定義
 */
#define BOARD_REG0	0xfff48000	/* DIPスイッチ */
#define BOARD_REG1	0xfff48004	/* ボードコントロールレジスタ */
#define BOARD_REG2	0xfff48008	/* フラッシュメモリ割込みクリア */

#define DGA_CSR0	0xfff44000	/* DGA-001 のレジスタ */
#define DGA_CSR1	0xfff44004

/*
 *  DVE68K/40 CPUボード用 ハードウェア依存初期化処理
 *
 *  スタートアップモジュールの中で，main を呼び出す前に呼び出される．
 */
	.text
	.align 1
	.globl _hardware_init_hook
_hardware_init_hook:
dve68k_start:
	move.l (sp)+, a0		/* 戻り番地を a0 に */

	move.w sr, d0
	or.w #0x2000, d0		/* スーパバイザモードに */
	and.w #0xe8ff ,d0		/* 割込みモード, IPL=0 に設定 */
	move.w d0, sr
	movea.l #STACKTOP, sp		/* スタックポインタ (SPI) の初期化 */
	movea.l #0, fp			/* フレームポインタの初期化 */

	cinva bc			/* すべてのキャッシュを無効化 */
	move.l #0x80008000, d0		/* 両方のキャッシュを ON */
	movec.l d0, cacr

	move.l BOARD_REG0, d0		/* DIPスイッチを読む */
	and.l #0x1f, d0			/* board_id の初期化 */
	move.l d0, _board_id
	asl.l #8, d0			/* board_addr の初期化 */
	asl.l #8, d0
	asl.l #8, d0
	move.l d0, _board_addr
	move.l #0x09, BOARD_REG1	/* コントロールレジスタの初期化 */

	move.l #0x2c07e000, DGA_CSR0	/* CSR0 の初期化 */
	move.l #0x60000300, DGA_CSR1	/* CSR1 の初期化 */

	jmp (a0)			/* 呼び元へ戻る */

/*
 *  低レベルメモリエリア確保ルーチン
 *
 *  void *sbrk(int size)
 */
	.text
	.align 1
	.globl _sbrk
_sbrk:
	movea.l _break, a0		/* _break の現在値を取り出す */
	move.l a0, d0			/* 更新前の _break を返す */
	add.l -4(sp), a0
	bcs sbrk_overflow		/* メモリフルのチェック */
	cmp.l #HEAPLIMIT, a0
	bhi sbrk_overflow
	move.l a0, _break		/* _break の更新 */
	rts
sbrk_overflow:
	moveq.l #-1, d0			/* オーバーフローの場合は NADR */
	rts

	.data
	.align 2
_break:
	.int HEAPTOP

/*
 *  Access Fault ハンドラ起動のための高級言語対応ルーチン
 */
	.text
	.align 1
	.globl _afault_handler_startup
_afault_handler_startup:
	movem.l d0-d1/a0-a1, -(sp)
	pea.l (16,sp)
	jsr _afault_handler
	addq.l #4, sp
	movem.l (sp)+, d0-d1/a0-a1
	rte
