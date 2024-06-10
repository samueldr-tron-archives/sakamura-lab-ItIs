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
 *  @(#) $Id: sys_util.c,v 1.4 1998/01/30 09:57:06 hiro Exp $
 */

#include "itis_kernel.h"
#include "mcube.h"

/*
 *  ターゲットシステム依存の初期化ルーチン
 */
void
sys_initialize(void)
{
	/*
	 *  IRC の初期化
	 */
	irc_assign(IRC1_IMR, 0x7e);	/* ABORT以外の全割込みをマスク */
	irc_assign(IRC1_BMR, 0x7f);
	irc_assign(IRC2_IMR, 0x7f);

	irc_assign(IRC1_VMR, 0x7f);	/* オートベクタモードに設定 */
	irc_assign(IRC1_VNR, IRC2_VECTOR_BASE);
	irc_assign(IRC2_VMR, 0x00);	/* IRCベクタモードに設定 */
	irc_assign(IRC2_VSR, 0x00);
	irc_assign(IRC2_VNR, IRC2_VECTOR_BASE);
}

/*
 *  ターゲットシステムの終了ルーチン
 *
 *  ItIs のシステムを終了する時に使う．通常はモニタ呼び出しで実現する．
 */
void
sys_exit(void)
{
	Asm("ldc #0, %0" : : "m"(CREG(PSW)));
	mcube_exit();
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
	int	i;

	for (i = 0; i < len; i++) {
		mcube_putc(*buf++);
	}
	return(len);
}
