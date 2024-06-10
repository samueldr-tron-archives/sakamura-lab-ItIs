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
 *  @(#) $Id: cpu_calls.c,v 1.3 1998/07/06 10:48:12 hiro Exp $
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
	dispatch_disabled = 1;
	return(E_OK);
}

#endif /* _i_dis_dsp */
#ifndef _i_ena_dsp

SYSCALL ER
i_ena_dsp(void)
{
	CHECK_CTX(!in_loc());
	dispatch_disabled = 0;
	if (ctxtsk != schedtsk) {
		dispatch();
	}
	return(E_OK);
}

#endif /* _i_ena_dsp */

/*
 *  割込み管理機能
 */

#ifndef _i_def_int

extern void	inthdr_startup(void);

static FP	hll_exc_table[0xfe];

SYSCALL ER
i_def_int(UINT dintno, T_DINT *pk_dint)
{
	FP	inthdr;

	CHECK_PAR(0x02 <= dintno && dintno <= 0xff);
	if (pk_dint != NADR) {
		CHECK_RSATR(pk_dint->intatr, TA_HLNG);
		inthdr = pk_dint->inthdr;

		BEGIN_CRITICAL_SECTION;
		if (pk_dint->intatr & TA_HLNG) {
			hll_exc_table[dintno - 2] = inthdr;
			inthdr = inthdr_startup;
		}
		define_exc(dintno, inthdr);
		END_CRITICAL_SECTION;
	}
	else {
#ifdef EXCVT_ITIS
		BEGIN_CRITICAL_SECTION;
		(current_vbr())[dintno] = ((EITVE *) EXCVT_ORIG)[dintno];
		END_CRITICAL_SECTION;
#else /* EXCVT_ITIS */
		return(E_NOSPT);
#endif /* EXCVT_ITIS */
	}
	return(E_OK);
}

/*
 *  割込みハンドラ起動のための高級言語対応ルーチン
 */
__asm__(".text				\n"
"	.align 1			\n"
"_inthdr_startup:			\n"
"	movem.l d0-d1/a0-a1, -(sp)	\n"
"	move.w (22,sp), d0		\n"	/* ベクタオフセットを読む */
"	and.l #0x0fff, d0		\n"
"	jsr ([_hll_exc_table-8,d0])	\n"
"	movem.l (sp)+, d0-d1/a0-a1	\n"
"	bra __ret_int			\n");

#endif /* _i_def_int */

/*
 *  割込みの禁止/許可機能
 */

#ifndef _i_loc_cpu

SYSCALL ER
i_loc_cpu(void)
{
	CHECK_INTSK();
	set_ipm(7);
	return(E_OK);
}

#endif /* _i_loc_cpu */
#ifndef _i_unl_cpu

SYSCALL ER
i_unl_cpu(void)
{
	CHECK_INTSK();
	set_ipm(0);
	dispatch_disabled = 0;
	if (ctxtsk != schedtsk) {
		dispatch();
	}
	return(E_OK);
}

#endif /* _i_unl_cpu */

/*
 *  IPM (Interrupt Priority Mask) の設定/参照機能
 */

#ifndef _i_chg_ipm

SYSCALL ER
i_chg_ipm(UINT ipm)
{
	CHECK_PAR(ipm <= 7);
	if (in_indp()) {
		CHECK_PAR(ipm > 0);
	}
	set_ipm(ipm);
	if (ipm == 0 && !dispatch_disabled && ctxtsk != schedtsk) {
		dispatch();
	}
	return(E_OK);
}

#endif /* _i_chg_ipm */
#ifndef _i_ref_ipm

SYSCALL ER
i_ref_ipm(UINT *p_ipm)
{
	*p_ipm = current_ipm();
	return(E_OK);
}

#endif /* _i_ref_ipm */

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
		CHECK_PAR((pk_eit->sr & 0x3000) != 0x2000);
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
	ssp = tcb->tskctxb.msp;

	if (pk_eit != NADR) {
		*((VW *)(((VH *)(ssp + 16)) + 1)) = pk_eit->pc;
		*((VH *)(ssp + 16)) = pk_eit->sr;
	}

	if (pk_regs != NADR) {
		memcpy(ssp, pk_regs, sizeof(VW) * 15);
		if (pk_cregs == NADR) {
			assert((*((VH *)(ssp + 16)) & 0x3000) != 0x2000);

			if (*((VH *)(ssp + 16)) & 0x2000) {
				tcb->tskctxb.msp = (VP)(pk_regs->sp);
			}
			else {
				tcb->tskctxb.usp = (VP)(pk_regs->sp);
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
	VW	*ssp;
	VH	sr;
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

	ssp = tcb->tskctxb.msp;
	sr = *((VH *)(ssp + 16));
	if (pk_regs != NADR) {
		memcpy(pk_regs, ssp, sizeof(VW) * 15);
		assert((sr & 0x3000) != 0x2000);
		if (sr & 0x2000) {
			pk_regs->sp = (VW) ssp;
		}
		else {
			pk_regs->sp = (VW)(tcb->tskctxb.usp);
		}
	}

	if (pk_eit != NADR) {
		pk_eit->pc = *((VW *)(((VH *)(ssp + 16)) + 1));
		pk_eit->sr = sr;
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
