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
 *  @(#) $Id: cpu_conf.h,v 1.7 1998/06/02 10:21:52 hiro Exp $
 */

#ifndef _CPU_CONF_
#define _CPU_CONF_

/*
 *  get_ver で参照される CPUコードの定義
 */
#define	CPU_CODE	0x0001		/* TRON仕様チップ一般 */

/*
 *  CPU依存のシステムコール名/パラメータ名の設定
 */
#define chg_iXX	chg_ims
#define ref_iXX	ref_ims
#define iXXXX	imask
#define p_iXXXX	p_imask

/*
 *  TRON仕様チップではサポートされない機能の定義
 */
#define _i_dis_int	_no_support	/* dis_intシステムコール */
#define _i_ena_int	_no_support	/* ena_intシステムコール */

/*
 *  タスク属性中の実行モードに関する定義
 */
#define TA_MODEMASK	(TA_RNG3)
#define SYSMODE(atr)	(((atr) & TA_MODEMASK) == TA_RNG0)

/*
 *  システムスタックとユーザスタックを別々に持つ
 */
#define	USE_SEPARATE_SSTACK

/*
 *  dispatch_disabled は，遅延割込みを使わない場合にのみ必要
 */
#ifdef TRON_NO_DI
#define USE_DISPATCH_DISABLED
#endif /* TRON_NO_DI */

/*
 *  システムスタックの最小サイズの定義
 *
 *  cre_tsk でタスク毎にシステムスタックサイズを設定する場合の最小のシ
 *  ステムスタックサイズ．
 */
#ifndef MIN_SYS_STACK_SIZE
#define	MIN_SYS_STACK_SIZE	256
#endif /* MIN_SYS_STACK_SIZE */

/*
 *  デフォルトシステムスタックサイズの定義
 *
 *  cre_tsk でタスク毎にシステムスタックサイズを設定しない場合のデフォ
 *  ルトのシステムスタックサイズ．
 */
#ifndef DEF_SYS_STACK_SIZE
#define DEF_SYS_STACK_SIZE	1024
#endif /* DEF_SYS_STACK_SIZE */

#endif /* _CPU_CONF_ */
