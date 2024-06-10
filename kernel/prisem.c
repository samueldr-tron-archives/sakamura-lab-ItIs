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
 *  @(#) $Id: prisem.c,v 1.13 1998/01/30 09:52:38 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_PIS

/*
 *  $BM%@hEY7Q>5%;%^%U%)4IM}%V%m%C%/$NDj5A(B
 */

struct prisem_control_block {
	QUEUE	wait_queue;	/* $BM%@hEY7Q>5%;%^%U%)BT$A%-%e!<(B */
	ID	pisid;		/* $BM%@hEY7Q>5%;%^%U%)(BID */
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	pisatr;		/* $BM%@hEY7Q>5%;%^%U%)B0@-(B */
	TCB	*pistsk;	/* $BM%@hEY7Q>5%;%^%U%)<hF@%?%9%/(B */
#ifdef PRISEM_SPEC1
	PISCB	*pislist;	/* $B3MF@M%@hEY7Q>5%;%^%U%)%j%9%H(B */
#endif /* PRISEM_SPEC1 */
};

static PISCB	piscb_table[NUM_PISID];

#define get_piscb(id)	(&(piscb_table[INDEX_PIS(id)]))

#ifdef PRISEM_SPEC1
#define pis_waited(piscb)	(!queue_empty_p(&(piscb->wait_queue)))
#define	pis_head_pri(piscb)	(((TCB *)(piscb->wait_queue.next))->priority)
#endif /* PRISEM_SPEC1 */

/*
 *  $BL$;HMQ$NM%@hEY7Q>5%;%^%U%)4IM}%V%m%C%/$N%j%9%H(B
 */
#ifndef _i_vvcre_pis
QUEUE	free_piscb;
#endif /* _i_vvcre_pis */

/* 
 *  $BM%@hEY7Q>5%;%^%U%)4IM}%V%m%C%/$N=i4|2=(B
 */
void
prisem_initialize(void)
{
	INT	i;
	PISCB	*piscb;
	ID	pisid;

#ifndef _i_vvcre_pis
	queue_initialize(&free_piscb);
#endif /* _i_vvcre_pis */

	for(piscb = piscb_table, i = 0; i < NUM_PISID; piscb++, i++) {
		pisid = ID_PIS(i);
		piscb->pisid = pisid;
		piscb->pisatr = OBJ_NONEXIST;
#ifndef _i_vvcre_pis
		if (!SYS_PISID(pisid)) {
			queue_insert(&(piscb->wait_queue), &free_piscb);
		}
#endif /* _i_vvcre_pis */
	}
}

/*
 *  $BM%@hEY7Q>5%;%^%U%)(B ($B;EMM(B1) $B%5%]!<%H%k!<%A%s(B
 */

#ifdef PRISEM_SPEC1

/*
 *  $BM%@hEY$N@_Dj=hM}(B
 *
 *  $B%?%9%/$NM%@hEY$r!$$=$N%?%9%/$N=i4|M%@hEY$H3MF@$7$F$$$kM%@hEY7Q>5%;(B
 *  $B%^%U%)$rBT$C$F$$$k%?%9%/$NM%@hEY$NFb!$:G9b$N$b$N$K@_Dj$9$k!%$?$@$7!$(B
 *  $B@_Dj$9$Y$-M%@hEY$,8=:_$NM%@hEY$H0lCW$7$F$$$k>l9g$K$O!$2?$b$7$J$$!%(B
 */
reset_priority(TCB *tcb)
{
	INT	newpri = tcb->ipriority;
	PISCB	*piscb;

	for (piscb = tcb->pislist; piscb; piscb = piscb->pislist) {
		if (pis_waited(piscb) && newpri > pis_head_pri(piscb)) {
			newpri = pis_head_pri(piscb);
		}
	}
	if (newpri != tcb->priority) {
		change_task_priority(tcb, newpri);
	}
}

/*
 *  $B%?%9%/$,M%@hEY7Q>5%;%^%U%)$r2rJ|$7$?>l9g$N=hM}(B
 *
 *  $B%?%9%/$,3MF@$7$F$$$kM%@hEY7Q>5%;%^%U%)$N%j%9%H$r99?7$7!$I,MW$J$i(B 
 *  ($B6qBNE*$K$O!$%;%^%U%)$r2rJ|$9$k%?%9%/$NM%@hEY$,!$%;%^%U%)$N@hF,$G(B
 *  $BBT$C$F$$$k%?%9%/$NM%@hEY$HF1$8>l9g(B) $B%?%9%/$NM%@hEY$r@_Dj$7$J$*$9!%(B
 *  $B$?$@$7!$@_Dj$9$Y$-M%@hEY$,!$8=:_$NM%@hEY$HF1$8$+9b$$>l9g$K$O!$2?$b(B
 *  $B$7$J$$!%(B
 */
static void
release_prisem(TCB *tcb, PISCB *relpiscb)
{
	PISCB	*piscb, **last_piscb;

	last_piscb = &tcb->pislist;
	if (pis_waited(relpiscb) && tcb->priority == pis_head_pri(relpiscb)) {
		/*
		 *  $BM%@hEY$N@_Dj$,I,MW$J>l9g(B
		 */
		INT	newpri = tcb->ipriority;

		while (piscb = *last_piscb) {
			if (piscb == relpiscb) {
				*last_piscb = piscb->pislist;
				break;
			}
			if (pis_waited(piscb)
					&& newpri > pis_head_pri(piscb)) {
				newpri = pis_head_pri(piscb);
			}
			last_piscb = &piscb->pislist;
		}
		assert(piscb);

		while (piscb = piscb->pislist) {
			if (pis_waited(piscb)
					&& newpri > pis_head_pri(piscb)) {
				newpri = pis_head_pri(piscb);
			}
		}
		if (tcb->priority < newpri) {
			change_task_priority(tcb, newpri);
		}
	}
	else {
		/*
		 *  $BM%@hEY$N@_Dj$,I,MW$J$$>l9g(B
		 */
		while (piscb = *last_piscb) {
			if (piscb == relpiscb) {
				*last_piscb = piscb->pislist;
				break;
			}
			last_piscb = &piscb->pislist;
		}
		assert(piscb);
	}
}

/*
 *  $B%?%9%/=*N;;~$NM%@hEY7Q>5%;%^%U%)2rJ|=hM}(B
 *
 *  $B%?%9%/$,3MF@$7$F$$$kM%@hEY7Q>5%;%^%U%)$r$9$Y$F2rJ|$9$k!%%?%9%/$O=*(B
 *  $BN;$9$k$N$G!$%?%9%/$NM%@hEY$d3MF@$7$F$$$kM%@hEY7Q>5%;%^%U%)%j%9%H$O(B
 *  $BJ]<i$9$kI,MW$,$J$$!%(B
 */
void
signal_all_prisem(TCB *tcb)
{
	PISCB	*piscb, *next_piscb;
	TCB	*next_tcb;

	next_piscb = tcb->pislist;
	while (piscb = next_piscb) {
		next_piscb = piscb->pislist;
		if (!queue_empty_p(&(piscb->wait_queue))) {
			next_tcb = (TCB *)(piscb->wait_queue.next);
			wait_release_ok(next_tcb);
			piscb->pistsk = next_tcb;
			piscb->pislist = next_tcb->pislist;
			next_tcb->pislist = piscb;
		}
		else {
			piscb->pistsk = (TCB *) 0;
		}
	}
}

#endif /* PRISEM_SPEC1 */

/*
 *  $B%;%^%U%)BT$A$N%?%9%/$NM%@hEY$,JQ99$5$l$?>l9g$N=hM}(B
 *
 *  $BM%@hEY7Q>5%;%^%U%)BT$A%-%e!<$N=g=x$rD>$7!$<!$NM%@hEY@_Dj=hM}$r9T$&!%(B
 *  1) $BM%@hEY$,JQ99$5$l$?%?%9%/$NJQ998e$NM%@hEY$,!$M%@hEY7Q>5%;%^%U%)(B
 *     $B$r3MF@$7$F$$$k%?%9%/$h$j$b9b$$M%@hEY$r;}$D>l9g!$8e<T$N%?%9%/$r(B
 *     $BA0<T$NM%@hEY$K@_Dj$9$k!%(B
 *  2) ($B;EMM(B1 $B$N>l9g$N$_(B) $BM%@hEY$,JQ99$5$l$?%?%9%/$NJQ99A0$NM%@hEY$,!$(B
 *     $BM%@hEY7Q>5%;%^%U%)$r3MF@$7$F$$$k%?%9%/$NM%@hEY$HF1$8>l9g!$8e<T(B
 *     $B$N%?%9%/$NM%@hEY$r:F@_Dj$9$k!%(B
 */
void
pis_chg_pri(TCB *tcb, INT oldpri)
{
	PISCB	*piscb;
	TCB	*pistsk;

	piscb = (PISCB *) tcb->wgcb;
	gcb_change_priority((GCB *) piscb, tcb);

	pistsk = piscb->pistsk;
	assert(pistsk);
	if (pistsk->priority > tcb->priority) {
		change_task_priority(pistsk, tcb->priority);
	}
#ifdef PRISEM_SPEC1
	else if (pistsk->priority == oldpri) {
		reset_priority(pistsk);
	}
#endif /* PRISEM_SPEC1 */
}

/*
 *  $BM%@hEY7Q>5%;%^%U%)BT$A$N%?%9%/$,BT$A2r=|$5$l$?>l9g$N=hM}(B ($B;EMM(B1)
 *
 *  $BM%@hEY7Q>5%;%^%U%)$r3MF@$7$F$$$k%?%9%/$NM%@hEY$,!$BT$A2r=|%?%9%/$N(B
 *  $BM%@hEY$HF1$8>l9g$K!$A0<T$N%?%9%/$NM%@hEY$r:F@_Dj$9$k!%(B
 */

#ifdef PRISEM_SPEC1

void
pis_rel_wai(TCB *tcb)
{
	PISCB	*piscb;
	TCB	*pistsk;

	piscb = (PISCB *) tcb->wgcb;
	pistsk = piscb->pistsk;
	assert(pistsk);
	if (pistsk->priority == tcb->priority) {
		reset_priority(pistsk);
	}
}

#endif /* PRISEM_SPEC1 */

/*
 *  $BM%@hEY7Q>5%;%^%U%)BT$A;EMM$NDj5A(B
 */
#ifdef PRISEM_SPEC1
static WSPEC wspec_pis = { TTW_PIS, pis_chg_pri, pis_rel_wai };
#else /* PRISEM_SPEC1 */
static WSPEC wspec_pis = { TTW_PIS, pis_chg_pri, 0 };
#endif /* PRISEM_SPEC1 */

/*
 *  $BM%@hEY7Q>5%;%^%U%)4IM}5!G=(B
 */

#if !defined(_i_vcre_pis) || !defined(_i_vvcre_pis)

static void
_vcre_pis(PISCB *piscb, T_CPIS *pk_cpis)
{
#ifndef _i_vvcre_pis
	if (!SYS_PISID(piscb->pisid)) {
		queue_delete(&(piscb->wait_queue));
	}
#endif /* _i_vvcre_pis */

	queue_initialize(&(piscb->wait_queue));
	piscb->exinf = pk_cpis->exinf;
	piscb->pisatr = (pk_cpis->pisatr | TA_TPRI);
	piscb->pistsk = (TCB *) 0;
}

#endif /* !defined(_i_vcre_pis) || !defined(_i_vvcre_pis) */
#ifndef _i_vcre_pis

SYSCALL ER
i_vcre_pis(ID pisid, T_CPIS *pk_cpis)
{
	PISCB	*piscb;
	ER	ercd = E_OK;

	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	CHECK_RSATR(pk_cpis->pisatr, TA_TPRI);
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr != OBJ_NONEXIST) {
		ercd =  E_OBJ;
	}
	else {
		_vcre_pis(piscb, pk_cpis);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_pis */
#ifndef _i_vvcre_pis

SYSCALL ER
i_vvcre_pis(T_CPIS *pk_cpis)
{
	PISCB	*piscb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cpis->pisatr, TA_TPRI);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_piscb)) {
		ercd =  EV_FULL;
	}
	else {
		piscb = (PISCB *)(free_piscb.next);
		_vcre_pis(piscb, pk_cpis);
		ercd = (ER)(piscb->pisid);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vvcre_pis */
#ifndef _i_vdel_pis

SYSCALL ER
i_vdel_pis(ID pisid)
{
	PISCB	*piscb;
	ER	ercd = E_OK;
    
	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
#ifdef PRISEM_SPEC1
		/*
		 *  $B:o=|$9$kM%@hEY7Q>5%;%^%U%)$r3MF@$7$F$$$k%?%9%/$,$"(B
		 *  $B$k>l9g!$$=$N%?%9%/$NM%@hEY$r:F@_Dj$9$k!%(B
		 */
		if (piscb->pistsk) {
			release_prisem(piscb->pistsk, piscb);
		}
#endif /* PRISEM_SPEC1 */
		wait_delete(&(piscb->wait_queue));
#ifndef _i_vvcre_pis
		if (!SYS_PISID(piscb->pisid)) {
			queue_insert(&(piscb->wait_queue), &free_piscb);
		}
#endif /* _i_vvcre_pis */
		piscb->pisatr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vdel_pis */
#ifndef _i_vsig_pis

SYSCALL ER
i_vsig_pis(ID pisid)
{
	PISCB	*piscb;	
	TCB	*tcb;
	ER	ercd = E_OK;
    
	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	CHECK_INTSK();
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if (piscb->pistsk != ctxtsk) {
		ercd = E_OBJ;
		goto error_exit;
	}

#ifdef PRISEM_SPEC1
	release_prisem(ctxtsk, piscb);
#else /* PRISEM_SPEC1 */
	if (ctxtsk->priority < ctxtsk->ipriority) {
		change_task_priority(ctxtsk, ctxtsk->ipriority);
	}
#endif /* PRISEM_SPEC1 */
	if (!queue_empty_p(&(piscb->wait_queue))) {
		tcb = (TCB *)(piscb->wait_queue.next);
		wait_release_ok(tcb);
		piscb->pistsk = tcb;
#ifdef PRISEM_SPEC1
		piscb->pislist = tcb->pislist;
		tcb->pislist = piscb;
#endif /* PRISEM_SPEC1 */
	}
	else {
		piscb->pistsk = (TCB *) 0;
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vsig_pis */
#ifndef _i_vwai_pis

SYSCALL ER
i_vwai_pis(ID pisid)
{
	return(i_vtwai_pis(pisid, TMO_FEVR));
}

#endif /* _i_vwai_pis */
#ifndef _i_vpreq_pis

SYSCALL ER
i_vpreq_pis(ID pisid)
{
	return(i_vtwai_pis(pisid, TMO_POL));
}

#endif /* _i_vpreq_pis */
#if !defined(_i_vwai_pis) || !defined(_i_vpreq_pis) || !defined(_i_vtwai_pis)

SYSCALL ER
i_vtwai_pis(ID pisid, TMO tmout)
{
	PISCB	*piscb;
	TCB	*pistsk;
	ER	ercd = E_OK;

	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!(pistsk = piscb->pistsk)) {
		piscb->pistsk = ctxtsk;
#ifdef PRISEM_SPEC1
		piscb->pislist = ctxtsk->pislist;
		ctxtsk->pislist = piscb;
#endif /* PRISEM_SPEC1 */
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			if (pistsk->priority > ctxtsk->priority) {
				change_task_priority(pistsk, ctxtsk->priority);
			}
			ctxtsk->wspec = &wspec_pis;
			ctxtsk->wercd = &ercd;
			gcb_make_wait((GCB *) piscb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vtwai_pis */
#ifndef _i_vref_pis

SYSCALL ER
i_vref_pis(T_RPIS *pk_rpis, ID pisid)
{
	PISCB	*piscb;
	ER	ercd = E_OK;

	CHECK_PISID(pisid);
	CHECK_PISACV(pisid);
	piscb = get_piscb(pisid);

	BEGIN_CRITICAL_SECTION;
	if (piscb->pisatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rpis->exinf = piscb->exinf;
		pk_rpis->wtsk = wait_tskid(&(piscb->wait_queue));
		pk_rpis->pistsk = piscb->pistsk ? piscb->pistsk->tskid
						: (ID) FALSE;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vref_pis */
#endif /* USE_PIS */
