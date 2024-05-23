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
 *  @(#) $Id: cpu_calls.c,v 1.4 1997/01/10 12:34:56 hiro Exp $
 */

/* 
 *  CPU に依存するシステムコール
 */

#include "itis_kernel.h"
#include "task.h"
#include "check.h"

/*
 *  ディスパッチの禁止/許可機能
 */

#ifndef _i_dis_dsp

SYSCALL ER
i_dis_dsp(void)
{
	CHECK_CTX(!in_loc());
#ifndef TRON_NO_DI
	set_imask(14);
#else /* TRON_NO_DI */
	dispatch_disabled = 1;
#endif /* TRON_NO_DI */
	return(E_OK);
}

#endif /* _i_dis_dsp */
#ifndef _i_ena_dsp

SYSCALL ER
i_ena_dsp(void)
{
	CHECK_CTX(!in_loc());
#ifndef TRON_NO_DI
	set_imask(15);
#else /* TRON_NO_DI */
	dispatch_disabled = 0;
	if (ctxtsk != schedtsk) {
		dispatch();
	}
#endif /* TRON_NO_DI */
	return(E_OK);
}

#endif /* _i_ena_dsp */

/*
 *  割込み管理機能
 */

#ifndef _i_def_int

extern void	inthdr_startup(void);

static FP	hll_eit_table[0xff];

SYSCALL ER
i_def_int(UINT dintno, T_DINT *pk_dint)
{
	FP	inthdr;
	UINT	eitatr;

	CHECK_PAR(0x01 <= dintno && dintno <= 0xff);
	if (pk_dint != NADR) {
		CHECK_RSATR(pk_dint->intatr, TA_HLNG|TA_EITATR);
		if (pk_dint->intatr & TA_EITATR) {
			CHECK_PAR((pk_dint->eitatr & ~(0x831f0000)) == 0);
			eitatr = pk_dint->eitatr;
		}
		else {
			eitatr = EITATR(0, 15);
		}
		inthdr = pk_dint->inthdr;

		BEGIN_CRITICAL_SECTION;
		if (pk_dint->intatr & TA_HLNG) {
			hll_eit_table[dintno - 1] = inthdr;
			inthdr = inthdr_startup;
		}
		define_eit(dintno, eitatr, inthdr);
		END_CRITICAL_SECTION;
	}
	else {
#ifdef EITVB_ITIS
		BEGIN_CRITICAL_SECTION;
		(current_eitvb())[dintno] = ((EITVE *) EITVB_ORIG)[dintno];
		END_CRITICAL_SECTION;
#else /* EITVB_ITIS */
		return(E_NOSPT);
#endif /* EITVB_ITIS */
	}
	return(E_OK);
}

/*
 *  割込みハンドラ起動のための高級言語対応ルーチン
 */
__asm__(".text				\n"
"	.align 1			\n"
"_inthdr_startup:			\n"
"	stm (r0-r6), @-sp		\n"
"	mova @(28,sp).w, r0.w		\n"	/* EITスタックフレームの先頭 */
"	movu @(7,r0).b, r1.w		\n"	/* ベクタ番号を取り出す */
"	jsr @@(_hll_eit_table-4,r1*4)	\n"
"	ldm @sp+, (r0-r6)		\n"
#ifndef TRON_NO_DI
"	reit				\n");
#else /* TRON_NO_DI */
"	bra __ret_int			\n");
#endif /* TRON_NO_DI */

#endif /* _i_def_int */

/*
 *  割込みの禁止/許可機能
 */

#ifndef _i_loc_cpu

SYSCALL ER
i_loc_cpu(void)
{
	CHECK_INTSK();
	set_imask(0);
	return(E_OK);
}

#endif /* _i_loc_cpu */
#ifndef _i_unl_cpu

SYSCALL ER
i_unl_cpu(void)
{
	CHECK_INTSK();
	set_imask(15);
#ifdef TRON_NO_DI
	dispatch_disabled = 0;
	if (ctxtsk != schedtsk) {
		dispatch();
	}
#endif /* TRON_NO_DI */
	return(E_OK);
}

