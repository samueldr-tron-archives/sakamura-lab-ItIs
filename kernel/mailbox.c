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
 *  @(#) $Id: mailbox.c,v 1.15 1997/01/10 13:36:25 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"

#ifdef USE_MBX

/*
 *  メイルボックス管理ブロックの定義
 *
 *  mq_head は，メッセージキューの最初のメッセージを指すポインタで，メッ
 *  セージキューが空の場合には NULL になっている．
 *
 *  mq_tail は，メッセージキューが空でない場合にメッセージキューの末尾
 *  を指すポインタで，メッセージキューが空の場合の値は保証されない．メッ
 *  セージのキューイングが FIFO の場合 (TA_MFIFO) にのみ使われる．
 */

typedef struct mailbox_control_block {
	QUEUE	wait_queue;	/* メイルボックス待ちキュー */
	ID	mbxid;		/* メイルボックスID */
	VP	exinf;		/* 拡張情報 */
	ATR	mbxatr;		/* メイルボックス属性 */
	T_MSG	*mq_head;	/* メッセージキューの先頭 */
	T_MSG	*mq_tail;	/* メッセージキューの末尾 */
} MBXCB;

static MBXCB	mbxcb_table[NUM_MBXID];

#define get_mbxcb(id)	(&(mbxcb_table[INDEX_MBX(id)]))

/*
 *  未使用のメイルボックス管理ブロックのリスト
 */
#ifndef _i_vcre_mbx
QUEUE	free_mbxcb;
#endif /* _i_vcre_mbx */

/* 
 *  メイルボックス管理ブロックの初期化
 */
void mailbox_initialize()
{
	INT	i;
	MBXCB	*mbxcb;
	ID	mbxid;

#ifndef _i_vcre_mbx
	queue_initialize(&free_mbxcb);
#endif /* _i_vcre_mbx */

	for(mbxcb = mbxcb_table, i = 0; i < NUM_MBXID; mbxcb++, i++) {
		mbxid = ID_MBX(i);
		mbxcb->mbxid = mbxid;
		mbxcb->mbxatr = OBJ_NONEXIST;
#ifndef _i_vcre_mbx
		if (!SYS_MBXID(mbxid)) {
			queue_insert(&(mbxcb->wait_queue), &free_mbxcb);
		}
#endif /* _i_vcre_mbx */
	}
}

/*
 *  メッセージ操作用マクロ
 */
#define nextmsg(msg)	*((T_MSG **) &((msg)->msgque[0]))

/*
 *  優先度ベースのメッセージキューの操作
 */
Inline void
queue_insert_mpri(T_MSG *pk_msg, T_MSG **head)
{
#if defined(tron) && !defined(TRON_LEVEL1)
	register T_MSG	*msg asm("r0") = (T_MSG *) head;
	register T_MSG	*prevmsg asm("r1");
	register T_MSG	*r2 asm("r2") = (T_MSG *) 0;
	register INT	r3 asm("r3") = pk_msg->msgpri;
	register INT	r5 asm("r5") = offsetof(T_MSG, msgpri);

	Asm("qsch/ge/f"
	  : "g="(msg), "g="(prevmsg)
	  : "0"(msg), "g"(r2), "g"(r3), "g"(r5));
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	T_MSG	*msg;
	T_MSG	*prevmsg = (T_MSG *) head;

	while (msg = nextmsg(prevmsg)) {
		if (msg->msgpri >= pk_msg->msgpri) {
			break;
		}
		prevmsg = msg;
	}
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
	nextmsg(pk_msg) = msg;
	nextmsg(prevmsg) = pk_msg;
}

/*
 *  メイルボックス待ち仕様の定義
 */
static WSPEC wspec_mbx_tfifo = { TTW_MBX, 0, 0 };
static WSPEC wspec_mbx_tpri = { TTW_MBX, obj_chg_pri, 0 };

/*
 *  メイルボックス管理機能
 */

#if !defined(_i_cre_mbx) || !defined(_i_vcre_mbx)

static void
_cre_mbx(MBXCB *mbxcb, T_CMBX *pk_cmbx)
{
#ifndef _i_vcre_mbx
	if (!SYS_MBXID(mbxcb->mbxid)) {
		queue_delete(&(mbxcb->wait_queue));
	}
#endif /* _i_vcre_mbx */

	queue_initialize(&(mbxcb->wait_queue));
	mbxcb->exinf = pk_cmbx->exinf;
	mbxcb->mbxatr = pk_cmbx->mbxatr;
	mbxcb->mq_head = (T_MSG *) 0;
}

#endif /* !defined(_i_cre_mbx) || !defined(_i_vcre_mbx) */
#ifndef _i_cre_mbx

SYSCALL ER
i_cre_mbx(ID mbxid, T_CMBX *pk_cmbx)
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);
	CHECK_MBXACV(mbxid);
	CHECK_RSATR(pk_cmbx->mbxatr, TA_MPRI|TA_TPRI);
	mbxcb = get_mbxcb(mbxid);

	BEGIN_CRITICAL_SECTION;
	if (mbxcb->mbxatr != OBJ_NONEXIST) {
		ercd =  E_OBJ;
	}
	else {
		_cre_mbx(mbxcb, pk_cmbx);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_cre_mbx */
#ifndef _i_vcre_mbx

SYSCALL ER
i_vcre_mbx(T_CMBX *pk_cmbx)
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_cmbx->mbxatr, TA_MPRI|TA_TPRI);

	BEGIN_CRITICAL_SECTION;
	if (queue_empty_p(&free_mbxcb)) {
		ercd =  EV_FULL;
	}
	else {
		mbxcb = (MBXCB *)(free_mbxcb.next);
		_cre_mbx(mbxcb, pk_cmbx);
		ercd = (ER)(mbxcb->mbxid);
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vcre_mbx */
#ifndef _i_del_mbx

SYSCALL ER
i_del_mbx(ID mbxid)
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);
	CHECK_MBXACV(mbxid);
	mbxcb = get_mbxcb(mbxid);
    
	BEGIN_CRITICAL_SECTION;
	if (mbxcb->mbxatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		wait_delete(&(mbxcb->wait_queue));
#ifndef _i_vcre_mbx
		if (!SYS_MBXID(mbxcb->mbxid)) {
			queue_insert(&(mbxcb->wait_queue), &free_mbxcb);
		}
#endif /* _i_vcre_mbx */
		mbxcb->mbxatr = OBJ_NONEXIST;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_del_mbx */
#ifndef _i_snd_msg

SYSCALL ER
i_snd_msg(ID mbxid, T_MSG *pk_msg)
{
	MBXCB	*mbxcb;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);
	CHECK_MBXACV(mbxid);
	mbxcb = get_mbxcb(mbxid);
    
	BEGIN_CRITICAL_SECTION;
	if (mbxcb->mbxatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (!queue_empty_p(&(mbxcb->wait_queue))) {
		tcb = (TCB *)(mbxcb->wait_queue.next);
		*(tcb->winfo.mbx.ppk_msg) = pk_msg;
		wait_release_ok(tcb);
	}
	else {
		if (mbxcb->mbxatr & TA_MPRI) {
			queue_insert_mpri(pk_msg, &(mbxcb->mq_head));
		}
		else {
			nextmsg(pk_msg) = (T_MSG *) 0;
			if (mbxcb->mq_head) {
				nextmsg(mbxcb->mq_tail) = pk_msg;
			}
			else {
				mbxcb->mq_head = pk_msg;
			}
			mbxcb->mq_tail = pk_msg;
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_snd_msg */
#ifndef _i_rcv_msg

SYSCALL ER
i_rcv_msg(T_MSG **ppk_msg, ID mbxid)
{
	return(i_trcv_msg(ppk_msg, mbxid, TMO_FEVR));
}

#endif /* _i_rcv_msg */
#ifndef _i_prcv_msg

SYSCALL ER
i_prcv_msg(T_MSG **ppk_msg, ID mbxid)
{
	return(i_trcv_msg(ppk_msg, mbxid, TMO_POL));
}

#endif /* _i_prcv_msg */
#if !defined(_i_rcv_msg) || !defined(_i_prcv_msg) || !defined(_i_trcv_msg)

SYSCALL ER
i_trcv_msg(T_MSG **ppk_msg, ID mbxid, TMO tmout)
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;
    
	CHECK_MBXID(mbxid);
	CHECK_MBXACV(mbxid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();
	mbxcb = get_mbxcb(mbxid);
    
	BEGIN_CRITICAL_SECTION;
	if (mbxcb->mbxatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else if (mbxcb->mq_head) {
		*ppk_msg = mbxcb->mq_head;
		mbxcb->mq_head = nextmsg(*ppk_msg);
	}
	else {
		ercd = E_TMOUT;
		if (tmout != TMO_POL) {
			ctxtsk->wspec = (mbxcb->mbxatr & TA_TPRI) ?
					&wspec_mbx_tpri : &wspec_mbx_tfifo;
			ctxtsk->wercd = &ercd;
			ctxtsk->winfo.mbx.ppk_msg = ppk_msg;
			gcb_make_wait((GCB *) mbxcb, tmout);
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_trcv_msg */
#ifndef _i_ref_mbx

SYSCALL ER
i_ref_mbx(T_RMBX *pk_rmbx, ID mbxid)
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);
	CHECK_MBXACV(mbxid);
	mbxcb = get_mbxcb(mbxid);
    
	BEGIN_CRITICAL_SECTION;
	if (mbxcb->mbxatr == OBJ_NONEXIST) {
		ercd = E_NOEXS;
	}
	else {
		pk_rmbx->exinf = mbxcb->exinf;
		pk_rmbx->wtsk = wait_tskid(&(mbxcb->wait_queue));
		if (mbxcb->mq_head) {
			pk_rmbx->pk_msg = NADR;
		}
		else {
			pk_rmbx->pk_msg = mbxcb->mq_head;
		}
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_ref_mbx */
#endif /* USE_MBX */
