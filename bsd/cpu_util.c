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
 *  @(#) $Id: cpu_util.c,v 1.8 1998/06/08 10:52:08 hiro Exp $
 */

#include "itis_kernel.h"
#include "task.h"

/*
 *  $B%?%9%/%G%#%9%Q%C%A%c(B
 * 
 *  $B<B9TCf$N%?%9%/(B (ctxtsk) $B$N%3%s%F%-%9%H$r(B TCB $B$KJ]B8$7!$<B9T$9$Y$-(B
 *  $B%?%9%/(B (schedtsk) $B$r?7$7$$<B9T%?%9%/$H$7$F!$$=$N%3%s%F%-%9%H$r(B TCB 
 *  $B$+$iI|5"$9$k!%%?%9%/%G%#%9%Q%C%A%c$+$iLa$k:]$K!$%7%0%J%k%^%9%/$O0J(B
 *  $BA0$N>uBV$KLa$5$l$k!%(B
 */
void
dispatch_handler()
{
	if (!ctxtsk || (ctxtsk != schedtsk
				&& _setjmp(ctxtsk->tskctxb.env) == 0)) {
		sigsetmask(SIGMASK_LOC);
		while (!(ctxtsk = schedtsk)) {
			sigpause(SIGMASK_DDSP);
			sigsetmask(SIGMASK_LOC);
		}
		_longjmp(ctxtsk->tskctxb.env, 1);
	}
}

/*
 *  $B%?!<%2%C%H(BCPU$B0MB8$N=i4|2=%k!<%A%s(B
 */
void
cpu_initialize(void)
{
	/*
	 *  $B%7%0%J%k%9%?%C%/$r!$%W%m%;%9%9%?%C%/>e$K<h$k!%%7%0%J%k%9%?%C(B
	 *  $B%/$r;H$C$F$$$k$H@_Dj$9$k$N$O!$%9%?!<%H%"%C%W%k!<%A%s$r%?%9(B
	 *  $B%/FHN)It$HH=Dj$5$;$k$?$a!%(B
	 */
	{
#ifdef USE_SIGSTACK
	       	struct sigstack	ss;

		ss.ss_sp = (void *)(((INT) &ss) - SIGSTACK_MERGIN);
		ss.ss_onstack = 1;
		sigstack(&ss, 0);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
		struct sigaltstack	ss;

		ss.ss_sp = (void *)(((INT) &ss) - SIGSTACK_MERGIN - SIGSTKSZ);
		ss.ss_size = SIGSTKSZ;
		ss.ss_flags |= SS_ONSTACK;
		sigaltstack(&ss, 0);
#endif /* USE_SIGALTSTACK */
	}

	/*
	 *  $B%G%#%9%Q%C%AMQ$N%7%0%J%k%O%s%I%i$r@_Dj!%(B
	 */
	{
		struct sigvec	vec;

		vec.sv_handler = dispatch_handler;
		vec.sv_mask = 0;
		vec.sv_flags = SV_INTERRUPT;
		sigvec(SIGUSR2, &vec, 0);
	}
}

/*
 *  $B%?!<%2%C%H(BCPU$B0MB8$N=*N;=hM}%k!<%A%s(B
 */
void
cpu_shutdown(void)
{
}

/*
 *  $B%?%9%/5/F0%k!<%A%s(B
 *
 *  BSD UNIX$B>e$G$O!$%+!<%M%k$H%?%9%/$,F1$8FC8"%l%Y%k$GF0:n$9$k$?$a!$%?(B
 *  $B%9%/$X$NJ,4t$O4X?t8F$S=P$7$G<B8=$G$-$k!%(B
 */
void
task_start()
{
	/*
	 *  $B%7%0%J%k%^%9%/$r@_Dj$7$F!$%?%9%/$r5/F0$9$k!%(B
	 */
	sigsetmask(SIGMASK_TASK);
	(*ctxtsk->task)(ctxtsk->tskctxb.stacd, ctxtsk->exinf);

	/*
	 *  $B%?%9%/$N=*N;;~$K(B ext_tsk $B$r8F$V$N$G!$$3$3$X$OLa$i$J$$!%(B
	 */
	assert(0);
}

/*
 *  $B%7%9%F%`%a%b%j%W!<%k$r;H$o$J$$>l9g(B
 *
 *  UNIX$B%i%$%V%i%j$N(B malloc $B$H(B free $B$K%7%9%F%`%a%b%j%W!<%k$NLr3d$rG$$;(B
 *  $B$k!%(B
 */

#ifndef USE_TMPL_OS

#include <malloc.h>

ER
sys_cre_mpl(void)
{
	return(E_OK);
}

VP
sys_get_blk(INT blksz)
{
	return(malloc(blksz));
}

void
sys_rel_blk(VP blk)
{
	free(blk);
}

#endif /* USE_TMPL_OS */

/*
 *  $B3HD%(BSVC $B$N=PF~8}$N=hM}(B
 *
 *  $B3HD%(BSVC $BFb$G%7%9%F%`%*%V%8%'%/%H$r%"%/%;%9$G$-$k$h$&$K$9$k$?$a$N=hM}!%(B
 */

void
enter_extended_svc()
{
	ctxtsk->sysmode++;
}

void
exit_extended_svc()
{
	ctxtsk->sysmode--;
}
