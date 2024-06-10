/*
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
 *  @(#) $Id: start.S,v 1.2 1998/06/18 07:35:10 hiro Exp $
 */

/*
 *  m68k 用のスタートアップモジュール
 *
 *  ItIs カーネルと ItIs タスクで共通に用いる．
 */

	.globl start
	.text
	.even
start:
	/*
	 *  main 関数に渡す引数のデフォルト値の設定
	 *
	 *  d4〜d6 が 第1引数〜第3引数に対応している．_hardware_init_hook
	 *  や _software_init_hook 中で書き換えられることを想定している．
	 *  ここでは，ItIs のタスク用のデフォルトを設定する．
	 */
	move.l d0, d4
	move.l d1, d5
	clr.l d6

	/*
	 *  スタックポインタを __stack に初期化する (0 でない場合)．
	 *
	 *  __stack にスタックポインタの初期値を設定しておくと，スタッ
	 *  クポインタの初期化が行われる．ItIs 環境では使わない．
	 */
	movea.l #__stack, a0
	cmpa.l #0, a0
	beq start_0
	movea.l a0, sp	

	/*
	 *  bssセクションをクリアする．
	 */
start_0:
	movea.l #__bss_start, a0
	move.l #__end, d0
	sub.l a0, d0
	beq start_2
start_1:
	clr.b (a0)+
	dbra d0, start_1
	clr.w d0
	subq.l #1, d0
	bcc start_1

	/*
	 *  dataセクションを初期化する (ROM化対応)．
	 *
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする．
	 */
start_2:
	move.l #__idata_end, d0
	sub.l #__idata_start, d0
	bls start_4
	movea.l #__idata_start, a0
	movea.l #__data_start, a1
start_3:
	move.b (a0)+, (a1)+
	dbra d0, start_3
	clr.w d0
	subq.l #1, d0
	bcc start_3

	/*
	 *  _hardware_init_hook を呼び出す (0 でない場合)．
	 *
	 *  ハードウェア依存に必要な初期化処理がある場合は，
	 *  hardware_init_hook という関数を用意すればよい．ItIs カーネ
	 *  ルでは，sys_support.S 内で hardware_init_hook を定義してい
	 *  る．
	 */
start_4:
	movea.l #_hardware_init_hook, a0
	cmpa.l #0, a0
	beq start_5
	jsr (a0)

start_5:
	/*
	 *  _software_init_hook を呼び出す (0 でない場合)．
	 *
	 *  ソフトウェア環境依存に必要な初期化処理がある場合は，
	 *  software_init_hook という関数を用意すればよい．
	 */
	movea.l #_software_init_hook, a0
	cmpa.l #0, a0
	beq start_6
	jsr (a0)

	/*
	 *  atexit(__do_global_dtors);
	 */
start_6:
	move.l #___do_global_dtors, -(sp)
	jsr _atexit
	add.l #4, sp

	/*
	 *  ercd = main('d4', 'd5', 'd6');
	 */
	move.l d6, -(sp)
	move.l d5, -(sp)
	move.l d4, -(sp)
	jsr _main
	add.l #12, sp

	/*
	 *  exit(ercd);
	 *
	 *  ItIs カーネルを作る場合には，main から戻ることはないので，
	 *  以下は不要．
	 *  ItIs タスクは，本来 ter_tsk で終了すべきなので，ここへ戻っ
	 *  てこないのが正しい．ただし，ItIs タスクで ter_tsk しないと
	 *  ここへ戻ってくる．
	 */
	move.l d0, -(sp)
	jsr _exit
