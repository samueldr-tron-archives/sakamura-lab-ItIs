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
 *  @(#) $Id: rendezvous.c,v 1.23 1998/01/30 09:52:41 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_POR

/*
 *  ランデブ用ポート管理ブロックの定義
 */

typedef struct port_control_block {
	QUEUE	call_queue;	/* ポート呼出待ちキュー */
	ID	porid;		/* ポートID */
	VP	exinf;		/* 拡張情報 */
	ATR	poratr;		/* ポート属性 */
	QUEUE	accept_queue;	/* ポート受付待ちキュー */
	INT	maxcmsz;	/* 呼出メッセージの最大長 */
	INT	maxrmsz;	/* 返答メッセージの最大長 */
} PORCB;

static PORCB	porcb_table[NUM_PORID];

#define get_porcb(id)	(&(porcb_table[INDEX_POR(id)]))

/*
 *  未使用のポート管理ブロックのリスト
 */
#ifndef _i_vcre_por
QUEUE	free_porcb;
#endif /* _i_vcre_por */

/* 
 *  ポート管理ブロックの初期化
 */
void
rendezvous_initialize()
{
	INT	i;
	PORCB	*porcb;
	ID	porid;

#ifndef _i_vcre_por
	queue_initialize(&free_porcb);
#endif /* _i_vcre_por */

	for(porcb = porcb_table, i = 0; i < NUM_PORID; porcb++, i++) {
		porid = ID_POR(i);
		porcb->porid = porid;
		porcb->poratr = OBJ_NONEXIST;
#ifndef _i_vcre_por
		if (!SYS_PORID(porid)) {
			queue_insert(&(porcb->call_queue), &free_porcb);
		}
#endif /* _i_vcre_por */
	}
}

/*
 *  ランデブ番号管理ルーチン
 */
#define RDVNO_SHIFT	16

Inline RNO
gen_rdvno(TCB *tcb)
{
	RNO	rdvno;

	rdvno = tcb->wrdvno;
	tcb->wrdvno += (1 << RDVNO_SHIFT);
	return(rdvno);
}

Inline ID
get_tskid_rdvno(RNO rdvno)
{
	return((W)(H)(rdvno & ((1 << RDVNO_SHIFT) - 1)));
}

#define CHECK_RDVNO(rdvno) {					\
	if (!CHK_TSKID(get_tskid_rdvno(rdvno))) {		\
		return(E_OBJ);					\
	}							\
}

/*
 *  ランデブ待ち仕様の定義
 */
static WSPEC wspec_cal = { TTW_CAL, 0, 0 };
static WSPEC wspec_acp = { TTW_ACP, 0, 0 };
static WSPEC wspec_rdv = { TTW_RDV, 0, 0 };

/*
 *  ランデブ用ポート管理機能
 */

#if !defined(_i_cre_por) || !defined(_i_vcre_por)

static void
_cre_por(PORCB *porcb, T_CPOR *pk_cpor)
{
#ifndef _i_vcre_por
	if (!SYS_PORID(porcb->porid)) {
		queue_delete(&(porcb->call_queue));
	}
#endif /* _i_vcre_por */

	queue_initialize(&porcb->call_queue);
	porcb->exinf = pk_cpor->exinf;
	porcb->poratr = pk_cpor->poratr;
	queue_initialize(&porcb->accept_queue);
	porcb->maxcmsz = pk_cpor->maxcmsz;
	porcb->maxrmsz = pk_cpor->maxrmsz;
}

#endif /* !defined(_i_cre_por) || !defined(_i_vcre_por) */
#ifndef _i_cre_por

