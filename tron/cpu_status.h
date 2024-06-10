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
 *  @(#) $Id: cpu_status.h,v 1.6 1998/01/30 09:56:28 hiro Exp $
 */

#ifndef	_CPU_STATUS_
#define	_CPU_STATUS_

/* 
 *  TRON$B;EMM%A%C%W0MB8Dj5A(B
 */

/*
 *  TRON$B;EMM%A%C%W(B $BDc%l%Y%kDj5A(B
 */
#include "tron_defs.h"
#include "tron_insn.h"

/*
 *  $B%/%j%F%#%+%k%;%/%7%g%sDj5AMQ%^%/%m(B
 *
 *  $BCY1d3d9~$_$r;H$o$J$$>l9g$K$O!$%/%j%F%#%+%k%;%/%7%g%s=*N;;~$K!$%G%#(B
 *  $B%9%Q%C%A$,I,MW$+$I$&$+$r%A%'%C%/$7!$I,MW$J$i%G%#%9%Q%C%A%c$r5/F0$9(B
 *  $B$kI,MW$,$"$k!%(B
 */

#ifndef TRON_NO_DI

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask); }

#else /* TRON_NO_DI */

#define BEGIN_CRITICAL_SECTION	{ INT intmask = current_intmask(); disint();
#define END_CRITICAL_SECTION	enaint(intmask);			\
				if (imask_psw(intmask) == 15		\
					    && !dispatch_disabled	\
					    && ctxtsk != schedtsk) {	\
					dispatch();			\
				} }
#endif /* TRON_NO_DI */

#define DISABLE_INTERRUPT	{ disint(); }

/*
 *  $B%7%9%F%`>uBVH=JLMQ%^%/%m(B
 */

/*
 *  in_indp(): $B%7%9%F%`%3!<%k$,!$%?%9%/FHN)It$+$i8F$P$l$?$+$rH=JL$9$k$?(B
 *  $B$a$N%^%/%m!%(B
 *
 *  $B%7%9%F%`%3!<%k8F=P$7$N(B TRAPA $B$G$O!$(BPSW$BCf$N%9%?%C%/%b!<%I$OJQ2=$7$J(B
 *  $B$$$N$G!$(Bin_indp $B$,8F$P$l$?;~E@$N%9%?%C%/%b!<%I$,(B 0 $B$J$i%?%9%/FHN)It(B
 *  $B$+$i8F$P$l$?$HH=JL$G$-$k!%(B
 */
#define in_indp()	((current_psw() & 0x80000000) == 0)

/*
 *  in_ddsp(): $B%7%9%F%`%3!<%k$,%G%#%9%Q%C%A6X;_Cf$K8F$P$l$?$+$rH=JL$9$k(B
 *  $B$?$a$N%^%/%m!%%?%9%/FHN)It$b!$%G%#%9%Q%C%A6X;_Cf$K4^$`!%(B
 *
 *  $B%7%9%F%`%3!<%k8F$S=P$7$N(B TRAPA $B$G$O(B IMASK $B$OJQ2=$7$J$$$N$G!$(Bin_ddsp 
 *  $B$,8F$P$l$?;~E@$N(B IMASK $B$,(B 15 $BL$K~$J$i!$%G%#%9%Q%C%A6X;_Cf$K8F$P$l$?(B
 *  $B$HH=JL$G$-$k!%%?%9%/FHN)It$G(B IMASK $B$,(B 15 $B$K$J$k%1!<%9$O9M$($J$$!%(B
 *
 *  $BCY1d3d9~$_$r;H$o$J$$>l9g$K$O!$(BIMASK = 14 $B$KAjEv$9$k>uBV$,!$(B
 *  dispatch_disabled $B$r(B TRUE $B$K$9$k$3$H$G5-21$5$l$k$?$a!$(B
 *  dispatch_disabled $B$N%A%'%C%/$bI,MW$G$"$k!%(B
 */
#ifndef TRON_NO_DI
#define in_ddsp()	(current_imask() < 15)
#else /* TRON_NO_DI */
#define in_ddsp()	(current_imask() < 15 || dispatch_disabled)
#endif /* TRON_NO_DI */

