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
 *  @(#) $Id: nd_dispatch.S,v 1.3 1996/02/17 09:43:23 hiro Exp $
 */

/*
 *  タスクディスパッチャ
 * 
 *  実行中のタスク (ctxtsk) のコンテキストを TCB に保存し，実行すべきタ
 *  スク (schedtsk) を新しい実行タスクとして，そのコンテキストを TCB か
 *  ら復帰する．
 *
 *  ディスパッチャは，以下の 2通りの方法で起動される．
 * 
 *  起動方法1:
 * 
 *  システムコール内から，スタックは SP0，IMASK は 15 で，TRAPA命令によ
 *  り起動される．タスク独立部から呼ばれたシステムコール内で起動される
 *  ことはない．
 *
 *  起動方法2:
 * 
 *  スタートアップルーチンまたは自タスクを終了するシステムコール中から，
 *  リングレベル，IMASK ともに 0 の状態で，_dispatch_to_schedtsk への分
 *  岐命令で起動される．この場合，ctxtsk は無視して schedtsk にディス
 *  パッチする．起動時のコンテキストは，SP0 を含めてどこにも保存されな
 *  い．スタートアップルーチンから起動される場合を除いては，タスク独立
 *  部から起動されないのが原則であるが，システムコールを発行したコンテ
 *  キストに戻らない E_CTXエラーを検出した場合には，やむをえずタスク独
 *  立部から起動されることになる．
 *
 *  タスクディスパッチャ内では，コンテキストスイッチにより SP0 が切り替
 *  わるため，システムスタック上に保存されている情報 (具体的には，ディ
 *  スパッチャ起動前の PC，PSW，R0～R14) は，コンテキストとともに保存さ
 *  れる．これが，ディスパッチャを SP0 で動かす理由である．実際には，シ
 *  ステムスタック上でのフォーマットは，次のようになる．
 *
 *  小 	+-----------------------------------+ ← TCB 中に保存されている SP0
 *  ↑	|                R0                 |
 *	+-----------------------------------+
 *	|                R1                 |
 *	+-----------------------------------+
 *		    . . . . . .
 *	+-----------------------------------+
 *	|               R14                 |
 *	+-----------------------------------+
 *	|    (ディスパッチャ起動前の) PSW   |
 *	+-----------------------------------+
 *	| (ディスパッチャ起動による) EITINF |
 *	+-----------------------------------+
 *	|    (ディスパッチャ起動前の) PC    |
 *	+-----------------------------------+
 *  ↓	|  (ディスパッチャ起動による) EXPC  |
 *  大	+-----------------------------------+ ← ディスパッチャ起動前の SP0
 *
 *  最後にある EXPC は，TRAPA命令でタスクディスパッチャが起動された場合
 *  には，TRAPA命令の置かれている番地が入ることになる．ret_int によって
 *  コンテキストが保存された場合には，TRAPA命令は使われないため，EXPC 
 *  に入る情報は意味を持たない (実際には 0 を入れているが，省略しても問
 *  題ないだろう)．
 *
 *  ディスパッチャの前半では割込みは許可されているため，途中で schedtsk
 *  が書きかわる可能性があるために注意が必要である．これに対して，ctxtsk 
 *  はディスパッチャの中でのみ更新される．他の部分から ctxtsk を更新し
 *  てはならない．また，ディスパッチャ実行中にかかった割込みハンドラの
 *  最後の ret_intシステムコールでタスクディスパッチを起こさないように，
 *  ディスパッチャ動作中は dispatch_disabled を TRUE にする．
 *
 *  ディスパッチャの後半は割込み禁止状態で動作する．これは，schedtsk を 
 *  R0 に入れた後に割込みがかかり，その割込みハンドラの中で schedtsk が
 *  削除された場合に schedtsk のシステムスタック領域が解放されてしまい，
 *  コンテキストの復帰および最後の REIT命令の動作が保証できなくなるため
 *  である．よって，タスク独立部から del_tsk の発行することを禁止し，タ
 *  スクのコンテキストが DORMANT状態においても有効であるなら，割込みを
 *  禁止する必要はないものと思われる．なお，割込み禁止の解除は，REIT命
 *  令によって行われる．
 *
 *  schedtsk が NULL の場合は，WAIT命令を使って，そうでなくなるまでディ
 *  スパッチャの中で待つ．やはり，dispatch_disabled を TRUE にすること
 *  で，この間に起動された割込みハンドラの最後で呼ばれる ret_int の中で
 *  タスクスイッチが起こらないようにしている．
 */
	.text
	.align 1
	.globl _dispatch_to_schedtsk
	.globl _dispatch_entry
_dispatch_to_schedtsk:
	stc @PSW, r0			/* スタックを SP0 に切替える */
	bset #0, r0
	ldc r0, @PSW
	mov #1, @_dispatch_disabled	/* タスクディスパッチを禁止 */
	bra dispatch_1
_dispatch_entry:
	mov #1, @_dispatch_disabled	/* タスクディスパッチを禁止 */
	stm (r0-r14), @-sp		/* レジスタとコンテキストの保存 */
	stctx/ls
dispatch_1:
	ldc #0, @IMASK			/* 割込みを禁止 */
	mov @_schedtsk, r0		/* schedtsk があるか？ */
	bne dispatch_2
	mov #0, @_ctxtsk		/* ctxtsk を NULL に */
	wait #15			/* 割込み待ち */
	bra dispatch_1
dispatch_2:
	mov r0, @_ctxtsk
	ldctx/ls @(TCB_tskctxb,r0)	/* レジスタとコンテキストの復帰 */
	ldm @sp+, (r0-r14)
	mov #0, @_dispatch_disabled	/* タスクディスパッチを許可 */
	reit

/*
 *  ret_intシステムコール処理ルーチン
 *
 *  _z_ret_int は TRAPA を用いたシステムコール呼び出しインタフェースに
 *  よる入口，_i_ret_int はサブルーチンコールを用いたインタフェースによ
 *  る入口である．__ret_int に BRA命令ないしは JMP命令で分岐する方法も
 *  ある．
 *
 *  ret_int の処理は，最初にディスパッチが必要かをチェックする部分を除
 *  いては，タスクディスパッチャと類似の動作を行う．最大の違いは，タス
 *  クディスパッチャが SP0 を使って動作するのに対して，ret_int の処理は
 *  タスク独立部から呼ばれるために SPI を使って動作することである．その
 *  ため，レジスタ等を保存する際に，R0 を SP0 の代わりに用いている．ま
 *  た，戻り先の PC や PSW を，スタックフレームの中から取り出して保存す
 *  る必要がある．
 *
 *  一方，次に実行すべきタスクのレジスタ等の復帰は，タスクディスパッチ
 *  ルーチンに任せている．タスクディスパッチルーチンに分岐する際に REIT
 *  命令を用いているのは，EITスタック情報に保存されている処理を行い，
 *  EITスタック情報を解放するためである．
 *
 *  ret_int が呼ばれた後に schedtsk が書きかわる状況で，タスクスイッチが
 *  間違いなく行われるよう，十分な配慮が必要である．そのために以下のルー
 *  チンでは，ctxtsk と schedtsk の比較の前の時点で割込みを禁止している．
 *  割込み禁止をこの比較の後にした場合，比較の直後に割込みがかかりその
 *  中でタスクディスパッチが要求された場合に，タスクスイッチが起こらな
 *  い (ret_int処理の中でかかった割込みハンドラからの ret_int では，戻
 *  り先が IMASK != 15 であるために，タスクスイッチは起こらないことに注
 *  意)．
 */
	.text
	.align 1
	.globl _z_ret_int
	.globl _i_ret_int
	.globl __ret_int
_z_ret_int:
	add #16, sp			/* TRAPA の EIT情報を捨てる */
	bra __ret_int
_i_ret_int:
	add #4, sp			/* 戻り番地を捨てる */
__ret_int:
	bfcmpu #15, #12,#4,@sp		/* EIT情報中の IMASK != 15 なら終了 */
	bne ret_int_1
	cmp #0, @_dispatch_disabled	/* タスクディスパッチ禁止中か？ */
	bne ret_int_1
	ldc #0, @IMASK			/* 割込みを禁止 */
	cmp @_ctxtsk, @_schedtsk	/* タスクスイッチが必要か？ */
	beq ret_int_1

	mov #1, @_dispatch_disabled	/* タスクディスパッチを禁止 */
	push r0
	stc @SP0, r0
	sub #76, r0
	ldc r0, @SP0
	mov #0, @(72,r0)		/* EXPC (ダミー) の設定 */
	mov @(12,sp), @(68,r0)		/* 戻り番地の保存 */
	mov #TRAPA4_EITINF, @(64,r0)	/* EITINF の設定 */
	mov @(4,sp), @(60,r0)		/* PSW の保存 */
	stm (r1-r14), @(4,r0)		/* レジスタの保存 */
	pop @r0
	stctx/ls			/* コンテキストの保存 */
	mov #dispatch_1, @(8,sp)	/* 戻り先を dispatch_1 に */
	mov #DISPATCHER_PSW, @sp	/* PSW を変更 */
ret_int_1:
	reit
