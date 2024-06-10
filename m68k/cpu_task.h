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
 *  $B%?%9%/5/F0$N$?$a$NDj?t$NDj5A(B
 */
#define INI_SR_SMODE	0x3000		/* SR $B$N=i4|CM(B ($B%9!<%Q%P%$%6%b!<%I(B) */
#define INI_SR_UMODE	0x1000		/* SR $B$N=i4|CM(B ($B%f!<%6%b!<%I(B) */

/*
 *  CPU$B0MB8$N%?%9%/5/F0=hM}(B
 *
 *  $B%7%9%F%`%9%?%C%/>e$K!$Nc30%9%?%C%/%U%l!<%`$r:n$k!%(Bmake_dormant $B$+$i(B
 *  $B8F$P$l$k!%(B
 */
Inline void
setup_context(TCB *tcb)
{
	VW	*ssp, *usp;

	ssp = (VW *)(tcb->isstack);	/* $B=i4|%7%9%F%`%9%?%C%/%]%$%s%?(B */

	if (SYSMODE(tcb->tskatr) && (tcb->tskatr & TA_HLNG)) {
		ssp -= 3;		/* $B5/F0%3!<%I$N$?$a$N%(%j%"$r<h$k(B */
	}
	*--((VH *) ssp) = 0;		/* $BNc30>pJs$r@Q$`(B */
	*--ssp = (VW)(tcb->task);	/* $B%?%9%/5/F0%"%I%l%9$r@Q$`(B */

	/*
	 *  SR $B$N=i4|CM$r%7%9%F%`%9%?%C%/$K@Q$_!$%f!<%6%9%?%C%/%]%$%s(B
	 *  $B%?$r=i4|2=$9$k!%(B
	 */
	if (!SYSMODE(tcb->tskatr)) {
		usp = tcb->istack;
		if (tcb->tskatr & TA_HLNG) {
			usp -= 3;	/* $B5/F0%3!<%I$N$?$a$N%(%j%"$r<h$k(B */
		}
		tcb->tskctxb.usp = usp;
		*--((VH *) ssp) = INI_SR_UMODE;
	}
	else {
		*--((VH *) ssp) = INI_SR_SMODE;
	}
	ssp -= 15;			/* D0$B!A(BD7/A0$B!A(BA6 $B$NJ,$N%(%j%"$r<h$k(B */

	tcb->tskctxb.msp = ssp;
}

/*
 *  make_dormant $B$G2u$5$l$k%9%?%C%/NN0h$N%5%$%:$NDj5A(B
 *
 *  ext_tsk $B$N=hM}Cf$G;H$o$l$k!%(B
 */
#define DORMANT_STACK_SIZE	(sizeof(VH) * 2 + sizeof(VW))

/*
 *  $B%?%9%/5/F0%3!<%IEy$N@_Dj(B
 *
 *  sta_tsk $B$N=hM}$+$i8F$P$l$k!%(B
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	VW	*sp;

	if (!(tcb->tskatr & TA_HLNG)) {
		sp = tcb->tskctxb.msp;
		*sp = stacd;			/* $B5/F0%3!<%I$r@Q$`(B (D0) */
		*(sp+1) = (VW)(tcb->exinf);	/* $B%?%9%/3HD%>pJs$r@Q$`(B (D1) */
	}
	else {
		sp = (SYSMODE(tcb->tskatr)) ? (VW *)(tcb->isstack)
					    : (VW *)(tcb->istack);
		*(sp-3) = (VW) NADR;		/* $BLa$jHVCO$r@Q$`(B */
		*(sp-2) = stacd;		/* $B5/F0%3!<%I$r@Q$`(B */
		*(sp-1) = (VW)(tcb->exinf);	/* $B%?%9%/3HD%>pJs$r@Q$`(B */
	}
}

#endif /* _CPU_TASK_ */
