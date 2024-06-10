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
 *  @(#) $Id: bsd_sigio.h,v 1.1 1998/07/06 14:17:27 hiro Exp $
 */

#ifndef	_BSD_SIGIO_
#define	_BSD_SIGIO_

/*
 *  BSD UNIX$BMQ(B $B%N%s%V%m%C%-%s%0(BI/O $B%5%]!<%H%b%8%e!<%k(B
 *
 *  $B%N%s%V%m%C%-%s%0(BI/O $B%5%]!<%H%b%8%e!<%k$O!$(BSIGIO $B%7%0%J%k$K$h$j!$%f!<(B
 *  $B%6$,EPO?$7$?%3!<%k%P%C%/4X?t$r8F$S=P$95!G=$r;}$D!%(B
 *
 *  SIGIO $B%7%0%J%k$K$h$j%3!<%k%P%C%/4X?t$r<B9T$5$;$?$$>l9g$K$O!$(BSIGIO 
 *  $BDLCN%$%Y%s%H%V%m%C%/$rMQ0U$7!$$=$N(B callback $B%U%#!<%k%I$K%3!<%k%P%C(B
 *  $B%/4X?t!$(Barg $B%U%#!<%k%I$K%3!<%k%P%C%/4X?t$XEO$90z?t$r@_Dj$7!$(B
 *  eneuque_sigioeb $B$rMQ$$$F(B SIGIO $BDLCN%$%Y%s%H%-%e!<$KEPO?$9$k!%(B
 *
 *  $B8F$S=P$5$l$?%3!<%k%P%C%/4X?t$,(B 0 $B$rJV$9$H!$4XO"$9$k(B SIGIO $BDLCN%$%Y(B
 *  $B%s%H%V%m%C%/(B $B$O%-%e!<$KEPO?$5$l$?$^$^$H$J$j!$B3$/(B SIGIO $B%7%0%J%k$G(B
 *  $B:F$SF1$8%3!<(B $B%k%P%C%/4X?t$,8F$S=P$5$l$k!%%3!<%k%P%C%/4X?t$,(B 0 $B0J30(B
 *  $B$rJV$9$H!$(BSIGIO $BDLCN%$%Y%s%H%V%m%C%/$O%-%e!<$+$i:o=|$5$l!$%3!<%k%P%C(B
 *  $B%/4X?t$O$=$l0J9_8F$S=P$5$l$J$/$J$k!%(B
 */

/*
 *  SIGIO $BDLCN%$%Y%s%H%V%m%C%/$NDj5A(B
 */
typedef BOOL	(*SIGIO_CBACK)(VP);	/* SIGIO $B%3!<%k%P%C%/4X?t$N7?(B */

typedef struct bsd_sigio_event_block {
	VP		queue[2];	/* SIGIO $BDLCN%$%Y%s%H%-%e!<%(%j%"(B */
	SIGIO_CBACK	callback;	/* SIGIO $B%3!<%k%P%C%/4X?t(B */
	VP		arg;		/* $B%3!<%k%P%C%/4X?t$XEO$90z?t(B */
} SIGIOEB;

/*
 *  SIGIO $BDLCN%$%Y%s%H%V%m%C%/$NEPO?(B
 */
extern ER	enqueue_sigioeb(SIGIOEB *sigioeb);

#endif /* _BSD_SIGIO_ */
