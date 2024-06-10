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
 *  @(#) $Id: eventflag.c,v 1.10 1997/01/10 13:36:25 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_FLG

/*
 *  イベントフラグ管理ブロックの定義
 */
typedef struct eventflag_control_block {
	QUEUE	wait_queue;	/* イベントフラグ待ちキュー */
	ID	flgid;		/* イベントフラグID */
	VP	exinf;		/* 拡張情報 */
	ATR	flgatr;		/* イベントフラグ属性 */
	UINT	flgptn;		/* イベントフラグ現在パターン */
} FLGCB;

static FLGCB	flgcb_table[NUM_FLGID];

#define get_flgcb(id)	(&(flgcb_table[INDEX_FLG(id)]))
    
/*
 *  未使用のイベントフラグ管管理ブロックのリスト
 */
#ifndef _i_vcre_flg
QUEUE	free_flgcb;
#endif /* _i_vcre_flg */

/* 
 *  イベントフラグ管理ブロックの初期化
 */
void
eventflag_initialize(void)
{
	INT	i;
	FLGCB	*flgcb;
	ID	flgid;

#ifndef _i_vcre_flg
	queue_initialize(&free_flgcb);
#endif /* _i_vcre_flg */

	for(flgcb = flgcb_table, i = 0; i < NUM_FLGID; flgcb++, i++) {
		flgid = ID_FLG(i);
		flgcb->flgid = flgid;
		flgcb->flgatr = OBJ_NONEXIST;
#ifndef _i_vcre_flg
		if (!SYS_FLGID(flgid)) {
			queue_insert(&(flgcb->wait_queue), &free_flgcb);
		}
#endif /* _i_vcre_flg */
	}
}

/*
 *  イベントフラグ待ち解除条件のチェック
 */
Inline BOOL
eventflag_cond(FLGCB *flgcb, UINT waiptn, UINT wfmode)
{
	if (wfmode & TWF_ORW) {
		return(flgcb->flgptn & waiptn);
	}
	else {
		return((flgcb->flgptn & waiptn) == waiptn);
	}
}

/*
 *  イベントフラグ待ち仕様の定義
 */
static WSPEC wspec_flg = { TTW_FLG, 0, 0 };

/*
 *  イベントフラグ管理機能
 */

#if !defined(_i_cre_flg) || !defined(_i_vcre_flg)

static void
_cre_flg(FLGCB *flgcb, T_CFLG *pk_cflg)
{
#ifndef _i_vcre_flg
	if (!SYS_FLGID(flgcb->flgid)) {
		queue_delete(&(flgcb->wait_queue));
	}
#endif /* _i_vcre_flg */

	queue_initialize(&(flgcb->wait_queue));
	flgcb->exinf = pk_cflg->exinf;
	flgcb->flgatr = pk_cflg->flgatr;
	flgcb->flgptn = pk_cflg->iflgptn;
}

#endif /* !defined(_i_cre_flg) || !defined(_i_vcre_flg) */
#ifndef _i_cre_flg

SYSCALL ER
i_cre_flg(ID flgid, T_CFLG *pk_cflg)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_FLGACV(flgid);
	CHECK_RSATR(pk_cflg->flgatr, TA_WMUL);
	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if (flgcb->flgatr != OBJ_NONEXIST) {
		ercd =  E_OBJ;
	}
	else {
		_cre_flg(flgcb, pk_cflg);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_flg */
#ifndef _i_vcre_flg

SYSCALL ER
i_vcre_flg(T_CFLG *pk_cflg)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cflg->flgatr, TA_WMUL);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_flgcb)) {
		ercd =  EV_FULL;
	}
	else {
		flgcb = (FLGCB *)(free_flgcb.next);
		_cre_flg(flgcb, pk_cflg);
		ercd = (ER)(flgcb->flgid);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_flg */
#ifndef _i_del_flg

SYSCALL ER
i_del_flg(ID flgid)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_FLGACV(flgid);
	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if (flgcb->flgatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		wait_delete(&(flgcb->wait_queue));
#ifndef _i_vcre_flg
		if (!SYS_FLGID(flgcb->flgid)) {
			queue_insert(&(flgcb->wait_queue), &free_flgcb);
		}
#endif /* _i_vcre_flg */
		flgcb->flgatr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_flg */
#ifndef _i_set_flg

SYSCALL ER
i_set_flg(ID flgid, UINT setptn)
{
	FLGCB	*flgcb;
	TCB	*tcb;
	QUEUE	*queue;
	UINT	wfmode;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_FLGACV(flgid);
	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if (flgcb->flgatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	flgcb->flgptn |= setptn;
    
	queue = flgcb->wait_queue.next;
	while (queue != &(flgcb->wait_queue)) {
		tcb = (TCB *) queue;
		queue = queue->next;
		wfmode = tcb->winfo.flg.wfmode;
		if (eventflag_cond(flgcb, tcb->winfo.flg.waiptn, wfmode)) {
			*(tcb->winfo.flg.p_flgptn) = flgcb->flgptn;
			wait_release_ok(tcb);
			if (wfmode & TWF_CLR) {
				flgcb->flgptn = 0;
				break;
			}
		}
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_set_flg */
#ifndef _i_clr_flg

SYSCALL ER
i_clr_flg(ID flgid, UINT clrptn)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_FLGACV(flgid);
	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if (flgcb->flgatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		flgcb->flgptn &= clrptn; 
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_clr_flg */
#ifndef _i_wai_flg

SYSCALL ER
i_wai_flg(UINT* p_flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
	return(i_twai_flg(p_flgptn, flgid, waiptn, wfmode, TMO_FEVR));
}

#endif /* _i_wai_flg */
#ifndef _i_pol_flg

SYSCALL ER
i_pol_flg(UINT* p_flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
	return(i_twai_flg(p_flgptn, flgid, waiptn, wfmode, TMO_POL));
}

#endif /* _i_pol_flg */
#if !defined(_i_wai_flg) || !defined(_i_pol_flg) || !defined(_i_twai_flg)

SYSCALL ER
i_twai_flg(UINT* p_flgptn, ID flgid, UINT waiptn, UINT wfmode, TMO tmout)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_FLGACV(flgid);
	CHECK_PAR(waiptn != 0);
	CHECK_PAR((wfmode & ~(TWF_ORW|TWF_CLR)) == 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if (flgcb->flgatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!(flgcb->flgatr & TA_WMUL)
			&& !queue_empty_p(&(flgcb->wait_queue))) {
		ercd = E_OBJ;
	}
	else if (eventflag_cond(flgcb, waiptn, wfmode)) {
		*p_flgptn = flgcb->flgptn;
		if (wfmode & TWF_CLR) {
			flgcb->flgptn = 0;
		}
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = &wspec_flg;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.flg.waiptn = waiptn;
			ctxtsk->winfo.flg.wfmode = wfmode;
			ctxtsk->winfo.flg.p_flgptn = p_flgptn;
			gcb_make_wait((GCB *) flgcb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_twai_flg */
#ifndef _i_ref_flg

SYSCALL ER
i_ref_flg(T_RFLG *pk_rflg, ID flgid)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_FLGACV(flgid);
	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if (flgcb->flgatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rflg->exinf = flgcb->exinf;
		pk_rflg->wtsk = wait_tskid(&(flgcb->wait_queue));
		pk_rflg->flgptn = flgcb->flgptn;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_flg */
#endif /* USE_FLG */
