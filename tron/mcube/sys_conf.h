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
 *  @(#) $Id: sys_conf.h,v 1.4 1996/02/17 09:43:58 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  ItIs 動作時のメモリマップ (デフォルト時)
 *
 *	80000000 - 8000ffff	ROMモニタワークエリア
 *	80020000 -		カーネルコード領域
 *		 - 8007efff	タスク独立部用スタック領域 (ItIs の SPI)
 *	8007f000 - 8007ffff	EITベクタテーブル (実際はこの半分のみ使用)
 *	80080000 - 800fffff	カーネルデータ領域 (大きくすることも可能)
 *	80100000 -		未使用 (ユーザプログラム領域)
 *		 - 80ffffff	ROMモニタの初期SPI (数バイト壊される)
 */

/*
 *  スタックエリア，ヒープエリアの定義
 *
 *  _end は，プログラムで使用するデータエリアの最後の次の番地．
 */
#define STACKTOP	0x8007f000	/* タスク独立部用スタックの初期値 */
#define HEAPTOP		_end		/* ヒープエリアの先頭 */
#define HEAPLIMIT	0x80100000	/* ヒープエリアの上限 */

/*
 *  EITベクタテーブル関連の定義
 *
 *  MCUBE の ROMモニタ (BMS) では，EITベクタテーブルを FFFFF000 番地か
 *  ら ROM に置いている．そこで，ItIs 起動時に，EITベクタテーブルを RAM 
 *  上に作る必要がある．EITVB_ITIS 番地に，EITベクタテーブルの初期設定
 *  を ROM からコピーして使う．
 */
#define EITVB_ORIG	((EITVE *) 0xfffff000)
#define EITVB_ITIS	((EITVE *) 0x8007f000)
#define EITVT_LEN	0x800

/*
 *  システムタスクに関する定義
 */
#define	CONSOLE_PORT	1	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORT	1	/* システムログを出力するシリアルポート番号 */

#endif /* _SYS_CONF_ */
