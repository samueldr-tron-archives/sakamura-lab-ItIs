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
 *  @(#) $Id: bsd_sigio.c,v 1.4 1996/02/17 09:38:07 hiro Exp $
 */

/*
 *  BSD UNIX用 ノンブロッキングI/O サポートモジュール
 */

#include "systask.h"
#include <signal.h>

/*
 *  SIGIO 通知ハンドラ
 */
static void
bsd_sigio_handler(void)
{
	syscall(set_flg(FLG_BSD_SIGIO, 1));
	syscall(clr_flg(FLG_BSD_SIGIO, 0));
}

/*
 *  ノンブロッキングI/O モジュール起動ルーチン
 *
 *  ノンブロッキングI/O用の SIGIO 通知ハンドラを設定．SIGIO 通知イベン
 *  トフラグを生成．
 */

static int	initflag = 0;		/* 初期化済フラグ */

void
bsd_sigio_startup(void)
{
	struct sigvec	vec;

	if (!initflag) {
		vec.sv_handler = bsd_sigio_handler;
		vec.sv_mask = sigmask(SIGUSR2);
		vec.sv_flags = SV_ONSTACK|SV_INTERRUPT;
		sigvec(SIGIO, &vec, 0);

		syscall(cre_flg(FLG_BSD_SIGIO, &FLG_BSD_SIGIO_CFLG));
		initflag = 1;
	}
}