SYSCALL ER
i_cre_por(ID porid, T_CPOR *pk_cpor)
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PORACV(porid);
	CHECK_RSATR(pk_cpor->poratr, 0);
	CHECK_PAR(pk_cpor->maxcmsz > 0);
	CHECK_PAR(pk_cpor->maxrmsz > 0);
	CHECK_INTSK();
	porcb = get_porcb(porid);

	BEGIN_CRITICAL_SECTION;
	if (porcb->poratr != OBJ_NONEXIST) {
		ercd =  E_OBJ;
	}
	else {
		_cre_por(porcb, pk_cpor);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_por */
#ifndef _i_vcre_por

SYSCALL ER
i_vcre_por(T_CPOR *pk_cpor)
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cpor->poratr, 0);
	CHECK_PAR(pk_cpor->maxcmsz > 0);
	CHECK_PAR(pk_cpor->maxrmsz > 0);
	CHECK_INTSK();

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_porcb)) {
		ercd =  EV_FULL;
	}
	else {
		porcb = (PORCB *)(free_porcb.next);
		_cre_por(porcb, pk_cpor);
		ercd = (ER)(porcb->porid);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_por */
#ifndef _i_del_por

SYSCALL ER
i_del_por(ID porid)
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PORACV(porid);
	CHECK_INTSK();
	porcb = get_porcb(porid);
    
	BEGIN_CRITICAL_SECTION;
	if (porcb->poratr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		wait_delete(&(porcb->call_queue));
		wait_delete(&(porcb->accept_queue));
#ifndef _i_vcre_por
		if (!SYS_PORID(porcb->porid)) {
			queue_insert(&(porcb->call_queue), &free_porcb);
		}
#endif /* _i_vcre_por */
		porcb->poratr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_por */
#ifndef _i_cal_por

SYSCALL ER
i_cal_por(VP msg, INT *p_rmsgsz, ID porid, UW calptn, INT cmsgsz)
{
	return(i_tcal_por(msg, p_rmsgsz, porid, calptn, cmsgsz, TMO_FEVR));
}

#endif /* _i_cal_por */
#ifndef _i_pcal_por

SYSCALL ER
i_pcal_por(VP msg, INT *p_rmsgsz, ID porid, UW calptn, INT cmsgsz)
{
	return(i_tcal_por(msg, p_rmsgsz, porid, calptn, cmsgsz, TMO_POL));
}

#endif /* _i_pcal_por */
#if !defined(_i_cal_por) || !defined(_i_pcal_por) || !defined(_i_tcal_por)

SYSCALL ER
i_tcal_por(VP msg, INT *p_rmsgsz, ID porid, UW calptn, INT cmsgsz, TMO tmout)
{
	PORCB	*porcb;
	TCB	*tcb;
	QUEUE	*queue;
	RNO	rdvno;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PORACV(porid);
	CHECK_PAR(calptn != 0);
	CHECK_PAR(cmsgsz > 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	porcb = get_porcb(porid);
    
	BEGIN_CRITICAL_SECTION;
	if (porcb->poratr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#ifdef CHK_PAR
	if (cmsgsz > porcb->maxcmsz) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif /* CHK_PAR */

	queue = porcb->accept_queue.next;
	while (queue != &(porcb->accept_queue)) {
		tcb = (TCB *) queue;
		queue = queue->next;
		if (calptn & tcb->winfo.acp.acpptn) {
			rdvno = gen_rdvno(ctxtsk);
			memcpy(tcb->winfo.acp.msg, msg, cmsgsz);
			*(tcb->winfo.acp.p_rdvno) = rdvno;
			*(tcb->winfo.acp.p_cmsgsz) = cmsgsz;
			wait_release_ok(tcb);

			ctxtsk->wspec = &wspec_rdv;
			ctxtsk->wgcb = (GCB *) 0;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.rdv.rdvno = rdvno;
			ctxtsk->winfo.rdv.msg = msg;
			ctxtsk->winfo.rdv.maxrmsz = porcb->maxrmsz;
			ctxtsk->winfo.rdv.p_rmsgsz = p_rmsgsz;
			make_wait(tmout);
			queue_initialize(&(ctxtsk->tskque));
			goto error_exit;
		}
	}
	ercd = E_TMOUT;
	if (tmout != TMO_POL) {
		ctxtsk->wspec = &wspec_cal;
		ctxtsk->wgcb = (GCB *) porcb;
		ctxtsk->wercd = &ercd;
		ctxtsk->winfo.cal.calptn = calptn;
		ctxtsk->winfo.cal.msg = msg;
		ctxtsk->winfo.cal.cmsgsz = cmsgsz;
		ctxtsk->winfo.cal.p_rmsgsz = p_rmsgsz;
		make_wait(tmout);
		queue_insert(&(ctxtsk->tskque), &(porcb->call_queue));
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_tcal_por */
#ifndef _i_acp_por

SYSCALL ER
i_acp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn)
{
	return(i_tacp_por(p_rdvno, msg, p_cmsgsz, porid, acpptn, TMO_FEVR));
}

#endif /* _i_acp_por */
#ifndef _i_pacp_por

SYSCALL ER
i_pacp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn)
{
	return(i_tacp_por(p_rdvno, msg, p_cmsgsz, porid, acpptn, TMO_POL));
}

#endif /* _i_pacp_por */
#if !defined(_i_acp_por) || !defined(_i_pacp_por) || !defined(_i_tacp_por)

SYSCALL ER
i_tacp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn,
	   TMO tmout)
{
	PORCB	*porcb;
	TCB	*tcb;
	QUEUE	*queue;
	RNO	rdvno;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PORACV(porid);
	CHECK_PAR(acpptn != 0);
	CHECK_DISPATCH();
	porcb = get_porcb(porid);
    
	BEGIN_CRITICAL_SECTION;
	if (porcb->poratr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	queue = porcb->call_queue.next;
	while (queue != &(porcb->call_queue)) {
		tcb = (TCB *) queue;
		queue = queue->next;
		if (acpptn & tcb->winfo.cal.calptn) {
			*p_rdvno = rdvno = gen_rdvno(tcb);
			*p_cmsgsz = tcb->winfo.cal.cmsgsz;
			memcpy(msg, tcb->winfo.cal.msg, *p_cmsgsz);
			wait_cancel(tcb);

			tcb->wspec = &wspec_rdv;
			tcb->wgcb = (GCB *) 0;
			tcb->winfo.rdv.rdvno = rdvno;
			tcb->winfo.rdv.msg = tcb->winfo.cal.msg;
			tcb->winfo.rdv.maxrmsz = porcb->maxrmsz;
			tcb->winfo.rdv.p_rmsgsz = tcb->winfo.cal.p_rmsgsz;
			timer_insert(&(tcb->wtmeb), TMO_FEVR,
					(CBACK) wait_release_tmout, tcb);
			queue_initialize(&(tcb->tskque));
			goto error_exit;
		}
	}
	ercd = E_TMOUT;
	if (tmout != TMO_POL) {
		ctxtsk->wspec = &wspec_acp;
		ctxtsk->wgcb = (GCB *) porcb;
		ctxtsk->wercd = &ercd;
		ctxtsk->winfo.acp.acpptn = acpptn;
		ctxtsk->winfo.acp.msg = msg;
		ctxtsk->winfo.acp.p_rdvno = p_rdvno;
		ctxtsk->winfo.acp.p_cmsgsz = p_cmsgsz;
		make_wait(tmout);
		queue_insert(&(ctxtsk->tskque), &(porcb->accept_queue));
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_tacp_por */
#ifndef _i_fwd_por

SYSCALL ER
i_fwd_por(ID porid, UINT calptn, RNO rdvno, VP msg, INT cmsgsz)
{
	PORCB	*porcb;
	TCB	*caltcb, *tcb;
	QUEUE	*queue;
	RNO	new_rdvno;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PORACV(porid);
	CHECK_PAR(calptn != 0);
	CHECK_RDVNO(rdvno);
	CHECK_PAR(cmsgsz > 0);
	CHECK_INTSK();
	porcb = get_porcb(porid);
	caltcb = get_tcb(get_tskid_rdvno(rdvno));

	BEGIN_CRITICAL_SECTION;
	if (porcb->poratr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#ifdef CHK_PAR
	if (cmsgsz > porcb->maxcmsz) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif /* CHK_PAR */
	if (!(caltcb->state & TS_WAIT) || (caltcb->wspec != &wspec_rdv)
				|| rdvno != caltcb->winfo.rdv.rdvno) {
		ercd = E_OBJ;
		goto error_exit;
	}
	if (porcb->maxrmsz > caltcb->winfo.rdv.maxrmsz) {
		ercd = E_OBJ;
		goto error_exit;
	}
#ifdef CHK_PAR
	if (cmsgsz > caltcb->winfo.rdv.maxrmsz) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif /* CHK_PAR */

	queue = porcb->accept_queue.next;
	while (queue != &(porcb->accept_queue)) {
		tcb = (TCB *) queue;
		queue = queue->next;
		if (calptn & tcb->winfo.acp.acpptn) {
			new_rdvno = gen_rdvno(caltcb);
			memcpy(tcb->winfo.acp.msg, msg, cmsgsz);
			*(tcb->winfo.acp.p_rdvno) = new_rdvno;
			*(tcb->winfo.acp.p_cmsgsz) = cmsgsz;
			wait_release_ok(tcb);

			assert(caltcb->wspec == &wspec_rdv);
			assert(caltcb->wgcb == (GCB *) 0);
			caltcb->winfo.rdv.rdvno = new_rdvno;
			caltcb->winfo.rdv.msg = caltcb->winfo.cal.msg;
			caltcb->winfo.rdv.maxrmsz = porcb->maxrmsz;
			caltcb->winfo.rdv.p_rmsgsz =
						caltcb->winfo.cal.p_rmsgsz;
			goto error_exit;
		}
	}
	caltcb->wspec = &wspec_cal;
	caltcb->wgcb = (GCB *) porcb;
	caltcb->winfo.cal.calptn = calptn;
	caltcb->winfo.cal.msg = caltcb->winfo.rdv.msg;
	caltcb->winfo.cal.cmsgsz = cmsgsz;
	caltcb->winfo.cal.p_rmsgsz = caltcb->winfo.rdv.p_rmsgsz;
	timer_insert(&(caltcb->wtmeb), TMO_FEVR,
			(CBACK) wait_release_tmout, caltcb);
	queue_insert(&(caltcb->tskque), &(porcb->call_queue));
	memcpy(caltcb->winfo.cal.msg, msg, cmsgsz);

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_fwd_por */
#ifndef _i_rpl_rdv

SYSCALL ER
i_rpl_rdv(RNO rdvno, VP msg, INT rmsgsz)
{
	TCB	*caltcb;
	ER	ercd = E_OK;

	CHECK_RDVNO(rdvno);
	CHECK_PORACV(porid);
	CHECK_PAR(rmsgsz >= 0);
	CHECK_INTSK();
	caltcb = get_tcb(get_tskid_rdvno(rdvno));

	BEGIN_CRITICAL_SECTION;
	if (!(caltcb->state & TS_WAIT) || (caltcb->wspec != &wspec_rdv)
				|| rdvno != caltcb->winfo.rdv.rdvno) {
		ercd = E_OBJ;
	}
#ifdef CHK_PAR
	else if (rmsgsz > caltcb->winfo.rdv.maxrmsz) {
		ercd = E_PAR;
	}
#endif /* CHK_PAR */
	else {
		memcpy(caltcb->winfo.rdv.msg, msg, rmsgsz);
		*(caltcb->winfo.rdv.p_rmsgsz) = rmsgsz;
		wait_release_ok(caltcb);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_rpl_rdv */
#ifndef _i_ref_por

SYSCALL ER
i_ref_por(T_RPOR *pk_rpor, ID porid)
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PORACV(porid);
	CHECK_INTSK();
	porcb = get_porcb(porid);
    
	BEGIN_CRITICAL_SECTION;
	if (porcb->poratr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rpor->exinf = porcb->exinf;
		pk_rpor->wtsk = wait_tskid(&(porcb->call_queue));
		pk_rpor->atsk = wait_tskid(&(porcb->accept_queue));
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_por */
#endif /* USE_POR */
