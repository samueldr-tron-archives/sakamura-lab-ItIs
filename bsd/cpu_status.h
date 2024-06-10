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
 *  @(#) $Id: cpu_status.h,v 1.8 1998/06/08 10:52:07 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  BSD UNIX $B0MB8Dj5A(B
 */

/* 
 *  BSD UNIX $BI8=`%$%s%/%k!<%I%U%!%$%k(B
 */
#include <signal.h>
#include <setjmp.h>

/*
 *  $B%7%0%J%k%^%9%/$NDj5A(B
 *
 *  $B%?%9%/%G%#%9%Q%C%A%c5/F0MQ$K(B SIGUSR2 $B$r;H$&!%(B
 *
 *  $BDL>o$N%?%9%/It<B9TCf$O%7%0%J%k%^%9%/$O(B 0 ($B$9$Y$F$N%7%0%J%k%^%9%/$,(B 
 *  $B2r=|$5$l$?>uBV(B) $B$K$J$C$F$$$k!%%G%#%9%Q%C%A6X;_>uBV$G$O(B SIGUSR2 $B$r(B 
 *  $B%^%9%/$7!$3d9~$_$*$h$S%G%#%9%Q%C%A6X;_>uBV$G$O$9$Y$F$N%7%0%J%k$r%^(B
 *  $B%9%/$9$k!%%?%9%/FHN)It<B9TCf$O!$>/$J$/$H$b(B SIGUSR2 $B$r%^%9%/$7$J$1(B
 *  $B$l$P$J$i$J$$!%(B
 */
#define SIGMASK_TASK	(0)
#define SIGMASK_DDSP	(sigmask(SIGUSR2))
#define SIGMASK_LOC	(~sigmask(SIGTRAP))
#define SIGMASK_INDP	(sigmask(SIGUSR2))

/*
 *  $B8=:_;HMQCf$N%9%?%C%/$rD4$Y$k(B
 *
 *  $B%?%9%/FHN)It<B9TCf$O!$%7%0%J%k%9%?%C%/$r;H$&$3$H$K$7$F!$%7%0%J%k%9(B
 *  $B%?%C%/$r;H$C$F$$$k$+$I$&$+$G!$%?%9%/FHN)It$rH=JL$9$k!%(B
 */
Inline int
current_stack()
{
#ifdef USE_SIGSTACK
	struct sigstack	ss;

	sigstack(0, &ss);
	return(ss.ss_onstack);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
	struct sigaltstack	ss;

	sigaltstack(0, &ss);
	return(ss.ss_flags & SS_ONSTACK);
#endif /* USE_SIGALTSTACK */
}

/*
 *  $B%/%j%F%#%+%k%;%/%7%g%sDj5AMQ%^%/%m(B
 */

#define BEGIN_CRITICAL_SECTION	{ int mask = sigblock(SIGMASK_LOC);
#define END_CRITICAL_SECTION	sigsetmask(mask); }

#define DISABLE_INTERRUPT	{ sigblock(SIGMASK_LOC); }

/*
 *  $B%7%9%F%`>uBVH=JLMQ%^%/%m(B
 */

/*
 *  in_indp(): $B%7%9%F%`%3!<%k$,!$%?%9%/FHN)It$+$i8F$P$l$?$+$rH=JL$9$k$?(B
 *  $B$a$N%^%/%m!%(B
 *
 *  $B%7%0%J%k%9%?%C%/$r;H$C$F$$$k$+$I$&$+$GH=CG$9$k!%(B
 */
#define in_indp()	(current_stack())

/*
 *  in_ddsp(): $B%7%9%F%`%3!<%k$,%G%#%9%Q%C%A6X;_Cf$K8F$P$l$?$+$rH=JL$9$k(B
 *  $B$?$a$N%^%/%m!%%?%9%/FHN)It$b!$%G%#%9%Q%C%A6X;_Cf$K4^$`!%(B
 *
 *  SIGUSR2 $B$,%^%9%/$5$l$F$$$k$+$I$&$+$GH=CG$9$k!%(B
 */
#define in_ddsp()	(sigblock(0) & SIGMASK_DDSP)

/*
 *  in_loc(): $B%7%9%F%`%3!<%k$,(B CPU$B%m%C%/Cf$K8F$P$l$?$+$rH=JL$9$k$?$a$N(B
 *  $B%^%/%m!%%?%9%/FHN)It$b!$(BCPU$B%m%C%/Cf$K4^$`!%(B
 *
 *  SIGUSR2 $B0J30$N$$$:$l$+$N%7%0%J%k$,%^%9%/$5$l$F$$$k>l9g$K!$(BCPU$B%m%C(B
 *  $B%/Cf$G$"$k$HH=CG$9$k!%%?%9%/FHN)It$G$O!$(BSIGUSR2 $B0J30$N$$$:$l$+$N%7(B
 *  $B%0%J%k$,%^%9%/$5$l$F$$$k$O$:!%(B
 */
#define in_loc()	(sigblock(0) & ~SIGMASK_DDSP)

/*
 *  in_sysmode(): $B%7%9%F%`%3!<%k$,%7%9%F%`%j%=!<%9$r%"%/%;%9$G$-$k>uBV$+(B
 *  $B$i8F$P$l$?$+$rH=JL$9$k$?$a$N%^%/%m!%(BE_OACV$B%(%i!<$N%A%'%C%/$KMQ$$$k!%(B
 *
 *  ctxtsk $B$N(B sysmode$B%U%#!<%k%I$r8+$FH=JL$9$kI,MW$,$"$k!%(B
 */
#define	in_sysmode()	(in_indp() || ctxtsk->sysmode)

/*
 *  in_qtsk(): $B%7%9%F%`%3!<%k$,!$=`%?%9%/It$r<B9TCf$+$i8F$P$l$?$+$rH=JL(B
 *  $B$9$k$?$a$N%^%/%m!%%?%9%/FHN)It$H$O6hJL$7$J$$$N$G!$(Bin_indp() $B$,(B FALSE 
 *  $B$N;~$K$N$_;H$&$3$H!%(B
 *
 *  $B$3$NH=JL$r2DG=$K$9$k$?$a$K!$(BTCB $BCf$K(B isysmode$B%U%#!<%k%I$rMQ0U$9$k!%(B
 *  USE_QTSK_PORTION $B$,Dj5A$5$l$F$$$J$$>l9g$K$O!$=`%?%9%/It$O;H$o$l$J$$(B
 *  $B$N$G!$>o$K(B FALSE $B$H$9$k!%(B
 */
#ifdef USE_QTSK_PORTION
#define in_qtsk()	(ctxtsk->sysmode > ctxtsk->isysmode)
#else /* USE_QTSK_PORTION */
#define in_qtsk()	0
#endif /* USE_QTSK_PORTION */

/*
 *  $B%?%9%/%G%#%9%Q%C%A%c5/F0%k!<%A%s(B
 */

/*
 *  $B%?%9%/%G%#%9%Q%C%A%c(B
 */
extern void dispatch_handler();

/*
 *  $B%?%9%/%G%#%9%Q%C%A%c$N5/F0MW5a$r=P$9!%(B
 */
Inline void
dispatch_request(void)
{
	kill(getpid(), SIGUSR2);
}

/*
 *  $B8=:_$N%?%9%/$N%3%s%F%-%9%H$r<N$F$F!$<!$K<B9T$9$Y$-%?%9%/$X6/@)E*$K(B
 *  $B%G%#%9%Q%C%A$9$k!%(B
 *
 *  $B%7%9%F%`5/F0;~$*$h$S(B ext_tsk, ext_tsk $B$N=hM}$GMQ$$$k!%(Bctxtsk $B$,A0(B
 *  $BJ};2>H$K$J$k$?$a!$(Bforce_dispatch $BK\BN$O!$%^%/%m$H$7$FDj5A$9$k!%(B
 */
Inline void
_force_dispatch(void)
{
#ifdef USE_SIGSTACK
	struct sigstack	ss;

	sigstack(0, &ss);
	ss.ss_onstack = 0;
	sigstack(&ss, 0);
#endif /* USE_SIGSTACK */
#ifdef USE_SIGALTSTACK
	struct sigaltstack	ss;

	sigaltstack(0, &ss);
	ss.ss_flags &= ~SS_ONSTACK;
	sigaltstack(&ss, 0);
#endif /* USE_SIGALTSTACK */
	dispatch_handler();
}

#define	force_dispatch() \
{					\
	ctxtsk = 0;			\
	_force_dispatch();		\
}

/*
 *  $B%?%9%/%3%s%F%-%9%H%V%m%C%/$NDj5A(B
 */
typedef struct {
	INT	stacd;		/* $B%?%9%/5/F0%3!<%I(B */
	jmp_buf	env;		/* CPU$B%3%s%F%-%9%H(B */
} CTXB;

#endif /* _CPU_STATUS_ */
