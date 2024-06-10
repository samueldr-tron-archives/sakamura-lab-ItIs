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
 *  @(#) $Id: sys_util.c,v 1.2 1998/06/18 07:44:10 hiro Exp $
 */

#include "itis_kernel.h"
#include "dve68k.h"

/*
 *  プロセッサ識別のための変数 (マルチプロセッサ対応)
 */
INT	board_id;			/* ボードID */
INT	board_addr;			/* ローカルメモリの先頭アドレス */

/*
 *  アセンブラレベルのバスエラーハンドラ
 */
extern void	afault_handler_startup(void);

/*
 *  バスエラーを捕まえるための変数
 */
VP	buserr_catch;

/*
 *  ターゲットシステム依存 初期化ルーチン
 */
void
sys_initialize(void)
{
	/*
	 *  割込み関連の初期化
	 */
	*DGA_CSR21 = 0;			/* すべての割込みをマスク */
	*DGA_CSR23 = ~0;		/* 全ての割込みをクリア */

	*DGA_CSR19 = (G0I_VEC << 24) | (G1I_VEC << 16)
				| (SWI_VEC << 8) | SPRI_VEC;
					/* 割込みベクトルの設定 */

	/*
	 *  アボート割込みの設定 (NMI)
	 */
	dga_set_ilv(DGA_CSR24, ABTIL_BIT, IRQ_NMI);
					/* 割込みレベル設定 */
	*DGA_CSR23 = ABT_BIT;		/* 割込み要求をクリア */
	*DGA_CSR21 |= ABT_BIT;		/* 割込みマスク解除 */

	/*
	 *  メモリ領域の設定
	 */
	*DGA_CSR4 = (board_addr | 0x00ff);	/* ローカルメモリ 16MB */
	*DGA_CSR5 = 0x0000012f;		/* VMEから拡張アドレスアクセス */

	/*
	 *  インタフェースレジスタ (IFR) の設定
	 */
	*DGA_CSR3 = ((board_id << 4) | 0x3);	/* ベースアドレス */
	*DGA_IFR0 = 0x80000000;			/* SQR フラグクリア */
	*DGA_IFR3 = board_id;			/* IFR3 を設定 */

	/*
	 *  ラウンドロビンモードに設定 (マルチプロセッサ対応)
	 */
	*DGA_CSR1 = (*DGA_CSR1 & 0xffeffcff)
			| (1 << 20) | ((board_id % 4) << 8);

	/*
	 *  Access Fault ハンドラの定義
	 */
	buserr_catch = (VP) 0;
	define_exc(EXCVEC_AFAULT, afault_handler_startup);
}

/*
 *  ターゲットシステムの終了ルーチン
 *
 *  ItIs のシステムを終了する時に使う．通常はモニタ呼び出しで実現する．
 */
void
sys_exit(void)
{
	dve68k_exit();
}

/*
 *  ターゲットシステムの文字出力ルーチン
 *
 *  ログタスクが動いていない時に，システムからのメッセージを出力するた
 *  めに使う．通常はモニタ呼び出しで実現する．
 */
int
sys_write(const char *buf, unsigned int len)
{
	int	i, c;

	for (i = 0; i < len; i++) {
		if ((c = *buf++) == '\n') {
			dve68k_putc('\r');
		}
		dve68k_putc(c);
	}
	return(len);
}

/*
 *  Access Fault ハンドラ (68040依存)
 *
 *  バスエラーが起こるのを捕まえたい場合は，buserr_catch にバスエラー 
 *  が起こった時の戻り番地を設定する．ただし，捕まえられるのは，リード
 *  アクセスによって起こったバスエラーのみである．
 */
void
afault_handler(VH *sp)
{
	if (buserr_catch && (*(sp+6) & 0x700)==0x100) {
		*((VW *)(sp+1)) = (VW) buserr_catch;	/* 戻り番地を変更 */
		buserr_catch = (VP) 0;
	}
	else {
		i_syslog(LOG_EMERG, "Bus error occurs.");
		i_syslog(LOG_EMERG, "CSR0-1 = %x   SP = %08x",
					*DGA_CSR0_1, sp);
		i_syslog(LOG_EMERG, "SR = %04x    PC = %08x",
					*sp, *((VW *)(sp+1)));
		i_syslog(LOG_EMERG, "SSW = %04x   FA = %08x",
					*(sp+6), *((VW *)(sp+10)));
		i_syslog(LOG_EMERG, "WB3S = %04x  WB2S = %04x  WB1S = %04x",
					*(sp+7), *(sp+8), *(sp+9));
		itis_exit();
	}
}
