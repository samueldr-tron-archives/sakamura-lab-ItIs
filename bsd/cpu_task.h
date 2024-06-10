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
 *  @(#) $Id: cpu_task.h,v 1.7 1998/06/02 03:07:07 hiro Exp $
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

/*
 *  $B%?%9%/5/F0%k!<%A%s(B
 */
extern void	task_start(void);

/*
 *  CPU$B0MB8$N%?%9%/5/F0=hM}(B
 *
 *  $B%7%9%F%`%9%?%C%/>e$K!$(BEIT$B%9%?%C%/%U%l!<%`$r:n$k!%(Bmake_dormant $B$+$i(B
 *  $B8F$P$l$k!%(B
 */
Inline void
setup_context(TCB *tcb)
{
	((int *) &(tcb->tskctxb.env))[JMPBUF_PC] = (int) task_start;
	((int *) &(tcb->tskctxb.env))[JMPBUF_SP] = (int)(((VB *) tcb->isstack)
							- STACK_MERGIN);
}

/*
 *  $B%?%9%/5/F0%3!<%IEy$N@_Dj(B
 *
 *  sta_tsk $B$N=hM}$+$i8F$P$l$k!%(B
 */
Inline void
setup_stacd(TCB *tcb, INT stacd)
{
	tcb->tskctxb.stacd = stacd;
}

/*
 *  $B%9%?%C%/%(%j%"$N<hF@(B/$BJV5Q(B
 *
 *  USE_MPROTECT_STACK $B$,Dj5A$5$l$F$$$k>l9g$K$O!$%9%?%C%/%(%j%"$r(B 2$B%Z!<(B
 *  $B%8J,B?$a$K3NJ]$7!$2<$NJ}$N(B 1$B%Z!<%8$r(B mprotect $B$G%"%/%;%9IT2D$K$9$k!%(B
 *  $B:G0-$G$b!$3NJ]$7$h$&$H$7$?J,$N%(%j%"$O;H$($k!%(B
 */

#ifdef USE_MPROTECT_STACK

#include <sys/types.h>
#include <sys/mman.h>

#ifndef PROT_NONE
#define PROT_NONE	0x00		/* $B%Z!<%8$r%"%/%;%9$G$-$J$/$9$k(B */
#endif

#define PROT_ORIG	(PROT_READ|PROT_WRITE)
					/* $B%Z!<%8$N>uBV$r85$KLa$9(B */

#define ALIGN(addr, unit)	((((INT)(addr)) + (unit) - 1) & ~((unit) - 1))

Inline VP
sys_get_stack(INT size)
{
	VP	blk;
	INT	pagesize = getpagesize();
  
	size += pagesize * 2;
	if (blk = sys_get_blk(size)) {
		mprotect((caddr_t) ALIGN(blk, pagesize), pagesize, PROT_NONE);
	}
	blk += pagesize * 2;
	return(blk);
}

Inline void
sys_rel_stack(VP blk)
{
	INT	pagesize = getpagesize();

	blk -= pagesize * 2;
	mprotect((caddr_t) ALIGN(blk, pagesize), pagesize, PROT_ORIG);
	sys_rel_blk(blk);
}

#endif /* USE_MPROTECT_STACK */
#endif /* _CPU_TASK_ */
