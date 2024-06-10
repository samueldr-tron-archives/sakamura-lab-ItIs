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
 *  @(#) $Id: sys_conf.h,v 1.6 1998/01/30 09:57:23 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  ItIs 動作時のメモリマップ
 *
 *	00000000 -		カーネルコード領域
 *		 - 00001000	ROMモニタの初期SPI (数バイト壊される)
 *	00040000 - 0007ffff	ユーザプログラム領域 (256KB)
 *	00080000 - 000fffff	カーネルデータ領域 (512KB)
 *	fff20000 - fff27fff	モニタワーク領域 (32KB)
 *	fff28000 - fff37fff	未使用 (32KB, 一部ダイレクト起動用のスタック)
 *		 - fff3ffff	タスク独立部用スタック領域
 *	fffb0000 - fffbffff	ローカルI/O領域
 *	fffc0000 - fffc007f	メイルボックス領域
 */

/*
 *  スタックエリア，ヒープエリアの定義
 *
 *  _end は，プログラムで使用するデータエリアの最後の次の番地．
 */
#define STACKTOP	0xfff40000	/* タスク独立部用スタックの初期値 */
#define HEAPTOP		_end		/* ヒープエリアの先頭 */
#define HEAPLIMIT	0x00100000	/* ヒープエリアの上限 */

/*
 *  システムタスクに関する定義
 */
#define	CONSOLE_PORT	1	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORT	1	/* システムログを出力するシリアルポート番号 */

#endif /* _SYS_CONF_ */
