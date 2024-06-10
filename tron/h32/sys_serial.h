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
 *  @(#) $Id: sys_serial.h,v 1.6 1998/01/30 09:56:43 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  H32SBCシステム CPUボード用 低レベルシリアルI/O 関連の定義
 */

#include "h32sbc.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct raw_serial_port_descripter {
	IOREG	*data;		/* ACIデータレジスタの番地 */
	IOREG	*status;	/* ACIステータスレジスタの番地 */
	IOREG	*mode;		/* ACIモードレジスタの番地 */
	IOREG	*command;	/* ACIコマンドレジスタの番地 */

	byte	def_mode1;	/* デフォールトのモード設定値(1) */
	byte	def_mode2;	/* デフォールトのモード設定値(2) */

	byte	irc_bit;	/* IRC設定のビットパターン */
	byte	int_level;	/* 割込みレベル */
	FP	int_handler;	/* 割込みハンドラ */
} RPORT;

/*
 *  ACIコマンドレジスタの設定値
 */
#define	COM_STOP	0x32	/* 送受信とも停止 */
#define	COM_SSTOP	0x36	/* 送信停止，受信はイネーブル */
#define	COM_START	0x37	/* 送受信ともイネーブル */

/*
 *  ACIモードレジスタのデフォルト設定値
 */
#define	DEF_MODE1	0x4d	/* 8bit, ストップビット 1bit, パリティなし */
#define	DEF_MODE2	0x3e	/* 9600bps */

/*
 *  割込みベクタとハンドラアドレスの取り出し
 */
#define	raw_int_vector(p)	INT_VECTOR((p)->int_level)
#define	raw_int_handler(p)	((p)->int_handler)

/*
 *  割込みハンドラのエントリ (前方宣言)
 */
static void	int_handler_aci1(void);
static void	int_handler_aci0(void);

static void	serial_int_handler(int portid);

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	2	/* サポートするシリアルポートの数 */

#define	RAWPORT1	{ ACI0_DATA, ACI0_STATUS, ACI0_MODE, ACI0_COMMAND, \
			  DEF_MODE1, DEF_MODE2,				   \
			  LIR2_BIT, 4, int_handler_aci0 }
#define	RAWPORT2	{ ACI1_DATA, ACI1_STATUS, ACI1_MODE, ACI1_COMMAND, \
			  DEF_MODE1, DEF_MODE2,				   \
			  LIR3_BIT, 3, int_handler_aci1 }

/*
 *  シリアル I/O ポートの初期化
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  ACI の設定
	 */
	*(p->command) = COM_STOP;
	n = *(p->command);		/* ダミーリード */
	*(p->mode) = p->def_mode1;
	*(p->mode) = p->def_mode2;
	*(p->command) = COM_SSTOP;	/* 受信のみイネーブル */

	/*
	 *  IRC の設定
	 */
	irc_assign(IRC_LMR(p->int_level), p->irc_bit);
	irc_or_assign(IRC_VMR, p->irc_bit);
	irc_and_assign(IRC_TMR, ~(p->irc_bit));	/* レベルトリガモード */
	irc_and_assign(IRC_IMR, ~(p->irc_bit));	/* 割込みマスク解除 */

	if ((n = irc_read(IRC_TMR)) & p->irc_bit) {
		/*
		 *  IRC の初期化に失敗
		 *
		 *  TMR (トリガモードレジスタ) の読み出しに失敗し，その
		 *  結果正しく割込みがかからなくあるという障害が出るこ
		 *  とが多い．この障害はプログラムの置かれる位置に依存
		 *  して起こり，再現性もある．原因は不明．
		 */
		syslog(LOG_EMERG,
			"Serial port initialization error (%02x).", n);
		return(1);
	}
	return(0);
}

/*
 *  シリアル I/O ポートのシャットダウン
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	irc_or_assign(IRC_IMR, p->irc_bit);	/* 割込みをマスク */
}

/*
 *  割込みハンドラのエントリ
 */
static void
int_handler_aci0(void)
{
	int	intmask;

	irc_assign(IRC_IRR, LIR2_BIT);	/* 割込み要求をクリア(本来は不要) */
	serial_int_handler(1);
}

static void
int_handler_aci1(void)
{
	irc_assign(IRC_IRR, LIR3_BIT);	/* 割込み要求をクリア(本来は不要) */
	serial_int_handler(2);
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
	return(*(p->status) & 0x02);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(*(p->status) & 0x01);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
raw_port_getchar(RPORT *p)
{
	return(*(p->data));
}

/*
 *  送信する文字の書き込み
 */
Inline void
raw_port_putchar(RPORT *p, byte c)
{
	*(p->data) = c;
}

/*
 *  送信制御関数
 */
Inline void
raw_port_sendstart(RPORT *p)
{
	*(p->command) = COM_START;
}

Inline void
raw_port_sendstop(RPORT *p)
{
	*(p->command) = COM_SSTOP;
}

/*
 *  IRC と ACI の設定状態を表示する (デバッグ用)．
 */
#ifdef DEBUG

Inline void
raw_port_stat(RPORT *p)
{
	syslog(LOG_NOTICE, "IRC_VMR = %x", irc_read(IRC_VMR));
	syslog(LOG_NOTICE, "IRC_TMR = %x", irc_read(IRC_TMR));
	syslog(LOG_NOTICE, "IRC_IMR = %x", irc_read(IRC_IMR));
	syslog(LOG_NOTICE, "IRC_IRR = %x", irc_read(IRC_IRR));
	syslog(LOG_NOTICE, "ACI_COMMAND = %x", *(p->command));
	syslog(LOG_NOTICE, "ACI_MODE1 = %x", *(p->mode));
	syslog(LOG_NOTICE, "ACI_MODE2 = %x", *(p->mode));
	syslog(LOG_NOTICE, "ACI_STATUS = %x", *(p->status));
	syslog(LOG_NOTICE, "ACI_DATA = %x", *(p->data));
}

#endif DEBUG

#endif /* _SYS_SERIAL_ */
