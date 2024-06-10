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
 *  @(#) $Id: cpu_status.h,v 1.2 1998/06/18 07:45:20 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  m68k $B0MB8Dj5A(B
 */

/*
 *  m68k $BDc%l%Y%kDj5A(B
 */
#include "m68k_insn.h"

/*
 *  $B%/%j%F%#%+%k%;%/%7%g%sDj5AMQ%^%/%m(B
 *
 *  $B%/%j%F%#%+%k%;%/%7%g%s=*N;;~$K!$%G%#%9%Q%C%A$,I,MW$+$I$&$+$r%A%'%C(B
 *  $B%/$7!$I,MW$J$i%G%#%9%Q%C%A%c$r5/F0$9$k!%(B
 */

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask);			\
				if (!intmask_lock(intmask)		\
					    && !dispatch_disabled	\
					    && ctxtsk != schedtsk) {	\
					dispatch();			\
				} }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 *  $B%7%9%F%`>uBVH=JLMQ%^%/%m(B
 */

/*
 *  in_indp(): $B%7%9%F%`%3!<%k$,!$%?%9%/FHN)It$+$i8F$P$l$?$+$rH=JL$9$k$?(B
 *  $B$a$N%^%/%m!%(B
 *
 *  $B%7%9%F%`%3!<%k8F=P$7$N(B TRAP $B$G$O!$(BSR$BCf$N(B M$B%S%C%H$OJQ2=$7$J$$$N$G!$(B
 *  in_indp $B$,8F$P$l$?;~E@$N(B M$B%S%C%H$,(B 0 $B$J$i%?%9%/FHN)It$+$i8F$P$l$?$H(B
 *  $BH=JL$G$-$k!%(B
 */
#define in_indp()	((current_sr() & 0x1000) == 0)

/*
 *  in_loc(): $B%7%9%F%`%3!<%k$,(B CPU$B%m%C%/Cf$K8F$P$l$?$+$rH=JL$9$k$?$a$N(B
 *  $B%^%/%m!%%?%9%/FHN)It$b!$(BCPU$B%m%C%/Cf$K4^$`!%(B
 *
 *  $B%7%9%F%`%3!<%k8F$S=P$7$N(B TRAP $B$G$O(B IPM $B$OJQ2=$7$J$$$N$G!$(Bin_loc $B$,(B
 *  $B8F$P$l$?;~E@$N(B IPM $B$,(B 0 $B$h$jBg$-$1$l$P!$(BCPU$B%m%C%/Cf$K8F$P$l$?$H9M(B
 *  $B$($k!%<B:]$K$O!$(Bloc_cpu $B%7%9%F%`%3!<%k$r;H$&$H!$(BIPM $B$O(B 7 $B$K$J$k!%(B
 *  $B%?%9%/FHN)It$G(B IPM $B$,(B 0 $B$K$J$k%1!<%9$O9M$($J$$!%(B
 */
#define in_loc()	(current_intmask() > 0)

/*
 *  in_ddsp(): $B%7%9%F%`%3!<%k$,%G%#%9%Q%C%A6X;_Cf$K8F$P$l$?$+$rH=JL$9$k(B
 *  $B$?$a$N%^%/%m!%(BCPU$B%m%C%/Cf$b!$%G%#%9%Q%C%A6X;_Cf$K4^$`!%(B
 *
 *  CPU$B%m%C%/Cf$O!$(Bin_loc $B$G%A%'%C%/$9$k!%%G%#%9%Q%C%A$N$_6X;_$5$l$?>u(B
 *  $BBV$O!$(Bdispatch_disabled $B$G%A%'%C%/$9$k!%(B
 */
#define in_ddsp()	(in_loc() || dispatch_disabled)

/*
 *  in_sysmode(): $B%7%9%F%`%3!<%k$,%7%9%F%`%j%=!<%9$r%"%/%;%9$G$-$k>uBV$+(B
 *  $B$i8F$P$l$?$+$rH=JL$9$k$?$a$N%^%/%m!%(BE_OACV$B%(%i!<$N%A%'%C%/$KMQ$$$k!%(B
 *
 *  SR$BCf$NF0:n%b!<%I$O!$%7%9%F%`%3!<%k8F$S=P$7$N(B TRAP $B$K$h$jJQ2=$9$k$?(B
 *  $B$a!$(Bin_sysmode $B$,8F$P$l$?;~E@$N(B S$B%S%C%H$r8+$FH=JL$9$k$3$H$O$G$-$:!$(B
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
 *  $B%?%9%/%G%#%9%Q%C%A%c$N5/F0MW5a$r=P$9!%(B
 */
Inline void
dispatch_request(void)
{
}

/*
 *  $B8=:_$N%?%9%/$N%3%s%F%-%9%H$r<N$F$F!$<!$K<B9T$9$Y$-%?%9%/$X6/@)E*$K(B
 *  $B%G%#%9%Q%C%A$9$k!%(B
 *
 *  $B%7%9%F%`5/F0;~$*$h$S(B ext_tsk, exd_tsk $B$N=hM}$GMQ$$$k!%(B
 *
 *  $B=hM}FbMF$H$7$F$O!$(Bdispatch_to_schedtsk $B$XJ,4t$9$k$@$1$G$"$k!%J,4t8e(B
 *  $B$O%?%9%/$N%3%s%F%-%9%H$O<N$F$i$l$k$N$G4X?t8F=P$7$G$b4pK\E*$K$OLdBj(B
 *  $B$J$$$,!$(BISP $B$r;H$C$F$$$k%b!<%I$G8F$s$@>l9g(B ($B@5>o$K;H$C$F$$$l$P!$%7(B
 *  $B%9%F%`5/F0;~$N$_(B) $B$K!$(Bbra $BL?Na$GJ,4t$7$?J}$,4X?t8F=P$7$r9T$&$h$j$b(B
 *  $BJV$jHVCO$N%9%?%C%/NN0hJ,$,(B ($B$o$:$+(B 4$B%P%$%H$@$1$G$O$"$k$,(B) $B@aLs$G$-(B
 *  $B$k!%(B
 */
Inline void
force_dispatch(void)
{
	Asm("bra _dispatch_to_schedtsk");
}

/*
 *  $B%?%9%/%G%#%9%Q%C%A%c$r5/F0$9$k!%(B
 */
Inline void
dispatch()
{
	Asm("trap #7" : : : "memory");
}

/*
 *  $B%?%9%/%3%s%F%-%9%H%V%m%C%/$NDj5A(B
 */
typedef struct {
	VP	msp;		/* MSP (Master Stack Pointer) */
	VP	usp;		/* USP (User Stack Pointer) */
} CTXB;

#endif /* _CPU_STATUS_ */
