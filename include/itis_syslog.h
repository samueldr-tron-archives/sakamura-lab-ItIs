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
 *  @(#) $Id: itis_syslog.h,v 1.5 1998/01/30 09:54:01 hiro Exp $
 */

#ifndef	_ITIS_SYSLOG_
#define	_ITIS_SYSLOG_

/* 
 *  $B%7%9%F%`%m%0=PNO%i%$%V%i%j$NDj5A(B
 * 
 *  $B%7%9%F%`%m%0$X%a%C%;!<%8$r=PNO$9$k$?$a$N4X?t$NDj5A!%(BUNIX $B$N%$%s%?%U%'!<(B
 *  $B%9$H6a$$%$%s%?%U%'!<%9$K$7$F$"$k$,!$:#$N$H$3$m(B facility $B$O;H$C$F$$$J$$!%(B
 *
 *  $B%+!<%M%kMQ$N(B setlogmask $B$OMQ0U$7$F$$$J$$(B ($B:n$k$N$O4JC1(B)$B!%(B
 */

extern void	syslog(int priority, const char *format, ...);
extern int	setlogmask(int maskpri);

/*
 *  facility $B$NDj5A(B
 */
#define LOG_KERN	(0 << 3)	/* $B%+!<%M%k(B */
#define LOG_NKERN	(1 << 3)	/* $B303K(B */
#define LOG_INDP	(2 << 3)	/* $B%?%9%/FHN)It(B */
#define LOG_EXTSVC	(3 << 3)	/* $B3HD%(BSVC$B%O%s%I%i(B */
#define LOG_STASK	(4 << 3)	/* $B%7%9%F%`%?%9%/(B */
#define LOG_UTASK	(5 << 3)	/* $B%f!<%6%?%9%/(B */

#define LOG_NFACILITIES	16		/* facility $B$N?t(B */
#define LOG_FACMASK	0x0078		/* facility $B$r<h$j=P$9;~$N%^%9%/CM(B */

/*
 *  priority $B$NDj5A(B
 */
#define LOG_EMERG	0		/* $B%7%9%F%`%@%&%s$KCM$9$k%(%i!<(B */
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3		/* $B%7%9%F%`%(%i!<(B */
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7		/* $B%G%P%C%0MQ%a%C%;!<%8(B */

#define LOG_PRIMASK	0x0007		/* priority $B$r<h$j=P$9%^%9%/CM(B */

/*
 *  setlogmask $B$N0z?t$r:n$k$?$a$N%^%/%m(B
 */
#define LOG_MASK(pri)	(1 << (pri))
#define LOG_UPTO(pri)	((1 << ((pri) + 1)) - 1)

#endif /* _ITIS_SYSLOG_ */
