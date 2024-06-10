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
 *  @(#) $Id: cpu_support.S,v 1.5 1998/07/06 10:48:13 hiro Exp $
 */

#include <itron_errno.h>

#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"
#include "isysconf.h"

#include "offset.h"

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
 *  システムコール内から，スタックは MSP，IPM は 0 で，TRAP命令により起
 *  動される．タスク独立部から呼ばれたシステムコール内で起動されること
 *  はない．
 *
 *  起動方法2:
 * 
 *  スタートアップルーチンまたは自タスクを終了するシステムコール中から，
 *  スーパバイザモード，IPM は 0 の状態で，_dispatch_to_schedtsk への分
 *  岐命令で起動される．この場合，ctxtsk は無視して schedtsk にディス
 *  パッチする．起動時のコンテキストは，MSP を含めてどこにも保存されな
 *  い．スタートアップルーチンから起動される場合を除いては，タスク独立
 *  部から起動されないのが原則であるが，システムコールを発行したコンテ
 *  キストに戻らない E_CTXエラーを検出した場合には，やむをえずタスク独
 *  立部から起動されることになる．
 *
 *  タスクディスパッチャ内では，コンテキストスイッチにより MSP が切り替
 *  わるため，システムスタック上に保存されている情報 (具体的には，ディ
 *  スパッチャ起動前の PC，SR，D0〜D7，A0〜A6) は，コンテキストとともに
 *  保存される．システムスタック上でのフォーマットは，次のようになる．
 *
 *  小 	+-----------------------------------+ ← TCB 中に保存されている MSP
 *  ↑	|                D0                 |
 *	+-----------------------------------+
 *	|                D1                 |
 *	+-----------------------------------+
 *		    . . . . . .
 *	+-----------------------------------+
 *	|                A5                 |
 *	+-----------------------------------+
 *	|                A6                 |
 *	+-----------------+-----------------+
 *	|        SR       |   PC (上16bit)  |
 *	+-----------------+-----------------+
 *  ↓	|   PC (下16bit)  |    例外情報     |
 *  大	+-----------------+-----------------+ ← ディスパッチャ起動前の MSP
 *
 *  ディスパッチャの前半では割込みは許可されているため，途中で schedtsk
 *  が書きかわる可能性があるために注意が必要である．これに対して，ctxtsk 
 *  はディスパッチャの中でのみ更新される．他の部分から ctxtsk を更新し
 *  てはならない．また，ディスパッチャ実行中にかかった割込みハンドラの
 *  最後の ret_intシステムコールでタスクディスパッチを起こさないように，
 *  ディスパッチャ動作中は dispatch_disabled を TRUE にする．
 *
 *  ディスパッチャの後半は割込み禁止状態で動作する．これは，schedtsk を 
 *  A0 に入れた後に割込みがかかり，その割込みハンドラの中で schedtsk が
 *  削除された場合に schedtsk のシステムスタック領域が解放されてしまい，
 *  コンテキストの復帰および最後の RTE命令の動作が保証できなくなるため
 *  である．よって，タスク独立部から del_tsk の発行することを禁止し，タ
 *  スクのコンテキストが DORMANT状態においても有効であるなら，割込みを
 *  禁止する必要はないものと思われる．なお，割込み禁止の解除は，RTE命令
 *  によって行われる．
 *
 *  schedtsk が NULL の場合は，STOP命令を使って，そうでなくなるまでディ
 *  スパッチャの中で待つ．やはり，dispatch_disabled を TRUE にすること
 *  で，この間に起動された割込みハンドラの最後で呼ばれる ret_int の中で
 *  タスクスイッチが起こらないようにしている．
 */
	.text
	.align 1
	.globl _dispatch_to_schedtsk
	.globl _dispatch_entry
_dispatch_to_schedtsk:
	ori.w #0x1000, sr		/* スタックを MSP に切替える */
	move.l #1, _dispatch_disabled	/* タスクディスパッチを禁止 */
	bra dispatch_1
_dispatch_entry:
	move.l #1, _dispatch_disabled	/* タスクディスパッチを禁止 */
	movem.l d0-d7/a0-a6, -(sp)	/* レジスタを保存 */
	movea.l _ctxtsk, a5		/* コンテキストを保存 */
	move.l sp, (TCB_msp,a5)
	movec.l usp, a4
	move.l a4, (TCB_usp,a5)
dispatch_1:
	ori.w #0x0700, sr		/* 割込みを禁止 */
	movea.l _schedtsk, a5		/* schedtsk があるか？ */
	move.l a5, _ctxtsk
	bne dispatch_2
	stop #0x3000			/* 割込み待ち */
	bra dispatch_1
dispatch_2:
	move.l (TCB_msp,a5), sp		/* コンテキストを復帰 */
	move.l (TCB_usp,a5), a4
	movec.l a4, usp
	movem.l (sp)+, d0-d7/a0-a6	/* レジスタを復帰 */
	clr.l _dispatch_disabled	/* タスクディスパッチを許可 */
	rte

/*
 *  ret_intシステムコール処理ルーチン
 *
 *  _z_ret_int は TRAP を用いたシステムコール呼び出しインタフェースによ
 *  る入口，_i_ret_int はサブルーチンコールを用いたインタフェースによる
 *  入口である．__ret_int に BRA命令ないしは JMP命令で分岐する方法もあ
 *  る．
 *
 *  ret_int の処理は，最初にディスパッチが必要かをチェックする部分を除
 *  いては，タスクディスパッチャと類似の動作を行う．最大の違いは，タス
 *  クディスパッチャが MSP を使って動作するのに対して，ret_int の処理は
 *  タスク独立部から呼ばれるために ISP を使って動作することである．その
 *  ため，レジスタ等を保存する際に，A6 を MSP の代わりに用いている．
 *
 *  ret_int が呼ばれた後に schedtsk が書きかわる状況で，タスクスイッチ
 *  が間違いなく行われるよう，十分な配慮が必要である．そのために以下の
 *  ルーチンでは，ctxtsk と schedtsk の比較の前の時点で割込みを禁止して
 *  いる．割込み禁止をこの比較の後にした場合，比較の直後に割込みがかか
 *  りその中でタスクディスパッチが要求された場合に，タスクスイッチが起
 *  こらない (ret_int処理の中でかかった割込みハンドラからの ret_int で
 *  は，戻り先が IPM != 0 であるために，タスクスイッチは起こらないこと
 *  に注意)．
 */
	.text
	.align 1
	.globl _z_ret_int
	.globl _i_ret_int
	.globl __ret_int
_z_ret_int:
	addq.l #8, sp			/* TRAP のスタックを捨てる */
	bra __ret_int
_i_ret_int:
	addq.l #4, sp			/* 戻り番地を捨てる */
__ret_int:
	btst.b #4, (sp)			/* 戻り先が割込みハンドラなら終了 */
	beq ret_int_1			/*	SR のスタックモードで判定 */
	tst.l _dispatch_disabled	/* タスクディスパッチ禁止中か？ */
	bne ret_int_1
	move.l a5, -(sp)		/* A5, A6 をスタックに保存 */
	move.l a6, -(sp)
	movec.l msp, a6			/* タスクのシステムスタックを A6 に */
	bftst (a6){5,3}			/* 戻り先の IPM != 0 なら終了 */
	bne ret_int_2
	movea.l _ctxtsk, a5		/* ctxtsk を A5 に */
	ori.w #0x0700, sr		/* 割込みを禁止 */
	cmp.l _schedtsk, a5		/* タスクスイッチが必要か？ */
	bne ret_int_3
ret_int_2:
	move.l (sp)+, a6
	move.l (sp)+, a5
ret_int_1:
	rte

ret_int_3:
	move.l (sp)+, -(a6)		/* A6 をシステムスタックに積む */
	move.l (sp)+, -(a6)		/* A5 をシステムスタックに積む */
	movem.l d0-d7/a0-a4, -(a6)	/* 残りのレジスタを保存 */
	move.l a6, (TCB_msp,a5)		/* コンテキストを保存 */
	move.l usp, a4
	move.l a4, (TCB_usp,a5)
ret_int_4:
	movea.l _schedtsk, a5		/* schedtsk があるか？ */
	move.l a5, _ctxtsk
	bne ret_int_5
	stop #0x2000			/* 割込み待ち */
	ori.w #0x0700, sr		/* 割込みを禁止 */
	bra ret_int_4
ret_int_5:
	move.l (TCB_msp,a5), a6		/* コンテキストを復帰 */
	move.l (TCB_usp,a5), a4
	movec.l a4, usp
	movem.l (a6)+, d0-d7/a0-a5	/* レジスタを復帰 */
	move.l (a6)+, -(sp)
	movec.l a6, msp
	movea.l (sp)+, a6
	rte

/*
 *  システムコール起動ルーチン
 *
 *  TRAP を使ってシステムコールを起動するための分岐ルーチン．機能コー
 *  ドに従って，各システムコールの処理ルーチンへ分岐する．
 */
	.text
	.align 1
	.globl _trap_ientry
_trap_ientry:
	tst.l d0
	bpl _trap_ientry_1
	cmp.l #FIRST_FNCD, d0		/* 機能コードの下限のチェック */
	blt _trap_ientry_2
	move.l a6, -(sp)
	move.l sp, a6			/* スタックポインタを a6 に保存 */
	jmp ([_ptable,d0*4])		/* push_6 〜 push_0 へ分岐 */
push_6:
	move.l d6, -(sp)
push_5:
	move.l d5, -(sp)
push_4:
	move.l d4, -(sp)
push_3:
	move.l d3, -(sp)
push_2:
	move.l d2, -(sp)
push_1:
	move.l d1, -(sp)
push_0:
	jsr ([_itable,d0*4])		/* システムコール処理ルーチンへ分岐 */
	move.l a6, sp			/* スタックポインタを a6 から復帰 */
	movea.l (sp)+, a6
	rte
_trap_ientry_1:
#ifdef USE_SVC
	move.l d0, -(sp)
	move.l d6, -(sp)
	move.l d5, -(sp)
	move.l d4, -(sp)
	move.l d3, -(sp)
	move.l d2, -(sp)
	move.l d1, -(sp)
	jsr _svc_ientry			/* 拡張SVCハンドラへの分岐 */
	add.l #28, sp
	rte
#endif /* USE_SVC */
_trap_ientry_2:
	moveq.l #E_RSFN, d0
	rte

/*
 *  未サポートシステムコールのエントリ
 */
	.text
	.align 1
	.globl _no_support
_no_support:
	moveq.l #E_RSFN, d0
	rts

/*
 *  アセンブラで記述する必要があるシステムコール
 *
 *  システムコールエントリテーブルを差し替えるために，TRAP で呼び出し
 *  た時の処理をアセンブラで記述する必要のあるシステムコール ???_??? 
 *  に対しては，_i_???_??? を _z_???_??? に定義する．
 */

/*
 *  割込みの禁止/許可機能
 */

#ifndef _z_loc_cpu
#define _i_loc_cpu	_z_loc_cpu

	.text
	.align 1
	.globl _z_loc_cpu
_z_loc_cpu:
#ifdef CHK_CTX
	bfextu (4,sp){2:2}, d0
	cmp.l #2, d0			/* タスク独立部なら E_CTXエラー */
	beq _context_error
#endif /* CHK_CTX */
	bfset (4,sp){5:3}		/* スタックフレーム中の IPM を設定 */
	moveq.l #E_OK, d0
	rts

#endif /* _z_loc_cpu */
#ifndef _z_unl_cpu
#define _i_unl_cpu	_z_unl_cpu

	.text
	.align 1
	.globl _z_unl_cpu
_z_unl_cpu:
#ifdef CHK_CTX
	bfextu (4,sp){2:2}, d0
	cmp.l #2, d0			/* タスク独立部なら E_CTXエラー */
	beq _context_error
#endif /* CHK_CTX */
	bfclr (4,sp){5:3}		/* スタックフレーム中の IPM を設定 */
	clr.l _dispatch_disabled	/* dispatch_disabled をクリア */
	move.l _ctxtsk, d0		/* ctxtsk != schedtsk の時 */
	cmp.l _schedtsk, d0
	bne _dispatch			/* ディスパッチャを起動 */
	moveq.l #E_OK, d0
	rts

#endif /* _z_unl_cpu */

/*
 *  IPM の設定/参照機能
 */

#ifndef _z_chg_ipm
#define _i_chg_ipm	_z_chg_ipm

	.text
	.align 1
	.globl _z_chg_ipm
_z_chg_ipm:
#ifdef CHK_PAR
	move.l (4,sp), d0		/* パラメータを D0 に */
	cmp.l #7, d0			/* IPM > 7 なら E_PARエラー */
	bhi _parameter_error
	bfextu (8,sp){2:2}, d1		/* タスク独立部か? */
	cmp.l #2, d1
	bne z_chg_ipm_1			/* そうでないなら z_chg_ipm_1 へ */
	tst.l d0			/* IPM == 0 なら E_PARエラー */
	beq _parameter_error
z_chg_ipm_1:
#endif /* CHK_PAR */
	bfins d0, (4,sp){5:3}		/* スタックフレーム中の IPM を設定 */
	tst.l d0			/* 設定したIPM値 == 0 かつ */
	bne z_chg_ipm_2
	tst.l _dispatch_disabled	/* dispatch_disabled == 0 かつ */
	bne z_chg_ipm_2
	move.l _ctxtsk, d0		/* ctxtsk != schedtsk の時 */
	cmp.l _schedtsk, d0
	bne _dispatch			/* ディスパッチャを起動 */
z_chg_ipm_2:
	moveq.l #E_OK, d0
	rts

#endif /* _z_chg_ipm */
#ifndef _z_ref_ipm
#define _i_ref_ipm	_z_ref_ipm

	.text
	.align 1
	.globl _z_ref_ipm
_z_ref_ipm:
	movea.l (4,sp), a0		/* パラメータを A0 に */
	bfextu (8,sp){5:3}, d0		/* IPM をスタックフレームから読む */
	move.l d0, (a0)
	moveq.l #E_OK, d0
	rts

#endif /* _z_ref_ipm */

/*
 *  ディスパッチャへの分岐ルーチン
 */
	.text
	.align 1
_dispatch:
	trap #7				/* ディスパッチャの起動 */
	moveq.l #E_OK, d0
	rts

/*
 *  アセンブラで記述されたシステムコールの共通エラー処理ルーチン
 */
	.text
	.align 1

#ifdef CHK_CTX
_context_error:				/* コンテキストエラー */
	moveq.l #E_CTX, d0
	rts
#endif /* CHK_CTX */

#ifdef CHK_PAR
_parameter_error:			/* パラメータエラー */
	moveq.l #E_PAR, d0
	rts
#endif /* CHK_PAR */

/*
 *  システムコールエントリテーブル
 */
	.text
	.align 2
#include "itable.S"
_itable:
	.int _no_support		/* 機能番号 0 に対応するエントリ */

#include "ptable.S"
_ptable:
	.int push_0			/* 機能番号 0 に対応するエントリ */

/*
 *  タイマハンドラ起動のための高級言語対応ルーチン
 */
	.text
	.align 1
	.globl _timer_handler_startup
_timer_handler_startup:
	movem.l d0-d1/a0-a1, -(sp)
	jsr _timer_handler
	movem.l (sp)+, d0-d1/a0-a1
	jmp __ret_int
