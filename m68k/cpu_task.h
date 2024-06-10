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
 *  @(#) $Id: cpu_task.h,v 1.4 1998/07/06 10:48:14 hiro Exp $
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

/*
 *  タスク起動のための定数の定義
 */
#define INI_SR_SMODE	0x3000		/* SR の初期値 (スーパバイザモード) */
#define INI_SR_UMODE	0x1000		/* SR の初期値 (ユーザモード) */

/*
 *  CPU依存のタスク起動処理
 *
 *  システムスタック上に，例外スタックフレームを作る．make_dormant から
 *  呼ばれる．
 */
Inline void
setup_context(TCB *tcb)
{
	VW	*ssp, *usp;

	ssp = (VW *)(tcb->isstack);	/* 初期システムスタックポインタ */

	if (SYSMODE(tcb->tskatr) && (tcb->tskatr & TA_HLNG)) {
		ssp -= 3;		/* 起動コードのためのエリアを取る */
	}
	*--((VH *) ssp) = 0;		/* 例外情報を積む */
	*--ssp = (VW)(tcb->task);	/* タスク起動アドレスを積む */

	/*
	 *  SR の初期値をシステムスタックに積み，ユーザスタックポイン
	 *  タを初期化する．
	 */
	if (!SYSMODE(tcb->tskatr)) {
		usp = tcb->istack;
		if (tcb->tskatr & TA_HLNG) {
			usp -= 3;	/* 起動コードのためのエリアを取る */
		}
		tcb->tskctxb.usp = usp;
		*--((VH *) ssp) = INI_SR_UMODE;
	}
	else {
		*--((VH *) ssp) = INI_SR_SMODE;
	}
	ssp -= 15;			/* D0〜D7/A0〜A6 の分のエリアを取る */

	tcb->tskctxb.msp = ssp;
}

/*
 *  make_dormant で壊されるスタック領域のサイズの定義
 *
 *  ext_tsk の処理中で使われる．
 */
#define DORMANT_STACK_SIZE	(sizeof(VH) * 2 + sizeof(VW))

/*
 *  タスク起動コード等の設定
 *
 *  sta_tsk の処理から呼ばれる．
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	VW	*sp;

	if (!(tcb->tskatr & TA_HLNG)) {
		sp = tcb->tskctxb.msp;
		*sp = stacd;			/* 起動コードを積む (D0) */
		*(sp+1) = (VW)(tcb->exinf);	/* タスク拡張情報を積む (D1) */
	}
	else {
		sp = (SYSMODE(tcb->tskatr)) ? (VW *)(tcb->isstack)
					    : (VW *)(tcb->istack);
		*(sp-3) = (VW) NADR;		/* 戻り番地を積む */
		*(sp-2) = stacd;		/* 起動コードを積む */
		*(sp-1) = (VW)(tcb->exinf);	/* タスク拡張情報を積む */
	}
}

#endif /* _CPU_TASK_ */
