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
 *  @(#) $Id: wait.h,v 1.11 1998/01/30 09:52:54 hiro Exp $
 */

#ifndef _WAIT_
#define _WAIT_

/*
 *  $B%?%9%/4VF14|!&DL?.%*%V%8%'%/%HHFMQ%k!<%A%s(B
 */

#include "queue.h"
#include "timer.h"
#include "task.h"

/*
 *  $B%?%9%/$NBT$A>uBV$r2r=|$9$k!%(B
 *
 *  $B%?%9%/$r%?%$%^%-%e!<$*$h$SBT$A%-%e!<$+$i$O$:$7!$%?%9%/>uBV$r99?7$9(B
 *  $B$k!%(Bwait_release_ok $B$O!$BT$A2r=|$5$l$?%?%9%/$K(B E_OK $B$rEO$9!%(B
 *  wait_release_tmout $B$O!$%?%$%^%-%e!<$+$i$O$:$9=hM}$r$7$J$$!%%?%$%`(B
 *  $B%"%&%H=hM}$N;~$KMQ$$$k!%(B
 */
extern void	wait_release(TCB *tcb);
extern void	wait_release_ok(TCB *tcb);
extern void	wait_release_tmout(TCB *tcb);

/*
 *  $B%?%9%/$NBT$A>uBV$r%-%c%s%;%k$9$k!%(B
 *
 *  $B%?%9%/$r%?%$%^%-%e!<$*$h$SBT$A%-%e!<$+$i$O$:$9!%%?%9%/>uBV$O99?7$7(B
 *  $B$J$$!%(B
 */
Inline void
wait_cancel(TCB *tcb)
{
	timer_delete(&(tcb->wtmeb));
	queue_delete(&(tcb->tskque));
}

/*
 *  $B<B9TCf$N%?%9%/$rBT$A>uBV$K0\9T$5$;!$%?%$%^%$%Y%s%H%-%e!<$K$D$J$0!%(B
 */
extern void	make_wait(TMO tmout);

/*
 *  $BBT$A%-%e!<$K$D$J$,$C$F$$$k%?%9%/$NBT$A>uBV$r$9$Y$F2r=|$7!$(BE_DLT$B%(%i!<(B
 *  $B$H$9$k!%(B
 *
 *  $B%?%9%/4VF14|!&DL?.%*%V%8%'%/%H$,:o=|$5$l$?;~$K;H$&!%(B
 */
extern void	wait_delete(QUEUE *wait_queue);

/*
 *  $BBT$A%-%e!<$N@hF,$N%?%9%/$N(B ID $B$r<h$j=P$9!%(B
 */
extern ID	wait_tskid(QUEUE *wait_queue);

/*
 *  $B%3%s%H%m!<%k%V%m%C%/6&DLItJ,A`:n%k!<%A%s(B
 *
 *  $B%?%9%/4VF14|!&DL?.%*%V%8%'%/%H$O$$$:$l$b!$%3%s%H%m!<%k%V%m%C%/$N@h(B
 *  $BF,ItJ,$,6&DL$K$J$C$F$$$k!%0J2<$O!$$=$N6&DLItJ,$r07$&$?$a$NHFMQ%k!<(B
 *  $B%A%s$G$"$k!%6&DLItJ,$r!$(BGCB ($BHFMQ%3%s%H%m!<%k%V%m%C%/(B) $B$H$$$&7?$K$9(B
 *  $B$k!%J#?t$NBT$A%-%e!<$r;}$D%*%V%8%'%/%H$N>l9g!$(B2$B$D$a0J9_$NBT$A%-%e!<(B
 *  $B$rA`:n$9$k>l9g$K$O!$$3$l$i$N%k!<%A%s$O;H$($J$$!%$^$?!$%*%V%8%'%/%H(B
 *  $BB0@-$N(B TA_TPRI$B%S%C%H$r;2>H$9$k$N$G!$$3$N%S%C%H$rB>$NL\E*$K;H$C$F$$(B
 *  $B$k>l9g$b!$$3$l$i$N%k!<%A%s$O;H$($J$$!%%*%V%8%'%/%H$,@8@.$5$l$F$$$J(B
 *  $B$$>l9g$K$O!$%*%V%8%'%/%HB0@-$r(B OBJ_NONEXIST $B$K$9$k(B ($B$=$N$?$a!$%*%V(B
 *  $B%8%'%/%HB0@-$N$9$Y$F$N%S%C%H$rFHN)$K;H$&$3$H$,$G$-$J$$$H$$$&@)8B$,(B
 *  $B$"$k(B)$B!%(B
 */

struct generic_control_block {
	QUEUE	wait_queue;	/* $BBT$A%-%e!<(B */
	ID	objid;		/* $B%*%V%8%'%/%H(BID */
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	objatr;		/* $B%*%V%8%'%/%HB0@-(B */
	/*  $B$3$l0J9_$KB>$N%U%#!<%k%I$,$"$C$F$b$h$$$,!$(B */
	/*  $BHFMQA`:n%k!<%A%s$G$O07$o$l$J$$!%(B */
};

#define OBJ_NONEXIST	(-1)

/*
 *  $B<B9TCf$N%?%9%/$rBT$A>uBV$K0\9T$5$;!$%?%$%^%$%Y%s%H%-%e!<$*$h$S%*%V(B
 *  $B%8%'%/%H$NBT$A%-%e!<$K$D$J$0!%$^$?!$(Bctxtsk $B$N(B wid $B$r@_Dj$9$k!%(B
 */
extern void	gcb_make_wait(GCB *gcb, TMO tmout);

/*
 *  $B%?%9%/$NM%@hEY$,JQ$o$C$?:]$K!$BT$A%-%e!<$NCf$G$N%?%9%/$N0LCV$r=$@5(B
 *  $B$9$k!%%*%V%8%'%/%HB0@-$K(B TA_TPRI $B$,;XDj$5$l$F$$$k>l9g$K$N$_!$8F$S=P(B
 *  $B$5$l$k!%(B
 */
extern void	gcb_change_priority(GCB *gcb, TCB *tcb);
extern void	obj_chg_pri(TCB *tcb, INT oldpri);

#endif /* _WAIT_ */
