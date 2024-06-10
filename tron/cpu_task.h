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
 *  @(#) $Id: cpu_task.h,v 1.4 1998/01/30 09:56:29 hiro Exp $
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

/*
 *  タスク起動のための定数の定義
 */
#define INI_PSW_RNG0	0x800f6000	/* PSW の初期値 (リングレベル0) */
#define INI_PSW_RNG1	0xa00f6000	/* PSW の初期値 (リングレベル1) */
#define INI_PSW_RNG2	0xc00f6000	/* PSW の初期値 (リングレベル2) */
#define INI_PSW_RNG3	0xe00f6000	/* PSW の初期値 (リングレベル3) */

#define INI_CSW		0x00070000	/* CSW の初期値 */

/*
 *  CPU依存のタスク起動処理
 *
 *  システムスタック上に，EITスタックフレームを作る．make_dormant から
 *  呼ばれる．
 */
Inline void
setup_context(TCB *tcb)
{
	VW	*ssp;

	ssp = (VW *)(tcb->isstack);	/* 初期システムスタックポインタ */
#ifndef TRON_NO_DI
	*--ssp = (VW)(tcb->task);	/* タスク起動アドレスを積む */
	*--ssp = DI14_EITINF;		/* EIT情報を積む */
#else /* TRON_NO_DI */
	*--ssp = 0;			/* EXPC (ダミー) を積む */
	*--ssp = (VW)(tcb->task);	/* タスク起動アドレスを積む */
	*--ssp = TRAPA4_EITINF;		/* EIT情報を積む */
#endif /* TRON_NO_DI */

	/*
	 *  PSW の初期値をシステムスタックに積み，ユーザスタックポイン
	 *  タを初期化する．
	 */
	switch (tcb->tskatr & TA_RNG3) {
	case TA_RNG0:
		*--ssp = INI_PSW_RNG0;
		break;
	case TA_RNG1:
		tcb->tskctxb.sp1 = tcb->istack;
		*--ssp = INI_PSW_RNG1;
		break;
	case TA_RNG2:
		tcb->tskctxb.sp2 = tcb->istack;
		*--ssp = INI_PSW_RNG2;
		break;
	case TA_RNG3:
		tcb->tskctxb.sp3 = tcb->istack;
		*--ssp = INI_PSW_RNG3;
		break;
	}
	ssp -= 15;			/* R14〜R0 の分のエリアを取る */

	tcb->tskctxb.sp0 = ssp;
	tcb->tskctxb.csw = INI_CSW;
}

/*
 *  make_dormant で壊されるスタック領域のサイズの定義
 *
 *  ext_tsk の処理中で使われる．
 */
#ifndef TRON_NO_DI
#define DORMANT_STACK_SIZE	(sizeof(VW) * 3)
#else /* TRON_NO_DI */
#define DORMANT_STACK_SIZE	(sizeof(VW) * 4)
#endif /* TRON_NO_DI */

/*
 *  タスク起動コード等の設定
 *
 *  sta_tsk の処理から呼ばれる．
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	VW	*ssp;

	ssp = tcb->tskctxb.sp0;
	*ssp = stacd;			/* 起動コードを積む (R0) */
	*(ssp+1) = (VW)(tcb->exinf);	/* タスク拡張情報を積む (R1) */
}

#endif /* _CPU_TASK_ */
