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
 *  @(#) $Id: sys_conf.h,v 1.2 1998/06/18 07:44:07 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  ItIs 動作時のメモリマップ
 *
 *	00000000 - 0000ffff	ROMモニタワークエリア (64KB)
 *	00010000 -		カーネルコード領域
 *	00080000 -		ページテーブル領域など (512KB)
 *		 - 000fffff	タスク独立部用スタック領域
 *	00100000 - 002fffff	カーネルデータ領域 (2MB)
 *	00300000 - 003fffff	ユーザプログラム領域 (1MB)
 */

/*
 *  スタックエリア，ヒープエリアの定義
 *
 *  _end は，プログラムで使用するデータエリアの最後の次の番地．
 */
#define STACKTOP	0x00100000	/* タスク独立部用スタックの初期値 */
#define HEAPTOP		_end		/* ヒープエリアの先頭 */
#define HEAPLIMIT	0x00300000	/* ヒープエリアの上限 */

/*
 *  システムタスクに関する定義
 */
#define	CONSOLE_PORT	2	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORT	2	/* システムログを出力するシリアルポート番号 */

/*
 *  システムタスクが使用するライブラリに関する定義
 */
#define NEWLIB			/* newlib を用いる */

#endif /* _SYS_CONF_ */
