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
 *  @(#) $Id: itis_kernel.h,v 1.12 1996/02/17 09:33:07 hiro Exp $
 */

#ifndef _ITIS_KERNEL_
#define _ITIS_KERNEL_

/*
 *  ItIsカーネル 標準インクルードファイル
 */

/*
 *  ItIs 標準インクルードファイル
 */
#include <itron.h>
#include <itron_errno.h>
#include <itron_fncode.h>
#include <itis_debug.h>
#include <itis_stddefs.h>
#include <itis_syslog.h>

/*
 *  カーネル用 assertマクロの定義
 */
#ifndef NDEBUG

extern void	i_assert_fail(const char *expr, const char *file,
					unsigned int line);
#define assert(exp) \
	{ if (!(exp)) { i_assert_fail(#exp, __FILE__, __LINE__); }}

#else /* NDEBUG */

#define assert(exp)

#endif /* NDEBUG */

/*
 *  カーネル構成設定ファイル
 */
#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"
#include "isysconf.h"

/*
 *  CPU依存インクルードファイル
 */
#include "cpu_status.h"

/*
 *  システムコールプロトタイプ宣言
 */
#include "isyscall.h"

/*
 *  システム初期化用関数 (各モジュール)
 */
extern void	task_initialize(void);
extern void	semaphore_initialize(void);
extern void	eventflag_initialize(void);
extern void	mailbox_initialize(void);
extern void	messagebuffer_initialize(void);
extern void	rendezvous_initialize(void);
extern void	prisem_initialize(void);
extern void	memorypool_initialize(void);
extern void	fix_memorypool_initialize(void);
extern void	cyclichandler_initialize(void);
extern void	alarmhandler_initialize(void);
extern void	extended_svc_initialize(void);

/*
 *  システム終了用関数 (startup.c)
 */
extern void	itis_exit(void);

/*
 *  システムメモリプールからのメモリブロックの取得と返却 (mempool.c)
 *
 *  システムメモリプールは，ID が TMPL_OS のメモリプールとしてアクセス
 *  することもできるが，カーネル内から用いるためのインタフェースを用意
 *  している．sys_get_blk は，メモリブロックが取れなかった場合に，待ち
 *  状態には入らずに，即座に NULL を返す．
 */
extern ER	sys_cre_mpl(void);
extern VP	sys_get_blk(INT size);
extern void	sys_rel_blk(VP blk);

/*
 *  カーネル用システムログ記録ライブラリ (isyslog.c)
 */
extern void	i_syslog(int class, const char *format, ...);

/*
 *  ターゲットシステム依存ルーチン (sys_util.c)
 */
extern void	sys_initialize(void);
extern void	sys_exit(void);
extern int	sys_write(const char *buf, unsigned int len);

/*
 *  その他の定義
 */
#define	offsetof(structure, field) ((INT) &(((structure *) 0)->field))

#define SYSCALL

#endif /* _ITIS_KERNEL_ */
