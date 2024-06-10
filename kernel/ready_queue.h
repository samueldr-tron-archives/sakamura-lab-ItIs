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
 *  @(#) $Id: ready_queue.h,v 1.10 1998/01/30 09:52:40 hiro Exp $
 */

#ifndef _READY_QUEUE_
#define _READY_QUEUE_

/*
 *  $B%l%G%#%-%e!<A`:n%k!<%A%s(B
 */

#include "queue.h"

/*
 *  $B%l%G%#%-%e!<$N9=B$$NDj5A(B
 *
 *  $B%l%G%#%-%e!<Cf$K$O!$M%@hEY$4$H$N%?%9%/%-%e!<(B tskque $B$,MQ0U$5$l$F$*(B
 *  $B$j!$%?%9%/$N(B TCB $B$O!$3:Ev$9$kM%@hEY$N%-%e!<$KEPO?$5$l$k!%(B
 *  $B%l%G%#%-%e!<$N%5!<%A$r8zN($h$/9T$&$?$a$K!$M%@hEY$4$H$N%?%9%/%-%e!<(B
 *  $B$K%?%9%/$,F~$C$F$$$k$+$I$&$+$r<($9%S%C%H%^%C%WNN0h(B bitmap $B$rMQ0U$7(B
 *  $B$F$$$k!%$3$NNN0h$r;H$&$3$H$G!$%o!<%IC10L$N%5!<%A$,2DG=$K$J$j!$%a%b(B
 *  $B%j%"%/%;%9$N2s?t$r8:$i$9$3$H$,$G$-$k!%$?$@$7!$M%@hEY$N%l%Y%k?t$,>/(B
 *  $B$J$/!$%S%C%HA`:nL?Na$,=<<B$7$F$$$J$$(B CPU $B$N>l9g$K$O!$%S%C%H%^%C%WA`(B
 *  $B:n$N%*!<%P!<%X%C%I$N$?$a$K!$5U$K8zN($,Mn$A$k2DG=@-$b$"$k!%(BTRON$B;EMM(B
 *  $B%A%C%WMQ$N%3!<%I$HHFMQ%3!<%I$G!$%o!<%IFb$N%S%C%H$NHV9fIU$1$,5U$K$J(B
 *  $B$C$F$$$k$N$GCm0U$;$h!%(B
 *  $B$^$?!$%l%G%#%-%e!<Cf$N:G9bM%@hEY$N%?%9%/$r;2>H$9$kA`:n$r8zN($h$/9T(B
 *  $B$&$?$a$K!$%l%G%#%-%e!<Cf$N:G9bM%@hEY$N%?%9%/$NM%@hEY$r(B top_priority
 *  $B%U%#!<%k%I$KF~$l$F$*$/!%%l%G%#%-%e!<$,6u$N>l9g$K$O!$$3$N%U%#!<%k%I(B
 *  $B$NCM$r(B NUM_PRI $B$K$9$k!%$3$N;~$K(B tskque[top_priority] $B$r;2>H$9$k$H(B 
 *  NULL $B$rJV$7$?$$$?$a!$>o$K(B NULL $B$,F~$C$F$$$k(B null$B%U%#!<%k%I$rMQ0U$7(B
 *  $B$F$$$k!%(B
 */

#define BITMAPSZ	(sizeof(UINT) * 8)
#define NUM_BITMAP	((NUM_PRI + BITMAPSZ - 1) / BITMAPSZ)

typedef	struct ready_queue {
	INT	top_priority;		/* $B%l%G%#%-%e!<Cf$N:G9bM%@hEY(B */
	QUEUE	tskque[NUM_PRI];	/* $BM%@hEY$4$H$N%?%9%/%-%e!<(B */
	TCB	*null;			/* $B%l%G%#%-%e!<$,6u$K$J$C$?;~$N$?$a(B */
	UINT	bitmap[NUM_BITMAP];	/* $BM%@hEY$4$H$N%S%C%H%^%C%WNN0h(B */
} RDYQUE;

/*
 *  $B%S%C%H%^%C%WNN0hA`:n%i%$%V%i%j(B
 *
 *  $BHFMQ%3!<%I$G(B (1 << xxx) $B$H$7$F$$$kItJ,$O!$%F!<%V%k$rMQ0U$7$?J}$,8z(B
 *  $BN($,$h$$$H;W$o$l$k$,!$%F!<%V%k$N:n$jJ}$,(B CPU $B$N%o!<%ID9$K0MB8$7$F$7(B
 *  $B$^$&$N$G!$(B(1 << xxx) $B$N7A$K$7$F$$$k!%(B
 */

/*
 *  $B%l%G%#%-%e!<(B rq $B$N%S%C%H%^%C%WNN0hCf$N!$M%@hEY(B priority $B$KBP1~$9$k(B
 *  $B%S%C%H$r%;%C%H$9$k!%(B
 */
Inline void
bitmap_set(RDYQUE *rq, INT priority)
{
#ifdef tron
	Asm("bset %0, %a1" : : "g"(priority), "g"(rq->bitmap) : "memory");
#else /* tron */
	rq->bitmap[priority / BITMAPSZ] |= 1 << (priority % BITMAPSZ);
#endif /* tron */
}

/*
 *  $B%l%G%#%-%e!<(B rq $B$N%S%C%H%^%C%WNN0hCf$N!$M%@hEY(B priority $B$KBP1~$9$k(B
 *  $B%S%C%H$r%/%j%"$9$k!%(B
 */
Inline void
bitmap_clear(RDYQUE *rq, INT priority)
{
#ifdef tron
	Asm("bclr %0, %a1" : : "g"(priority), "g"(rq->bitmap) : "memory");
#else /* tron */
	rq->bitmap[priority / BITMAPSZ] &= ~(1 << (priority % BITMAPSZ));
#endif /* tron */
}

/*
 *  $B0J2<$N(B _ffs$B4X?t$O!$I8=`$N(B ffs$B4X?t$H0c$$!$(Bi $B$,(B 0 $B$N>l9g$O9MN8$7$F$*(B
 *  $B$i$:!$JV$9CM$b(B 0 $B$r%Y!<%9$K$7$F$$$k!%$^$?!$(BTRON$B;EMM%A%C%WMQ$N%3!<%I(B
 *  $B$G$O!$%5!<%A$9$kJ}8~$,5U$K$J$C$F$$$k!%(B
 *
 *  $BI8=`%i%$%V%i%j$K(B ffs $B$,$"$k$J$i!$<!$N$h$&$KDj5A$7$F!$I8=`%i%$%V%i%j(B
 *  $B$r;H$C$?J}$,8zN($,NI$$2DG=@-$,9b$$!%(B
 *	#define _ffs(i) (ffs(i) - 1)
 *  TRON$B;EMM%A%C%W$NI8=`@_Dj$G$O!$G$0UD9%S%C%H%U%#!<%k%IA`:nL?Na$r;H$&(B
 *  $B$N$G(B _ffs$B4X?t$O;H$o$l$J$$$,!$(Bbvsch $BL?Na$,$J$$>l9g$K$O$3$N4X?t$r;H$&(B
 *  $BI,MW$,$"$k!%(B
 */
Inline INT
_ffs(INT i)
{
#ifdef tron
	Asm("bsch/1/f %0, %2" : "=g"(i) : "0"(i), "rm"(0));
	return(i);
#else /* tron */
	INT	j = 0;

	assert(i != 0);
	while ((i & 1) == 0) {
		i >>= 1;
		j++;
	}
	return(j);
#endif /* tron */
}

/*
 *  $B%l%G%#%-%e!<$N=i4|2=(B
 */
Inline void
ready_queue_initialize(RDYQUE *rq)
{
	INT	i;

	rq->top_priority = NUM_PRI;
	for (i = 0; i < NUM_PRI; i++) {
		queue_initialize(&rq->tskque[i]);
	}
	rq->null = (TCB *) 0;
	memset(rq->bitmap, 0, sizeof(rq->bitmap));
}

/*
 *  $B%l%G%#%-%e!<Cf$N:G9bM%@hEY$N%?%9%/$r;2>H$9$k!%(B
 *
 *  $B%l%G%#%-%e!<$,6u$N;~$O(B NULL $B$rJV$9!%(B
 */
Inline TCB *
ready_queue_top(RDYQUE *rq)
{
	return((TCB *)(rq->tskque[rq->top_priority].next));
}

/*
 *  $B%l%G%#%-%e!<Cf$N:G9bM%@hEY$N%?%9%/$NM%@hEY$r;2>H$9$k!%(B
 *
 *  $B%l%G%#%-%e!<$,6u$N;~$K$O8F$P$l$J$$!%(B
 */
Inline INT
ready_queue_top_priority(RDYQUE *rq)
{
	return(rq->top_priority);
}

/*
 *  $B%?%9%/$r%l%G%#%-%e!<$KA^F~$9$k!%(B
 *
 *  tcb $B$G;X$9%?%9%/$NM%@hEY$HF1$8M%@hEY$r;}$D%?%9%/$NCf$G$N:G8e$KF~$l(B
 *  $B$k!%(B
 *  $B%S%C%H%^%C%WNN0h$N3:Ev$9$k%S%C%H$r%;%C%H$7!$I,MW$J$i(B top_priority 
 *  $B$r99?7$9$k!%(Btop_priority $B$r99?7$7$?;~$O(B 1$B!$$=$&$G$J$$>l9g$O(B 0 $B$rJV(B
 *  $B$9(B ($B%$%s%i%$%s4X?t$J$N$G!$JV$jCM$,ITMW$J>l9g$G$bL5BL$J%3!<%I$O=P$J(B
 *  $B$$(B)$B!%(B
 */
Inline BOOL
ready_queue_insert(RDYQUE *rq, TCB *tcb)
{
	INT	priority = tcb->priority;

	queue_insert(&(tcb->tskque), &(rq->tskque[priority]));
	bitmap_set(rq, priority);
	if (priority < rq->top_priority) {
		rq->top_priority = priority;
		return(1);
	}
	return(0);
}

/*
 *  $B%?%9%/$r%l%G%#%-%e!<$KA^F~$9$k!%(B
 *
 *  tcb $B$G;X$9%?%9%/$NM%@hEY$HF1$8M%@hEY$r;}$D%?%9%/$NCf$G$N@hF,$KF~$l(B
 *  $B$k!%(BRUN$B>uBV$N%?%9%/$r%l%G%#%-%e!<$+$i30$7$F$*$/<BAu$G!$%?%9%/$,%W(B
 *  $B%j%(%s%W%H$5$l$?>l9g$K;H$&!%(B
 *  $B%S%C%H%^%C%WNN0h$N3:Ev$9$k%S%C%H$r%;%C%H$7!$I,MW$J$i(B top_priority 
 *  $B$r99?7$9$k!%(B
 */
Inline void
ready_queue_insert_top(RDYQUE *rq, TCB *tcb)
{
	INT	priority = tcb->priority;

	queue_insert(&(tcb->tskque), rq->tskque[priority].next);
	bitmap_set(rq, priority);
	if (priority < rq->top_priority) {
		rq->top_priority = priority;
	}
}

/*
 *  $B%?%9%/$r%l%G%#%-%e!<$+$i:o=|$9$k!%(B
 *
 *  TCB $B$r3:Ev$9$kM%@hEY$N%?%9%/%-%e!<$+$i$O$:$7!$$=$l$K$h$C$F%?%9%/%-%e!<(B
 *  $B$,6u$K$J$C$?>l9g$K$O!$%S%C%H%^%C%WNN0h$N3:Ev$9$k%S%C%H$r%/%j%"$9$k!%(B
 *  $B$5$i$K!$:o=|$7$?%?%9%/$,:G9bM%@hEY$G$"$C$?;~$O!$(Btop_priority $B$r99?7(B
 *  $B$9$k!%$3$N:]$K!$<!$KM%@hEY$N9b$$%?%9%/$r%5!<%A$9$k$?$a$K!$%S%C%H%^%C(B
 *  $B%WNN0h$r;H$&!%(B
 */
Inline void
ready_queue_delete(RDYQUE *rq, TCB *tcb)
{
#if defined(tron) && !defined(TRON_LEVEL1)
   Asm("qdel %a1, r0	\n"	/* $B%?%9%/%-%e!<$+$i$N:o=|(B */
"	bne 1f		\n"	/* $B%-%e!<$,6u$K$J$i$J$$;~$O$3$l$G=*N;(B */
"	mov %2 ,r0	\n"
"	mov %3, r1	\n"
"	bclr r1, @r0	\n"	/* $B3:Ev%S%C%H$N%/%j%"(B */
"	cmp %4, r1	\n"
"	bne 1f		\n"	/* $B$h$j9bM%@hEY$N%?%9%/$,$"$k;~$O=*N;(B */
"	mov %5, r2	\n"
"	sub r1, r2	\n"	/* $B%S%C%H%5!<%A$9$k%S%C%HI}$r7W;;(B */
"	bvsch/1/f	\n"
"	mov r1, %0	\n"	/* top_priority $B$r99?7(B */
"1:			"
:	"=g"(rq->top_priority)
:	"g"(tcb->tskque.prev), "g"(rq->bitmap), "g"(tcb->priority),
	"g"(rq->top_priority), "g"(NUM_PRI)
:	"r0", "r1", "r2", "memory");
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	INT	priority = tcb->priority;
	INT	i;

	queue_delete(&(tcb->tskque));
	if (!queue_empty_p(&(rq->tskque[priority]))) {
		return;
	}
	bitmap_clear(rq, priority);
	if (priority != rq->top_priority) {
		return;
	}
	for (i = priority / BITMAPSZ; i < NUM_BITMAP; i++) {
		if (rq->bitmap[i]) {
			rq->top_priority = i * BITMAPSZ + _ffs(rq->bitmap[i]);
			return;
		}
	}
	rq->top_priority = NUM_PRI;
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
}

/*
 *  $B%l%G%#%-%e!<$NM%@hEY%l%Y%k(B priority $B$N%-%e!<$N@hF,$N%?%9%/$r!$%-%e!<(B
 *  $B:G8e$X0\F0$9$k!%%-%e!<$,6u$N>l9g$O!$2?$b$7$J$$!%(B
 *
 *  ready_queue_delete $B$H(B ready_queue_insert $B$rB3$1$F8F$s$G$bF1$8$3$H$,(B
 *  $B<B8=$G$-$k$,!$(Bready_queue_rotate $B$N>l9g$O%S%C%H%^%C%WNN0h$NA`:n$OI,(B
 *  $BMW$J$$$?$a!$8zN($r$h$/$9$k$?$a$KJL$N4X?t$K$7$F$$$k!%(B
 */
Inline void
ready_queue_rotate(RDYQUE *rq, INT priority)
{
#ifdef tron
   Asm("qdel %a0, r0	\n"
"	bvs 1f		\n"
"	qins @r0, %a0	\n"
"1:			"
:	/* no output */
:	"g"(&(rq->tskque[priority]))
:	"r0", "memory");
#else /* tron */
	QUEUE	*entry, *queue;

	if (!queue_empty_p(queue = &(rq->tskque[priority]))) {
		entry = queue_delete_next(queue);
		queue_insert(entry, queue);
	}
#endif /* tron */
}

#endif /* _READY_QUEUE_ */
