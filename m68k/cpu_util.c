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
 *  @(#) $Id: cpu_util.c,v 1.2 1998/06/18 07:45:22 hiro Exp $
 */

#include "itis_kernel.h"

/*
 *  EITハンドラの定義
 */
extern void	trap_ientry(void);
extern void	dispatch_entry(void);
extern void	z_ret_int(void);

/*
 *  ターゲットCPU依存の初期化ルーチン
 */
void
cpu_initialize(void)
{
	/*
	 *  例外ベクタテーブルの初期化
	 */
#ifdef EXCVT_ITIS
	memcpy(EXCVT_ITIS, EXCVT_ORIG, EXCVT_LEN);
	set_vbr(EXCVT_ITIS);
#endif /* EXCVT_ITIS */

	/*
	 *  例外ハンドラの定義
	 */
	define_exc(EXCVEC_TRAP1, trap_ientry);
	define_exc(EXCVEC_TRAP6, z_ret_int);
	define_exc(EXCVEC_TRAP7, dispatch_entry);
}

/*
 *  ターゲットCPU依存の終了処理ルーチン
 */
void
cpu_shutdown(void)
{
#ifdef EXCVT_ITIS
	set_vbr(EXCVT_ORIG);
#endif /* EXCVT_ITIS */
}
