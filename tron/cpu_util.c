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
 *  @(#) $Id: cpu_util.c,v 1.3 1996/02/17 09:43:19 hiro Exp $
 */

#include "itis_kernel.h"

/*
 *  EITハンドラの定義
 */
extern void	trapa_ientry(void);
#ifndef TRON_NO_DI
extern void	dispatch_entry(void);
#else /* TRON_NO_DI */
extern void	dispatch_entry(void);
extern void	z_ret_int(void);
#endif /* TRON_NO_DI */

/*
 *  ターゲットCPU依存の初期化ルーチン
 */
void
cpu_initialize(void)
{
	/*
	 *  EITベクタテーブルの初期化
	 */
#ifdef EITVB_ITIS
	memcpy(EITVB_ITIS, EITVB_ORIG, EITVT_LEN);
	set_eitvb(EITVB_ITIS);
#endif /* EITVB_ITIS */

	/*
	 *  EITハンドラの定義
	 */
	define_eit(EITVEC_TRAPA1, EITATR(1, 15), trapa_ientry);
#ifndef TRON_NO_DI
	define_eit(EITVEC_DI14, EITATR(1, 14), dispatch_entry);
#else /* TRON_NO_DI */
	define_eit(EITVEC_TRAPA3, EITATR(0, 15), z_ret_int);
	define_eit(EITVEC_TRAPA4, EITATR(1, 15), dispatch_entry);
#endif /* TRON_NO_DI */
}

/*
 *  ターゲットCPU依存の終了処理ルーチン
 */
void
cpu_shutdown(void)
{
#ifdef EITVB_ITIS
	set_eitvb(EITVB_ORIG);
#endif /* EITVB_ITIS */
}
