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
 *  @(#) $Id: sys_serial.h,v 1.4 1996/02/17 09:44:00 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  MCUBE用 低レベルシリアルI/O 関連の定義
 */

#include "mcube.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct raw_serial_port_descripter {
	BOOL	*initflag;	/* 初期化済フラグへのポインタ */
	IOREG	*data;		/* データレジスタの番地 */
	IOREG	*cntrl;		/* コントロールレジスタの番地 */

	byte	com_reset;	/* ポートリセットコマンド */
	byte	mode3_def;	/* デフォールトのモード設定値 (WR3) */
	byte	mode4_def;	/* デフォールトのモード設定値 (WR4) */
	byte	mode5_def;	/* デフォールトのモード設定値 (WR5) */
	byte	mode12_def;	/* デフォールトのモード設定値 (WR12) */
	byte	mode13_def;	/* デフォールトのモード設定値 (WR13) */
} RPORT;

/*
 *  SCCコントロールレジスタの設定値
 */

#define MODE4_DEF	0x44		/* ストップビット 1bit, パリティなし */
#define MODE3_DEF	0xc1		/* データ 8bit, 受信イネーブル */
#define MODE3_INIT_MASK	0xfe		/* 受信ディスエーブル */
#define MODE5_DEF	0xea		/* データ 8bit, 送信イネーブル */
#define MODE5_INIT_MASK	0x75		/* 送信ディスエーブル */

#define MODE10_DEF	0x00		/* NRZ */
#define MODE11_DEF	0x56		/* クロックに関する設定 */
#define MODE12_DEF	10		/* 9600bps */
#define MODE13_DEF	0		/* 9600bps */
#define MODE14_DEF	0x03		/* ボーレートジェネレータ */

#define MODE1_DEF	0x13		/* 各割込みを許可 */
#define MODE1_DOWN	0x00		/* 全割込みを禁止 */
#define MODE15_DEF	0x00		/* その他の割込みの禁止 */
#define MODE9_INIT	0x02		/* マスター割込み禁止 */
#define MODE9_DEF	0x0a		/* マスター割込み許可 */
#define MODE9_DOWN	0x02		/* マスター割込み禁止 */

#define MODE4_KBD	0x45		/* ストップビット 1bit, 奇数パリティ */
#define MODE3_KBD	0xc1		/* データ 8bit, 受信イネーブル */
#define MODE5_KBD	0x68		/* データ 8bit, 送信イネーブル */
#define MODE12_KBD	4		/* 19200bps */
#define MODE13_KBD	0		/* 19200bps */

/*
 *  割込みベクタとハンドラアドレスの取り出し
 */
#define raw_int_vector(p)	IRC1_VECTOR(3)
#define raw_int_handler(p)	int_handler_scc

/*
 *  割込みハンドラのエントリ (前方宣言)
 */
static void	int_handler_scc(void);

static void	serial_int_handler(int portid);

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	2	/* サポートするシリアルポートの数 */

static int	initflag[2] = { -1, 0 } ;	/* 初期化済フラグ */

#define RAWPORT1	{ &initflag[0], SCC_DATAB, SCC_CNTRLB,		\
			  0x40, MODE3_DEF, MODE4_DEF, MODE5_DEF,	\
			  MODE12_DEF, MODE13_DEF }
#define RAWPORT2	{ &initflag[1], SCC_DATAA, SCC_CNTRLA,		\
			  0x80, MODE3_KBD, MODE4_KBD, MODE5_KBD,	\
			  MODE12_KBD, MODE13_KBD }

#define IOCTL_PORT2	{ 0, 0, 0, 0 }

/*
 *  シリアル I/O ポートの初期化
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  SCC の設定
	 */
	io_write(p->cntrl, SCC_WR0);			/* WR0 指定 */
	if (*(p->initflag) == 0) {
		scc_write(p->cntrl, SCC_WR9, p->com_reset);

		scc_write(p->cntrl, SCC_WR4, p->mode4_def);
		scc_write(p->cntrl, SCC_WR3, (p->mode3_def & MODE3_INIT_MASK));
		scc_write(p->cntrl, SCC_WR5, (p->mode5_def & MODE5_INIT_MASK));

		scc_write(p->cntrl, SCC_WR9, MODE9_INIT);
		scc_write(p->cntrl, SCC_WR10, MODE10_DEF);
		scc_write(p->cntrl, SCC_WR11, MODE11_DEF);
		scc_write(p->cntrl, SCC_WR12, p->mode12_def);
		scc_write(p->cntrl, SCC_WR13, p->mode13_def);
		scc_write(p->cntrl, SCC_WR14, MODE14_DEF);

		scc_write(p->cntrl, SCC_WR3, p->mode3_def);
		scc_write(p->cntrl, SCC_WR5, p->mode5_def);
	}

	/*
	 *  IRC の設定
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		irc_assign(IRC1_LMR(3), LIR3_BIT);
		irc_and_assign(IRC1_TMR, ~LIR3_BIT);	/* レベルトリガ */
		irc_and_assign(IRC1_IMR, ~LIR3_BIT);	/* 割込みマスク解除 */
	}

	/*
	 *  SCC の割込み関係の設定
	 */
	scc_write(p->cntrl, SCC_WR15, MODE15_DEF);
	scc_write(p->cntrl, SCC_WR1, MODE1_DEF);
	io_write(p->cntrl, 0x28);
	scc_write(p->cntrl, SCC_WR9, MODE9_DEF);

	*(p->initflag) = 1;				/* 初期化フラグ設定 */
	return(0);
}

/*
 *  シリアル I/O ポートのシャットダウン
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	*(p->initflag) = -1;				/* 初期化フラグ設定 */

	/*
	 *  SCC の割込み関係の設定
	 */
	scc_write(p->cntrl, SCC_WR1, MODE1_DOWN);
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		scc_write(p->cntrl, SCC_WR9, MODE9_DOWN);
	}

	/*
	 *  IRC の設定
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		irc_or_assign(IRC1_IMR, LIR3_BIT);	/* 割込みマスク設定 */
	}
}

/*
 *  割込みハンドラのエントリ
 */
static void
int_handler_scc(void)
{
	if (initflag[0] > 0) {
		serial_int_handler(1);
	}
	if (initflag[1] > 0) {
		serial_int_handler(2);
	}
}

/*
 *  割込みクリア処理
 */
Inline void
raw_port_clear_int(RPORT *p)
{
	io_write(p->cntrl, 0x10);
}

/*
 *  文字を受信したか？
 */
Inline BOOL
raw_port_getready(RPORT *p)
{
	return(scc_read(p->cntrl, SCC_RR0) & 0x01);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(scc_read(p->cntrl, SCC_RR0) & 0x04);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
raw_port_getchar(RPORT *p)
{
	return(io_read(p->data));
}

/*
 *  送信する文字の書き込み
 */
Inline void
raw_port_putchar(RPORT *p, byte c)
{
	io_write(p->data, c);
}

/*
 *  送信制御関数
 */
Inline void
raw_port_sendstart(RPORT *p)
{
}

Inline void
raw_port_sendstop(RPORT *p)
{
	io_write(p->cntrl, 0x28);
}

#endif /* _SYS_SERIAL_ */
