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
 *  @(#) $Id: dispatch.S,v 1.4 1998/01/30 09:56:31 hiro Exp $
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
 *  DI=14 の遅延割込みから，スタックは SP0，IMASK は 14 で起動される．
 *  タスク独立部から起動されることはない．
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
 *  スパッチャ起動前の PC，PSW，R0〜R14) は，コンテキストとともに保存さ
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
 *  ↓	|    (ディスパッチャ起動前の) PC    |
 *  大	+-----------------------------------+ ← ディスパッチャ起動前の SP0
 *
 *
 *  ディスパッチャの前半では割込みは許可されているため，途中で schedtsk
 *  が書きかわる可能性があるために注意が必要である．これに対して，ctxtsk 
 *  はディスパッチャの中でのみ更新される．他の部分から ctxtsk を更新し
 *  てはならない．
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
 *  スパッチャの中で待つ．WAIT 命令の直後で DIR (遅延割込み要求レジスタ) 
 *  を 15 にすることで，タスクディスパッチャが再度起動されることを防止す
 *  る．この処理を dispatch_1 の直後で行うことで，ディスパッチャが起動後
 *  に dispatch_1 に到達するまでの間に DIR が書きかえられる状況でディス
 *  パッチャの再起動を防ぐことができるが，確率的に低く，効率はむしろ低下
 *  すると思われる．
 */
	.text
	.align 1
	.globl _dispatch_to_schedtsk
	.globl _dispatch_entry
_dispatch_to_schedtsk:
	stc @PSW, r0			/* スタックを SP0 に切替える */
	bset #0, r0
	ldc r0, @PSW
	ldc #15, @DIR			/* 遅延割込み要求の解除 */
	bra dispatch_1
_dispatch_entry:
	cmp @_ctxtsk, @_schedtsk	/* タスクスイッチが必要か？ */
	beq dispatch_3
	stm (r0-r14), @-sp		/* レジスタとコンテキストの保存 */
	stctx/ls
dispatch_1:
	ldc #0, @IMASK			/* 割込みを禁止 */
	mov @_schedtsk, r0		/* schedtsk があるか？ */
	bne dispatch_2
	mov #0, @_ctxtsk		/* ctxtsk を NULL に */
	wait #14			/* 割込み待ち */
	ldc #15, @DIR			/* 遅延割込み要求の解除 */
	bra dispatch_1
dispatch_2:
	mov r0, @_ctxtsk
	ldctx/ls @(TCB_tskctxb,r0)	/* レジスタとコンテキストの復帰 */
	ldm @sp+, (r0-r14)
dispatch_3:
	reit
