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
 *  @(#) $Id: cpu_conf.h,v 1.9 1998/01/30 06:59:08 hiro Exp $
 */

#ifndef _CPU_CONF_
#define _CPU_CONF_

/*
 *  get_ver で参照される CPUコードの定義
 */
#define	CPU_CODE	0x0000		/* CPU を特定しない */

/*
 *  CPU依存のシステムコール名/パラメータ名の設定
 */
#define chg_iXX	chg_ims
#define ref_iXX	ref_ims
#define iXXXX	imask
#define p_iXXXX	p_imask

/*
 *  BSD UNIX上ではサポートされない機能の定義
 */
#undef NUM_SVC				/* 拡張SVCハンドラ */
#undef USE_QTSK_PORTION			/* 準タスク部 */
#undef USE_VGET_TIM			/* 性能評価用システム時刻参照機能 */

#define _i_dis_int	_no_support	/* dis_intシステムコール */
#define _i_ena_int	_no_support	/* ena_intシステムコール */
#define _i_def_int	_no_support	/* def_intシステムコール */
#define _i_set_reg	_no_support	/* set_regシステムコール */
#define _i_get_reg	_no_support	/* get_regシステムコール */

/*
 *  システムスタックの最小サイズの定義
 *
 *  cre_tsk でタスク毎にシステムスタックサイズを設定する場合の最初のシ
 *  ステムスタックサイズ．
 */
#ifndef MIN_SYS_STACK_SIZE
#define	MIN_SYS_STACK_SIZE	8192
#endif /* MIN_SYS_STACK_SIZE */

/*
 *  デフォルトシステムスタックサイズの定義
 *
 *  cre_tsk でタスク毎にシステムスタックサイズを設定しない場合のデフォ
 *  ルトのシステムスタックサイズ．
 */
#ifndef DEF_SYS_STACK_SIZE
#define DEF_SYS_STACK_SIZE	8192
#endif /* DEF_SYS_STACK_SIZE */

/*
 *  拡張SVC の出入口の処理
 */
#define ENTER_EXTENDED_SVC	enter_extended_svc()
#define EXIT_EXTENDED_SVC	exit_extended_svc()

/*
 *  システムタスクに関する定義
 */
#define	CONSOLE_PORT	1	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORT	1	/* システムログを出力するシリアルポート番号 */

#endif /* _CPU_CONF_ */
