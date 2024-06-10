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
 *  @(#) $Id: task.h,v 1.19 1998/01/30 09:52:45 hiro Exp $
 */

#ifndef _TASK_
#define _TASK_

typedef struct task_control_block	TCB;
typedef struct generic_control_block	GCB;
typedef struct prisem_control_block	PISCB;

#include "queue.h"
#include "timer.h"
#include "winfo.h"

/*
 *  $B%?%9%/>uBV$NFbItI=8=$NDj5A(B
 *
 *  $B%?%9%/$,BT$A>uBV$+$I$&$+$O(B (state & TS_WAIT) $B$G%A%'%C%/$G$-$k(B.
 *  $B%?%9%/$,6/@)BT$A>uBV$+$I$&$+$O(B (state & TS_SUSPEND) $B$G%A%'%C%/$G$-$k(B.
 */
typedef enum {
	TS_NONEXIST = 0,	/* $BL$EPO?>uBV(B */
	TS_READY = 1,		/* $B<B9T$^$?$O<B9T2DG=>uBV(B */
	TS_WAIT = 2,		/* $BBT$A>uBV(B */
	TS_SUSPEND = 4,		/* $B6/@)BT$A>uBV(B */
	TS_WAITSUS = 6,		/* $BFs=EBT$A>uBV(B */
	TS_DORMANT = 8		/* $B5Y;_>uBV(B */
} TSTAT;

/*
 *  $B%?%9%/$,@8$-$F$$$k$+(B (NON-EXISTENT$B!$(BDORMANT$B0J30$+(B) $B$N%A%'%C%/(B
 */
Inline BOOL
task_alive(TSTAT state)
{
	return(state & (TS_READY|TS_WAIT|TS_SUSPEND));
}

/*
 *  $B%?%9%/M%@hEY$NFbIt(B/$B30ItI=8=JQ49%^%/%m(B
 */
#define int_priority(x)	((INT)((x) - MIN_PRI))
#define ext_tskpri(x)	((PRI)((x) + MIN_PRI))

/*
 *  $B%?%9%/%3%s%H%m!<%k%V%m%C%/(B (TCB) $B$NDj5A(B
 */
struct task_control_block {
	QUEUE	tskque;		/* $B%?%9%/%-%e!<(B */
	ID	tskid;		/* $B%?%9%/(BID */
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	tskatr;		/* $B%?%9%/B0@-(B */
	FP	task;		/* $B%?%9%/5/F0%"%I%l%9(B */
	INT	ipriority;	/* $B%?%9%/5/F0;~M%@hEY(B */
#ifdef USE_SEPARATE_SSTACK
	INT	stksz;		/* $B%f!<%6%9%?%C%/%5%$%:(B */
#endif /* USE_SEPARATE_SSTACK */
	INT	sstksz;		/* $B%7%9%F%`%9%?%C%/%5%$%:(B */
	INT	priority;	/* $B8=:_$NM%@hEY(B */
	TSTAT	state;		/* $B%?%9%/>uBV(B ($BFbItI=8=(B) */
	WSPEC	*wspec;		/* $BBT$A;EMM(B */
	GCB	*wgcb;		/* $BBT$A%*%V%8%'%/%H$N%3%s%H%m!<%k%V%m%C%/(B */
	INT	wupcnt;		/* $B5/>2MW5a%-%e!<%$%s%0?t(B */
	INT	suscnt;		/* SUSPEND$BMW5a%M%9%H?t(B */
	INT	sysmode;	/* $B%?%9%/F0:n%b!<%I!$=`%?%9%/It8F=P$7%l%Y%k(B */
#ifdef USE_QTSK_PORTION
	INT	isysmode;	/* $B%?%9%/F0:n%b!<%I=i4|CM(B */
#endif /* USE_QTSK_PORTION */

#ifdef USE_POR
	RNO	wrdvno;		/* $B%i%s%G%VHV9f@8@.MQ(B */
#endif /* USE_POR */
#ifdef PRISEM_SPEC1
	PISCB	*pislist;	/* $B3MF@M%@hEY7Q>5%;%^%U%)%j%9%H(B */
#endif /* PRISEM_SPEC1 */
#ifdef USE_TASK_MAILBOX
	T_MSG	*tmq_head;	/* $B%?%9%/IUB0%a%C%;!<%8%-%e!<$N@hF,(B */
	T_MSG	*tmq_tail;	/* $B%?%9%/IUB0%a%C%;!<%8%-%e!<$NKvHx(B */
#endif /* USE_TASK_MAILBOX */
	ER	*wercd;		/* $BBT$A%(%i!<%3!<%I@_Dj%(%j%"(B */
	WINFO	winfo;		/* $BBT$A>pJs(B */
	TMEB	wtmeb;		/* $BBT$A%?%$%^%$%Y%s%H%V%m%C%/(B */

#ifdef USE_SEPARATE_SSTACK
	VP	istack;		/* $B%f!<%6%9%?%C%/%]%$%s%?$N=i4|CM(B */
#endif /* USE_SEPARATE_SSTACK */
	VP	isstack;	/* $B%7%9%F%`%9%?%C%/%]%$%s%?$N=i4|CM(B */
	CTXB	tskctxb;	/* $B%?%9%/%3%s%F%-%9%H%V%m%C%/(B */
};

/*
 *  $B%?%9%/%G%#%9%Q%C%A6X;_>uBV(B
 *
 *  dispatch_disabled $B$O!$%?%9%/%G%#%9%Q%C%A6X;_>uBV$r5-21$7$F$*$/$?$a(B
 *  $B$NJQ?t!%CY1d3d9~$_$r;H$&>l9g$J$I$K$O!$I,MW$J$$!%(B
 */
#ifdef USE_DISPATCH_DISABLED
extern BOOL	dispatch_disabled;
#endif /* USE_DISPATCH_DISABLED */

/*
 *  $B<B9TCf$N%?%9%/(B
 *
 *  ctxtsk $B$O!$<B9TCf$N%?%9%/(B (= CPU $B$,%3%s%F%-%9%H$r;}$C$F$$$k%?%9%/(B) 
 *  $B$N(B TCB $B$r;X$9JQ?t!%%7%9%F%`%3!<%k$N=hM}Cf$G!$%7%9%F%`%3!<%k$rMW5a$7(B
 *  $B$?%?%9%/$K4X$9$k>pJs$r;2>H$9$k>l9g$O!$(Bctxtsk $B$r;H$&!%(Bctxtsk $B$r=q$-(B
 *  $B49$($k$N$O!$%?%9%/%G%#%9%Q%C%A%c$N$_!%(B
 */
extern TCB	*ctxtsk;

/*
 *  $B<B9T$9$Y$-%?%9%/(B
 *
 *  schedtsk $B$O!$<B9T$9$Y$-%?%9%/$N(B TCB $B$r;X$9JQ?t!%CY1d%G%#%9%Q%C%A$d(B
 *  $B%G%#%9%Q%C%A$N6X;_$K$h$j%G%#%9%Q%C%A$,CY1d$5$l$F$$$k>uBV$G$O!$(B
 *  ctxtsk $B$H0lCW$7$J$$!%(B
 */
extern TCB	*schedtsk;

/*
 *  TCB $B$N%(%j%"(B
 *
 *  TCB $B$N%(%j%"$O!$%7%9%F%`@8@.;~$K@EE*$K3d$jIU$1$F$$$k!%(B
 */
extern TCB	tcb_table[];

/*
 *  $BL$;HMQ$N(B TCB $B$N%j%9%H(B
 */
#ifndef _i_vcre_tsk
extern QUEUE	free_tcb;
#endif /* _i_vcre_tsk */

/*
 *  $B%?%9%/(BID $B$+$i(B TCB $B$r<h$j=P$9!%(B
 */
#define get_tcb(id)	 (&tcb_table[INDEX_TSK(id)])
#define get_tcb_self(id) ((id)==TSK_SELF ? ctxtsk : &tcb_table[INDEX_TSK(id)])

/*
 *  $B%?%9%/$N<B9T=`Hw$r$9$k!%(B
 */
extern void	make_dormant(TCB *tcb);

/*
 *  $B%?%9%/$r<B9T2DG=>uBV$K$9$k!%(B
 *
 *  tcb $B$G<($5$l$k%?%9%/$NM%@hEY$,!$8=:_<B9TCf$N%?%9%/$NM%@hEY$h$j$b9b(B
 *  $B$$>l9g$O!$<B9T>uBV$K$9$k!%$=$&$G$J$$>l9g$O!$%?%9%/$r%l%G%#%-%e!<$K(B
 *  $B$D$J$0!%(B
 */
extern void	make_ready(TCB *tcb);

/*
 *  $B%?%9%/$r<B9T2DG=0J30$N>uBV$K$9$k!%(B
 *
 *  tcb $B$G<($5$l$k%?%9%/$r<B9T2DG=0J30$N>uBV!$$D$^$jBT$A>uBV!$6/@)BT$A(B
 *  $B>uBV!$5Y;_>uBV$X0\9T$5$;$k!%$3$N4X?t$r8F$S=P$9:]$K$O!$%?%9%/$,<B9T(B
 *  $B2DG=>uBV$K$J$C$F$$$k$3$H!%(Btcb->state $B$O!$8F$S=P$7$?B&$G!$$3$N4X?t$+(B
 *  $B$i%j%?!<%s8e$KJQ99$9$k!%(B
 */
extern void	make_non_ready(TCB *tcb);

/*
 *  $B%?%9%/$NM%@hEY$rJQ99$9$k!%(B
 *
 *  tcb $B$G<($5$l$k%?%9%/$NM%@hEY$r(B priority $B$KJQ99$9$k!%$=$l$KH<$C$FI,(B
 *  $BMW$H$J$k%?%9%/$N>uBVA+0\$r5/$3$5$;$k!%(B
 */
extern void	change_task_priority(TCB *tcb, INT priority);

/*
 *  $B%l%G%#%-%e!<$r2sE>$5$;$k!%(B
 *
 *  rotate_ready_queue $B$O!$(Bpriority $B$G;XDj$5$l$?M%@hEY$N%l%G%#%-%e!<$r(B
 *  $B2sE>$5$;$k!%(Brotate_ready_queue_run $B$O!$%l%G%#%-%e!<Cf$N:G9bM%@hEY(B
 *  $B$N%?%9%/$r4^$`%l%G%#%-%e!<$r2sE>$5$;$k!%(B
 */
extern void	rotate_ready_queue(INT priority);
extern void	rotate_ready_queue_run(void);

#endif /* _TASK_ */
