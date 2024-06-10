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
 *  @(#) $Id: queue.h,v 1.7 1998/01/30 09:52:39 hiro Exp $
 */

#ifndef	_QUEUE_
#define	_QUEUE_

/*
 *  $B%@%V%k%j%s%/%-%e!<(B
 * 
 *  $B%@%V%k%j%s%/%-%e!<$N9=B$$O!$(BTRON$B;EMM%A%C%W$N%-%e!<A`:nL?Na$,07$&7A(B
 *  $B<0$K$"$o$;$F$"$k!%(B
 */
typedef struct queue {
	struct queue *next;
	struct queue *prev;
} QUEUE;

/*
 *  $B%-%e!<A`:n%i%$%V%i%j(B
 */

/*
 *  $B%-%e!<$N=i4|2=(B
 */
Inline void
queue_initialize(QUEUE *queue)
{
	queue->prev = queue->next = queue;
}

/*
 *  $B%-%e!<$X%(%s%H%j$rA^F~(B
 *
 *  queue $B$ND>A0$K(B entry $B$rA^F~$9$k!%(Bqueue $B$,%-%e!<%X%C%@$r;X$9>l9g$K$O!$(B
 *  $B%-%e!<$N:G8eHx$K(B entry $B$rA^F~$9$k$3$H$K$J$k!%(B
 */
Inline void
queue_insert(QUEUE *entry, QUEUE *queue)
{
#ifdef tron
	Asm("qins %a0, %a1" : : "g"(entry), "g"(queue) : "memory");
#else /* tron */
	entry->prev = queue->prev;
	entry->next = queue;
	queue->prev->next = entry;
	queue->prev = entry;
#endif /* tron */
}

/*
 *  $B%-%e!<$+$i%(%s%H%j$r:o=|(B
 *
 *  entry $B$r%-%e!<$+$i:o=|$9$k!%(B
 */
Inline void
queue_delete(QUEUE *entry)
{
#ifdef tron
	Asm("qdel %a0, r0" : : "g"(entry->prev) : "r0", "memory");
#else /* tron */
	if (entry->next != entry) {
		entry->prev->next = entry->next;
		entry->next->prev = entry->prev;
	}
#endif /* tron */
}

/*
 *  $B%-%e!<$N@hF,$N%(%s%H%j$N<h$j=P$7(B
 *
 *  queue $B$ND>8e$N%(%s%H%j$r%-%e!<$+$i:o=|$7!$:o=|$7$?%(%s%H%j$rJV$9!%(B
 *  queue $B$,%-%e!<%X%C%@$r;X$9>l9g$K$O!$%-%e!<$N@hF,$N%(%s%H%j$r<h$j=P(B
 *  $B$9$3$H$K$J$k!%(B
 */
Inline QUEUE *
queue_delete_next(QUEUE *queue)
{
	QUEUE	*entry;

	assert(queue->next != queue);
#ifdef tron
	Asm("qdel %a1, %0" : "r="(entry) : "g"(queue) : "memory");
#else /* tron */
	entry = queue->next;
	queue->next = entry->next;
	entry->next->prev = queue;
#endif /* tron */
	return(entry);
}

/*
 *  $B%-%e!<Cf$N%(%s%H%j$N%5!<%A(B
 *
 *  queue $B$G<($9%-%e!<$+$i!$(Boffset $B$G<($9%U%#!<%k%I$,(B val $B$h$jBg$-$$$N(B
 *  $B%(%s%H%j$r%5!<%A$7!$$=$N%(%s%H%j$rJV$9!%3:Ev$9$k%(%s%H%j$,$J$$>l9g(B
 *  $B$O!$(Bqueue $B$rJV$9!%(B
 */
Inline QUEUE *
queue_search_gt(QUEUE *queue, INT val, INT offset)
{
#if defined(tron) && !defined(TRON_LEVEL1)
	register QUEUE	*r0 asm("r0") = queue;
	register QUEUE	*r2 asm("r2") = queue;
	register INT	r3 asm("r3") = val + 1;
	register INT	r5 asm("r5") = offset;

	Asm("qsch/ge/f"
	  : "g="(r0)
	  : "0"(r0), "g"(r2), "g"(r3), "g"(r5)
	  : "r1");
	return(r0);
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	QUEUE	*entry;

	for (entry = queue->next; entry != queue; entry = entry->next) {
		if (*((INT *)(((VB *) entry) + offset)) > val) {
			break;
		}
	}
	return(entry);
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
}

/*
 *  $B%-%e!<$,6u$+$I$&$+$N%A%'%C%/(B
 */
Inline BOOL
queue_empty_p(QUEUE *queue)
{
	if (queue->next == queue) {
		assert(queue->prev == queue);
		return(1);
	}
	return(0);
}

#endif /* _QUEUE_ */
