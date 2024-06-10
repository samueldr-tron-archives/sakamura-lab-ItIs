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
 *  $B%?%9%/5/F0$N$?$a$NDj?t$NDj5A(B
 */
#define INI_PSW_RNG0	0x800f6000	/* PSW $B$N=i4|CM(B ($B%j%s%0%l%Y%k(B0) */
#define INI_PSW_RNG1	0xa00f6000	/* PSW $B$N=i4|CM(B ($B%j%s%0%l%Y%k(B1) */
#define INI_PSW_RNG2	0xc00f6000	/* PSW $B$N=i4|CM(B ($B%j%s%0%l%Y%k(B2) */
#define INI_PSW_RNG3	0xe00f6000	/* PSW $B$N=i4|CM(B ($B%j%s%0%l%Y%k(B3) */

#define INI_CSW		0x00070000	/* CSW $B$N=i4|CM(B */

/*
 *  CPU$B0MB8$N%?%9%/5/F0=hM}(B
 *
 *  $B%7%9%F%`%9%?%C%/>e$K!$(BEIT$B%9%?%C%/%U%l!<%`$r:n$k!%(Bmake_dormant $B$+$i(B
 *  $B8F$P$l$k!%(B
 */
Inline void
setup_context(TCB *tcb)
{
	VW	*ssp;

	ssp = (VW *)(tcb->isstack);	/* $B=i4|%7%9%F%`%9%?%C%/%]%$%s%?(B */
#ifndef TRON_NO_DI
	*--ssp = (VW)(tcb->task);	/* $B%?%9%/5/F0%"%I%l%9$r@Q$`(B */
	*--ssp = DI14_EITINF;		/* EIT$B>pJs$r@Q$`(B */
#else /* TRON_NO_DI */
	*--ssp = 0;			/* EXPC ($B%@%_!<(B) $B$r@Q$`(B */
	*--ssp = (VW)(tcb->task);	/* $B%?%9%/5/F0%"%I%l%9$r@Q$`(B */
	*--ssp = TRAPA4_EITINF;		/* EIT$B>pJs$r@Q$`(B */
#endif /* TRON_NO_DI */

	/*
	 *  PSW $B$N=i4|CM$r%7%9%F%`%9%?%C%/$K@Q$_!$%f!<%6%9%?%C%/%]%$%s(B
	 *  $B%?$r=i4|2=$9$k!%(B
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
	ssp -= 15;			/* R14$B!A(BR0 $B$NJ,$N%(%j%"$r<h$k(B */

	tcb->tskctxb.sp0 = ssp;
	tcb->tskctxb.csw = INI_CSW;
}

/*
 *  make_dormant $B$G2u$5$l$k%9%?%C%/NN0h$N%5%$%:$NDj5A(B
 *
 *  ext_tsk $B$N=hM}Cf$G;H$o$l$k!%(B
 */
#ifndef TRON_NO_DI
#define DORMANT_STACK_SIZE	(sizeof(VW) * 3)
#else /* TRON_NO_DI */
#define DORMANT_STACK_SIZE	(sizeof(VW) * 4)
#endif /* TRON_NO_DI */

/*
 *  $B%?%9%/5/F0%3!<%IEy$N@_Dj(B
 *
 *  sta_tsk $B$N=hM}$+$i8F$P$l$k!%(B
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	VW	*ssp;

	ssp = tcb->tskctxb.sp0;
	*ssp = stacd;			/* $B5/F0%3!<%I$r@Q$`(B (R0) */
	*(ssp+1) = (VW)(tcb->exinf);	/* $B%?%9%/3HD%>pJs$r@Q$`(B (R1) */
}

#endif /* _CPU_TASK_ */
