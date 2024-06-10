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
 *  @(#) $Id: systask.h,v 1.8 1998/07/06 14:21:02 hiro Exp $
 */

#ifndef _SYSTASK_
#define _SYSTASK_

/*
 *  ItIsシステムタスク 標準インクルードファイル
 */

/*
 *  ItIsアプリケーション標準インクルードファイル
 */
#include <itis_services.h>
#include <itis_syslog.h>

/*
 *  システム構成設定ファイル
 */
#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"

/*
 *  拡張SVC を使うかどうかの定義
 */
#ifdef NUM_SVC
#define USE_EXTENDED_SVC
#endif

/*
 *  拡張SVC 関係の定義
 */

#ifdef USE_EXTENDED_SVC

#include <svc_fncode.h>

#define DSVC(handler)	&((T_DSVC) { TA_HLNG, (FP)(handler) })

#endif /* USE_EXTENDED_SVC */

/*
 *  拡張SVC の出入口のデフォルト処理
 */
#ifndef ENTER_EXTENDED_SVC
#define ENTER_EXTENDED_SVC	((void) 0)
#endif

#ifndef EXIT_EXTENDED_SVC
#define EXIT_EXTENDED_SVC	((void) 0)
#endif

/*
 *  システムで利用する資源の定義
 */

/*
 *  初期化タスク関連の定義
 */
extern void	init_task(int);
#define TSK_INIT_CTSK \
		((T_CTSK) { 0, TA_HLNG, init_task, MIN_PRI, 8192 })

/*
 *  シリアルインタフェースドライバ関連の定義
 */
extern void	serial_startup(int portid);
extern int	serial_init(int portid);
extern int	serial_shutdown(int portid, int flush);
extern int	serial_read(int portid, char *buf, unsigned int len);
extern int	serial_write(int portid, char *buf, unsigned int len);
extern int	serial_ioctl(int portid, int req, int arg);

/*
 *  システムログタスク関連の定義
 */
extern void	logtask_startup(int portid);

extern void	log_task(int);
#define TSK_LOG_CTSK \
		((T_CTSK) { 0, TA_HLNG, log_task, 5, 8192 })

#define	MBF_LOG_BUFSZ	2048		/* ログ用メッセージバッファのサイズ */
#define	MBF_LOG_MAXMSZ	256		/* ログメッセージの最大長 */
#define MBF_LOG_CMBF \
		((T_CMBF) { 0, 0, MBF_LOG_BUFSZ, MBF_LOG_MAXMSZ })

/*
 *  初期起動タスク関連の定義
 */
extern void	first_task(int);
#define TSK_FIRST_CTSK \
		((T_CTSK) { 0, TA_HLNG, first_task, 20, 8192 })

/*
 *  BSD UNIX用 ノンブロッキングI/O 関連の定義
 */
extern void	bsd_sigio_startup(void);

extern void	bsd_sigio_task(void);
#define TSK_BSD_SIGIO_CTSK \
		((T_CTSK) { 0, TA_HLNG, bsd_sigio_task, 2, 8192 })

#define SEM_BSD_SIGIO_CSEM \
		((T_CSEM) { 0, 0, 1, 1 })

#endif /* _SYSTASK_ */
