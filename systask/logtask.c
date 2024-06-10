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
 *  @(#) $Id: logtask.c,v 1.9 1996/02/17 09:38:13 hiro Exp $
 */

/*
 *  システムログタスク
 */

#include <stdarg.h>
#include "systask.h"

/*
 *  ログタスクの変数
 */
int	logtask_alive = 0;	/* ログタスクが動いているか */
int	log_msg_maxmsz;		/* ログメッセージの最大長 */

/*
 *  前方参照宣言
 */
ER	svc_syslog_send(const char *string, int len);

/*
 *  ログタスクの起動と初期化，拡張SVCの定義
 */
void
logtask_startup(int portid)
{
	syscall(serial_init(portid));
	syscall(cre_mbf(TMBF_OS, &MBF_LOG_CMBF));
	syscall(cre_tsk(TSK_LOG, &TSK_LOG_CTSK));
	syscall(sta_tsk(TSK_LOG, portid));

#ifdef USE_EXTENDED_SVC
	syscall(def_svc(SVC_SYSLOG_SEND, DSVC(svc_syslog_send)));
#endif

	log_msg_maxmsz = MBF_LOG_MAXMSZ;
	logtask_alive = 1;

	syslog(LOG_NOTICE, "System logging task started on port %d.", portid);
}

/*
 *  ログタスクの本体
 */

static int	logtask_portid;			/* ログ出力ポート番号 */
static char	logtask_buf[MBF_LOG_MAXMSZ+1];	/* ログタスク用バッファ */

void
log_task(int portid)
{
	INT	msgsz;
	ER	ercd;

	logtask_portid = portid;
	while ((ercd = rcv_mbf(logtask_buf, &msgsz, TMBF_OS)) == E_OK) {
		logtask_buf[msgsz++] = '\n';
		serial_write(logtask_portid, logtask_buf, msgsz);
	}
	logtask_alive = 0;
	perror("logtask", ercd);
	ext_tsk();
}

/*
 *  拡張SVCハンドラ本体
 *
 *  ログタスクが動いている場合は，ログメッセージバッファへ送る．動いて
 *  いない場合は，直接低レベルの文字出力ルーチンを使って出力する．
 */

ER
svc_syslog_send(const char *string, int len)
{
	ENTER_EXTENDED_SVC;
	if (len > 0) {
		if (logtask_alive) {
			if (len > log_msg_maxmsz) {
				len = log_msg_maxmsz;
			}
			if (psnd_mbf(TMBF_OS, (VP) string, len) != E_OK) {
				logtask_alive = 0;
			}
		}
		if (!logtask_alive) {
			sys_write(string, len);
			sys_write("\n", 1);
		}
	}
	EXIT_EXTENDED_SVC;
	return(E_OK);
}