#endif /* _i_unl_cpu */

/*
 *  IMASK の設定/参照機能
 */

#ifndef _i_chg_ims

SYSCALL ER
i_chg_ims(UINT imask)
{
	CHECK_PAR(imask <= (in_indp() ? 7 : 15));
	set_imask(imask);
#ifdef TRON_NO_DI
	if (imask == 15 && !dispatch_disabled && ctxtsk != schedtsk) {
		dispatch();
	}
#endif /* TRON_NO_DI */
	return(E_OK);
}

#endif /* _i_chg_ims */
#ifndef _i_ref_ims

SYSCALL ER
i_ref_ims(UINT *p_imask)
{
	*p_imask = current_imask();
	return(E_OK);
}

#endif /* _i_ref_ims */

/* 
 *  デバッグサポート機能
 */

#ifdef USE_DEBUG_SUPPORT

/*
 *  タスクのレジスタ内容の設定，参照
 */

#ifndef _i_vset_reg

SYSCALL ER
i_vset_reg(ID tskid, T_REGS* pk_regs, T_EIT* pk_eit, T_CREGS* pk_cregs)
{
	TCB	*tcb;
	VW	*ssp;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	if (pk_eit != NADR) {
		CHECK_PAR(pk_eit->psw & 0x80000000);
	}
	CHECK_INTSK();
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if (tcb->state == TS_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	if (pk_cregs != NADR) {
		memcpy(&(tcb->tskctxb), pk_cregs, sizeof(T_CREGS));
	}
	ssp = tcb->tskctxb.sp0;

	if (pk_eit != NADR) {
		*(ssp + 17) = pk_eit->pc;
		*(ssp + 15) = pk_eit->psw;
	}

	if (pk_regs != NADR) {
		memcpy(ssp, pk_regs, sizeof(VW) * 15);
		if (pk_cregs == NADR) {
			assert(*(ssp + 15) & 0x80000000);
			switch (*(ssp + 15) & 0x60000000) {
			case 0:
				tcb->tskctxb.sp0 = (VP)(pk_regs->sp);
				break;
			case 0x20000000:
				tcb->tskctxb.sp1 = (VP)(pk_regs->sp);
				break;
			case 0x40000000:
				tcb->tskctxb.sp2 = (VP)(pk_regs->sp);
				break;
			case 0x60000000:
				tcb->tskctxb.sp3 = (VP)(pk_regs->sp);
				break;
			}
		}
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vset_reg */
#ifndef _i_vget_reg

SYSCALL ER
i_vget_reg(T_REGS* pk_regs, T_EIT* pk_eit, T_CREGS* pk_cregs, ID tskid)
{
	TCB	*tcb;
	VW	*ssp, psw;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_TSKACV(tskid);
	CHECK_NONSELF(tskid);
	CHECK_INTSK();
	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if (tcb->state == TS_NONEXIST) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	ssp = tcb->tskctxb.sp0;
	psw = *(ssp + 15);
	if (pk_regs != NADR) {
		memcpy(pk_regs, ssp, sizeof(VW) * 15);
		assert(psw & 0x80000000);
		switch (psw & 0x60000000) {
		case 0:
			pk_regs->sp = (VW) ssp;
			break;
		case 0x20000000:
			pk_regs->sp = (VW)(tcb->tskctxb.sp1);
			break;
		case 0x40000000:
			pk_regs->sp = (VW)(tcb->tskctxb.sp2);
			break;
		case 0x60000000:
			pk_regs->sp = (VW)(tcb->tskctxb.sp3);
			break;
		}
	}

	if (pk_eit != NADR) {
		pk_eit->pc = *(ssp + 17);
		pk_eit->psw = psw;
	}

	if (pk_cregs != NADR) {
		memcpy(pk_cregs, &(tcb->tskctxb), sizeof(T_CREGS));
	}

    error_exit:
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_vget_reg */
#endif /* USE_DEBUG_SUPPORT */
