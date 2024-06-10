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
 *  @(#) $Id: cpu_support.S,v 1.5 1997/01/13 03:54:14 hiro Exp $
 */

#include <itron_errno.h>

#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"
#include "isysconf.h"

#include "tron_defs.h"
#include "offset.h"

/*
 *  タスクディスパッチャ
 *
 *  遅延割込みを使う場合と使わない場合で大きく違うため，混乱を避けるた
 *  めに，ファイルを分けている．
 */
#ifndef TRON_NO_DI
#include "dispatch.S"
#else /* TRON_NO_DI */
#include "nd_dispatch.S"
#endif /* TRON_NO_DI */

/*
 *  システムコール起動ルーチン
 *
 *  TRAPA を使ってシステムコールを起動するための分岐ルーチン．機能コー
 *  ドに従って，各システムコールの処理ルーチンへ分岐する．
 */
	.text
	.align 1
	.globl _trapa_ientry
_trapa_ientry:
	cmp #0, r6
	bgt _trapa_ientry_1
	cmp #FIRST_FNCD, r6		/* 機能コードの下限のチェック */
	blt _trapa_ientry_2
	jsr @@(_itable,r6*4)		/* システムコール処理ルーチンへ分岐 */
	reit
_trapa_ientry_1:
#ifdef USE_SVC
	jsr @_svc_ientry		/* 拡張SVCハンドラへの分岐 */
	reit
#endif /* USE_SVC */
_trapa_ientry_2:
	mov #E_RSFN, r0
	reit

/*
 *  未サポートシステムコールのエントリ
 */
	.text
	.align 1
	.globl _no_support
_no_support:
	mov #E_RSFN, r0
	rts

/*
 *  アセンブラで記述する必要があるシステムコール
 *
 *  システムコールエントリテーブルを差し替えるために，TRAPA で呼び出し
 *  た時の処理をアセンブラで記述する必要のあるシステムコール ???_??? 
 *  に対しては，_i_???_??? を _z_???_??? に定義する．
 */

/*
 *  ディスパッチの禁止/許可機能
 *
 *  TRON_NO_DI の時は，C言語で書いたルーチンを使える．
 */

#ifndef TRON_NO_DI
#ifndef _i_dis_dsp
#define _i_dis_dsp	_z_dis_dsp

	.text
	.align 1
	.globl _z_dis_dsp
_z_dis_dsp:
#ifdef CHK_CTX
	bfextu #12,#4,@(4,sp), r0	/* IMASK を EIT情報から取り出す */
	cmpu #14, r0			/* IMASK < 14 なら E_CTXエラー */
	blt _context_error
#endif /* CHK_CTX */
	bfinsu #14, #12,#4,@(4,sp)	/* IMASK を EIT情報中に設定 */
	mov #E_OK, r0
	rts

#endif /* _z_dis_dsp */
#ifndef _z_ena_dsp
#define _i_ena_dsp	_z_ena_dsp

	.text
	.align 1
	.globl _z_ena_dsp
_z_ena_dsp:
#ifdef CHK_CTX
	bfextu #12,#4,@(4,sp), r0	/* IMASK を EIT情報から取り出す */
	cmpu #14, r0			/* IMASK < 14 なら E_CTXエラー */
	blt _context_error
#endif /* CHK_CTX */
	bfinsu #15, #12,#4,@(4,sp)	/* IMASK を EIT情報中に設定 */
	mov #E_OK, r0
	rts

#endif /* _z_ena_dsp */
#endif /* TRON_NO_DI */

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
	btst #0,@(4,sp)			/* タスク独立部なら E_CTXエラー */
	beq _context_error
#endif /* CHK_CTX */
	bfinsu #0, #12,#4,@(4,sp)	/* IMASK を EIT情報中に設定 */
	mov #E_OK, r0
	rts

#endif /* _z_loc_cpu */
#ifndef _z_unl_cpu
#define _i_unl_cpu	_z_unl_cpu

	.text
	.align 1
	.globl _z_unl_cpu
_z_unl_cpu:
#ifdef CHK_CTX
	btst #0,@(4,sp)			/* タスク独立部なら E_CTXエラー */
	beq _context_error
#endif /* CHK_CTX */
	bfinsu #15, #12,#4,@(4,sp)	/* IMASK を EIT情報中に設定 */
#ifdef TRON_NO_DI
	mov #0, @_dispatch_disabled
	cmp @_ctxtsk, @_schedtsk	/* ctxtsk != schedtsk の時 */
        bne _dispatch			/* ディスパッチャを起動 */
#endif /* TRON_NO_DI */
	mov #E_OK, r0
	rts

#endif /* _z_unl_cpu */

/*
 *  IMASK の設定/参照機能
 */

#ifndef _z_chg_ims
#define _i_chg_ims	_z_chg_ims

	.text
	.align 1
	.globl _z_chg_ims
_z_chg_ims:
#ifdef CHK_PAR
	btst #0,@(4,sp)			/* タスク独立部なら z_chg_ims_1 へ */
	beq z_chg_ims_1
	cmpu #15, r0			/* IMASK > 15 なら E_PARエラー */
	bra z_chg_ims_2
z_chg_ims_1:
	cmpu #7, r0			/* IMASK > 7 なら E_PARエラー */
z_chg_ims_2:
	bgt _parameter_error
#endif /* CHK_PAR */
	bfinsu r0, #12,#4,@(4,sp)	/* IMASK を EIT情報中に設定 */
#ifdef TRON_NO_DI
	cmp #15, r0			/* 設定したIMASK値 == 15 かつ */
	bne z_chg_ims_3			/* dispatch_disabled == 0 かつ */
	cmp #0, @_dispatch_disabled	/* ctxtsk != schedtsk の時 */
	bne z_chg_ims_3			/* ディスパッチャを起動 */
	cmp @_ctxtsk, @_schedtsk
	bne _dispatch
#endif /* TRON_NO_DI */
z_chg_ims_3:
	mov #E_OK, r0
	rts

#endif /* _z_chg_ims */
#ifndef _z_ref_ims
#define _i_ref_ims	_z_ref_ims

	.text
	.align 1
	.globl _z_ref_ims
_z_ref_ims:
	bfextu #12,#4,@(4,sp), r1	/* IMASK を EIT情報から取り出す */
	mov r1, @r0
	mov #E_OK, r0
	rts

#endif /* _z_ref_ims */

/*
 *  ディスパッチャへの分岐ルーチン
 */
#ifdef TRON_NO_DI
	.text
	.align 1
_dispatch:
	trapa #4			/* ディスパッチャの起動 */
	mov #E_OK, r0
	rts
#endif /* TRON_NO_DI */

/*
 *  アセンブラで記述されたシステムコールの共通エラー処理ルーチン
 */
	.text
	.align 1

#ifdef CHK_CTX
_context_error:				/* コンテキストエラー */
	mov #E_CTX, r0
	rts
#endif /* CHK_CTX */

#ifdef CHK_PAR
_parameter_error:			/* パラメータエラー */
	mov #E_PAR, r0
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

/*
 *  タイマハンドラ起動のための高級言語対応ルーチン
 */
	.text
	.align 1
	.globl _timer_handler_startup
_timer_handler_startup:
	stm (r0-r6), @-sp
	bsr _timer_handler
	ldm @sp+, (r0-r6)
#ifndef TRON_NO_DI
	reit
#else /* TRON_NO_DI */
	bra __ret_int
#endif /* TRON_NO_DI */