/*
 *  in_loc(): $B%7%9%F%`%3!<%k$,(B CPU$B%m%C%/Cf$K8F$P$l$?$+$rH=JL$9$k$?$a$N(B
 *  $B%^%/%m!%%?%9%/FHN)It$b!$(BCPU$B%m%C%/Cf$K4^$`!%(B
 *
 *  $B%7%9%F%`%3!<%k8F$S=P$7$N(B TRAPA $B$G$O(B IMASK $B$OJQ2=$7$J$$$N$G!$(Bin_loc 
 *  $B$,8F$P$l$?;~E@$N(B IMASK $B$,(B 14 $BL$K~$J$i!$(BCPU$B%m%C%/Cf$K8F$P$l$?$H9M$((B
 *  $B$k!%<B:]$K$O!$(Bloc_cpu $B%7%9%F%`%3!<%k$r;H$&$H!$(BIMASK $B$O(B 0 $B$K$J$k!%%?(B
 *  $B%9%/FHN)It$G(B IMASK $B$,(B 14 $B$J$$$7$O(B 15 $B$K$J$k%1!<%9$O9M$($J$$!%(B
 *
 *  $BCY1d3d9~$_$r;H$o$J$$>l9g$K$O!$(BIMASK $B$,(B 14 $B$K$J$k$3$H$O$J$/!$(B15 $BL$K~(B
 *  $B$+$I$&$+$G%A%'%C%/$9$k!%(B
 */
#ifndef TRON_NO_DI
#define in_loc()	(current_imask() < 14)
#else /* TRON_NO_DI */
#define in_loc()	(current_imask() < 15)
#endif /* TRON_NO_DI */

/*
 *  in_sysmode(): $B%7%9%F%`%3!<%k$,%7%9%F%`%j%=!<%9$r%"%/%;%9$G$-$k>uBV$+(B
 *  $B$i8F$P$l$?$+$rH=JL$9$k$?$a$N%^%/%m!%(BE_OACV$B%(%i!<$N%A%'%C%/$KMQ$$$k!%(B
 *
 *  PSW$BCf$N%j%s%0%l%Y%k$O!$%7%9%F%`%3!<%k8F$S=P$7$N(B TRAPA $B$K$h$jJQ2=$9$k(B
 *  $B$?$a!$(Bin_sysmode $B$,8F$P$l$?;~E@$N(B RNG $B$r8+$FH=JL$9$k$3$H$O$G$-$:!$(B
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
#ifndef TRON_NO_DI
	set_dir(14);
#endif /* TRON_NO_DI */
}

/*
 *  $B8=:_$N%?%9%/$N%3%s%F%-%9%H$r<N$F$F!$<!$K<B9T$9$Y$-%?%9%/$X6/@)E*$K(B
 *  $B%G%#%9%Q%C%A$9$k!%(B
 *
 *  $B%7%9%F%`5/F0;~$*$h$S(B ext_tsk, exd_tsk $B$N=hM}$GMQ$$$k!%(B
 *
 *  $B=hM}FbMF$H$7$F$O!$(Bdispatch_to_schedtsk $B$XJ,4t$9$k$@$1$G$"$k!%J,4t8e(B
 *  $B$O%?%9%/$N%3%s%F%-%9%H$O<N$F$i$l$k$N$G4X?t8F=P$7$G$b4pK\E*$K$OLdBj(B
 *  $B$J$$$,!$(BSPI $B$r;H$C$F$$$k%b!<%I$G8F$s$@>l9g(B ($B@5>o$K;H$C$F$$$l$P!$%7(B
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
 *
 *  TRON$B;EMM%A%C%W$G$O!$CY1d3d9~$_$r;H$o$J$$>l9g$K$N$_;H$o$l$k!%(B
 */
#ifdef TRON_NO_DI
Inline void
dispatch()
{
	Asm("trapa #4" : : : "memory");
}
#endif /* TRON_NO_DI */

/*
 *  $B%?%9%/%3%s%F%-%9%H%V%m%C%/$NDj5A(B
 */
typedef struct {
	VW	csw;		/* CSW (Context Status Word) */
	VP	sp0;		/* SP0 */
	VP	sp1;		/* SP1 */
	VP	sp2;		/* SP2 */
	VP	sp3;		/* SP3 */
	VW	uatb;		/* UATB */
	VW	lsid;		/* LSID */
} CTXB;

#endif /* _CPU_STATUS_ */
