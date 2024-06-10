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
 *  @(#) $Id: sys_serial.h,v 1.2 1998/06/18 07:44:08 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  DVE68K/40 CPUボード用 低レベルシリアルI/O 関連の定義
 */

#include "dve68k.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct raw_serial_port_descripter {
	INT	*initflag;	/* 初期化済フラグへのポインタ */
	IOREG	*data;		/* データレジスタの番地 */
	IOREG	*cntrl;		/* コントロールレジスタの番地 */
	BOOL	sendflag;	/* 送信割込みイネーブルフラグ */

	byte	cr3_def;	/* デフォールトの設定値 (CR3) */
	byte	cr4_def;	/* デフォールトの設定値 (CR4) */
	byte	cr5_def;	/* デフォールトの設定値 (CR5) */
	byte	brg2_def;	/* デフォールトの設定値 (ボーレート下位) */
	byte	brg1_def;	/* デフォールトの設定値 (ボーレート上位) */
} RPORT;

/*
 *  MPSCコントロールレジスタの設定値
 */

#define MPSC_RESET	0x18		/* ポートリセットコマンド */
#define MPSC_EOI	0x38		/* EOI (End of Interrupt) */

#define CR3_DEF		0xc1		/* データ 8bit, 受信イネーブル */
#define CR4_DEF		0x44		/* ストップビット 1bit, パリティなし */
#define CR5_DEF		0xea		/* データ 8bit, 送信イネーブル */
#define BRG2_DEF	0x1e		/* 9600bps (下位) */
#define BRG1_DEF	0x00		/* 9600bps (上位) */

#define CR10_DEF	0x00		/* NRZ */
#define CR14_DEF	0x07		/* ボーレートジェネレータイネーブル */
#define CR15_DEF	0x56		/* ボーレートジェネレータ使用 */

#define CR1_ALL		0x12		/* 全割込みを許可 */
#define CR1_RECV	0x10		/* 受信割込みのみ許可 */
#define CR1_DOWN	0x00		/* 全割込みを禁止 */

/*
 *  割込みベクタとハンドラアドレスの取り出し
 */
#define raw_int_vector(p)	GP0_VEC
#define raw_int_handler(p)	int_handler_mpsc

/*
 *  割込みハンドラのエントリ (前方宣言)
 */
static void	int_handler_mpsc(void);

static void	serial_int_handler(int portid);

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	2	/* サポートするシリアルポートの数 */

static BOOL	initflag[2] = { 0, 0 } ;	/* 初期化済フラグ */

#define RAWPORT1	{ &initflag[0], MPSC_DATAA, MPSC_CNTRLA, 0,	\
			  CR3_DEF, CR4_DEF, CR5_DEF, BRG2_DEF, BRG1_DEF }
#define RAWPORT2	{ &initflag[1], MPSC_DATAB, MPSC_CNTRLB, 0,	\
			  CR3_DEF, CR4_DEF, CR5_DEF, BRG2_DEF, BRG1_DEF }

/*
 *  シリアル I/O ポートの初期化
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  MPSC の設定
	 */
	io_write(p->cntrl, MPSC_RESET);
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		mpsc_write(MPSC_CNTRLA, MPSC_CR2, 0x18);
		mpsc_write(MPSC_CNTRLB, MPSC_CR2, 0x00);
	}
	mpsc_write(p->cntrl, MPSC_CR1, CR1_RECV);
	p->sendflag = 0;
	mpsc_write(p->cntrl, MPSC_CR4, p->cr4_def);
	mpsc_write_brg(p->cntrl, MPSC_CR12, 0x01, p->brg2_def, p->brg1_def);
	mpsc_write_brg(p->cntrl, MPSC_CR12, 0x02, p->brg2_def, p->brg1_def);
	mpsc_write(p->cntrl, MPSC_CR15, CR15_DEF);
	mpsc_write(p->cntrl, MPSC_CR14, CR14_DEF);
	mpsc_write(p->cntrl, MPSC_CR10, CR10_DEF);
	mpsc_write(p->cntrl, MPSC_CR3, p->cr3_def);
	mpsc_write(p->cntrl, MPSC_CR5, p->cr5_def);

	/*
	 *  割込み関連の設定
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		dga_set_ilv(DGA_CSR25, GP0IL_BIT, IRQ_LEVEL6);
						/* 割込みレベル設定 */
		*DGA_CSR21 |= GP0_BIT;		/* 割込みマスク解除 */
	}

	*(p->initflag) = 1;			/* 初期化フラグ設定 */
	return(0);
}

/*
 *  シリアル I/O ポートのシャットダウン
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	*(p->initflag) = -1;			/* 初期化フラグ設定 */

	/*
	 *  MPSC の割込み関係の設定
	 */
	mpsc_write(p->cntrl, MPSC_CR1, CR1_DOWN);
	p->sendflag = 0;

	/*
	 *  割込み関連の設定
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		*DGA_CSR21 &= ~GP0_BIT;		/* 割込みマスク設定 */
	}
}

/*
 *  割込みハンドラのエントリ
 */
static void
int_handler_mpsc(void)
{
	if (initflag[0] > 0) {
		serial_int_handler(1);
	}
	if (initflag[1] > 0) {
		serial_int_handler(2);
	}
	mpsc_write(MPSC_CNTRLA, MPSC_CR0, MPSC_EOI);
}

/*
 *  割込みクリア処理
 */
Inline void
raw_port_clear_int(RPORT *p)
{
}

/*
 *  文字を受信したか？
 */
Inline BOOL
raw_port_getready(RPORT *p)
{
	return(mpsc_read(p->cntrl, MPSC_SR0) & 0x01);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(mpsc_read(p->cntrl, MPSC_SR0) & 0x04);
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
	if (!(p->sendflag)) {
		mpsc_write(p->cntrl, MPSC_CR1, CR1_ALL);
		p->sendflag = 1;
	}
}

Inline void
raw_port_sendstop(RPORT *p)
{
	if (p->sendflag) {
		mpsc_write(p->cntrl, MPSC_CR1, CR1_RECV);
		p->sendflag = 0;
	}
}

#endif /* _SYS_SERIAL_ */
