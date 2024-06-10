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
 *  @(#) $Id: sys_serial.h,v 1.6 1998/01/30 09:56:43 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  H32SBC$B%7%9%F%`(B CPU$B%\!<%IMQ(B $BDc%l%Y%k%7%j%"%k(BI/O $B4XO"$NDj5A(B
 */

#include "h32sbc.h"

/*
 *  $B%7%j%"%k%]!<%H$N%O!<%I%&%'%"0MB8>pJs$NDj5A(B
 */
typedef struct raw_serial_port_descripter {
	IOREG	*data;		/* ACI$B%G!<%?%l%8%9%?$NHVCO(B */
	IOREG	*status;	/* ACI$B%9%F!<%?%9%l%8%9%?$NHVCO(B */
	IOREG	*mode;		/* ACI$B%b!<%I%l%8%9%?$NHVCO(B */
	IOREG	*command;	/* ACI$B%3%^%s%I%l%8%9%?$NHVCO(B */

	byte	def_mode1;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B(1) */
	byte	def_mode2;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B(2) */

	byte	irc_bit;	/* IRC$B@_Dj$N%S%C%H%Q%?!<%s(B */
	byte	int_level;	/* $B3d9~$_%l%Y%k(B */
	FP	int_handler;	/* $B3d9~$_%O%s%I%i(B */
} RPORT;

/*
 *  ACI$B%3%^%s%I%l%8%9%?$N@_DjCM(B
 */
#define	COM_STOP	0x32	/* $BAw<u?.$H$bDd;_(B */
#define	COM_SSTOP	0x36	/* $BAw?.Dd;_!$<u?.$O%$%M!<%V%k(B */
#define	COM_START	0x37	/* $BAw<u?.$H$b%$%M!<%V%k(B */

/*
 *  ACI$B%b!<%I%l%8%9%?$N%G%U%)%k%H@_DjCM(B
 */
#define	DEF_MODE1	0x4d	/* 8bit, $B%9%H%C%W%S%C%H(B 1bit, $B%Q%j%F%#$J$7(B */
#define	DEF_MODE2	0x3e	/* 9600bps */

/*
 *  $B3d9~$_%Y%/%?$H%O%s%I%i%"%I%l%9$N<h$j=P$7(B
 */
#define	raw_int_vector(p)	INT_VECTOR((p)->int_level)
#define	raw_int_handler(p)	((p)->int_handler)

/*
 *  $B3d9~$_%O%s%I%i$N%(%s%H%j(B ($BA0J}@k8@(B)
 */
static void	int_handler_aci1(void);
static void	int_handler_aci0(void);

static void	serial_int_handler(int portid);

/*
 *  $BDc%l%Y%k%]!<%H>pJs4IM}%V%m%C%/$N=i4|CM(B
 */

#define NUM_PORT	2	/* $B%5%]!<%H$9$k%7%j%"%k%]!<%H$N?t(B */

#define	RAWPORT1	{ ACI0_DATA, ACI0_STATUS, ACI0_MODE, ACI0_COMMAND, \
			  DEF_MODE1, DEF_MODE2,				   \
			  LIR2_BIT, 4, int_handler_aci0 }
#define	RAWPORT2	{ ACI1_DATA, ACI1_STATUS, ACI1_MODE, ACI1_COMMAND, \
			  DEF_MODE1, DEF_MODE2,				   \
			  LIR3_BIT, 3, int_handler_aci1 }

/*
 *  $B%7%j%"%k(B I/O $B%]!<%H$N=i4|2=(B
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  ACI $B$N@_Dj(B
	 */
	*(p->command) = COM_STOP;
	n = *(p->command);		/* $B%@%_!<%j!<%I(B */
	*(p->mode) = p->def_mode1;
	*(p->mode) = p->def_mode2;
	*(p->command) = COM_SSTOP;	/* $B<u?.$N$_%$%M!<%V%k(B */

	/*
	 *  IRC $B$N@_Dj(B
	 */
	irc_assign(IRC_LMR(p->int_level), p->irc_bit);
	irc_or_assign(IRC_VMR, p->irc_bit);
	irc_and_assign(IRC_TMR, ~(p->irc_bit));	/* $B%l%Y%k%H%j%,%b!<%I(B */
	irc_and_assign(IRC_IMR, ~(p->irc_bit));	/* $B3d9~$_%^%9%/2r=|(B */

	if ((n = irc_read(IRC_TMR)) & p->irc_bit) {
		/*
		 *  IRC $B$N=i4|2=$K<:GT(B
		 *
		 *  TMR ($B%H%j%,%b!<%I%l%8%9%?(B) $B$NFI$_=P$7$K<:GT$7!$$=$N(B
		 *  $B7k2L@5$7$/3d9~$_$,$+$+$i$J$/$"$k$H$$$&>c32$,=P$k$3(B
		 *  $B$H$,B?$$!%$3$N>c32$O%W%m%0%i%`$NCV$+$l$k0LCV$K0MB8(B
		 *  $B$7$F5/$3$j!$:F8=@-$b$"$k!%860x$OITL@!%(B
		 */
		syslog(LOG_EMERG,
			"Serial port initialization error (%02x).", n);
		return(1);
	}
	return(0);
}

/*
 *  $B%7%j%"%k(B I/O $B%]!<%H$N%7%c%C%H%@%&%s(B
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	irc_or_assign(IRC_IMR, p->irc_bit);	/* $B3d9~$_$r%^%9%/(B */
}

/*
 *  $B3d9~$_%O%s%I%i$N%(%s%H%j(B
 */
static void
int_handler_aci0(void)
{
	int	intmask;

	irc_assign(IRC_IRR, LIR2_BIT);	/* $B3d9~$_MW5a$r%/%j%"(B($BK\Mh$OITMW(B) */
	serial_int_handler(1);
}

static void
int_handler_aci1(void)
{
	irc_assign(IRC_IRR, LIR3_BIT);	/* $B3d9~$_MW5a$r%/%j%"(B($BK\Mh$OITMW(B) */
	serial_int_handler(2);
}

/*
 *  $B3d9~$_%/%j%"=hM}(B
 */
Inline void
raw_port_clear_int(RPORT *p)
{
}

/*
 *  $BJ8;z$r<u?.$7$?$+!)(B
 */
Inline BOOL
raw_port_getready(RPORT *p)
{
	return(*(p->status) & 0x02);
}

/*
 *  $BJ8;z$rAw?.$G$-$k$+!)(B
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(*(p->status) & 0x01);
}

/*
 *  $B<u?.$7$?J8;z$N<h$j=P$7(B
 */
Inline byte
raw_port_getchar(RPORT *p)
{
	return(*(p->data));
}

/*
 *  $BAw?.$9$kJ8;z$N=q$-9~$_(B
 */
Inline void
raw_port_putchar(RPORT *p, byte c)
{
	*(p->data) = c;
}

/*
 *  $BAw?.@)8f4X?t(B
 */
Inline void
raw_port_sendstart(RPORT *p)
{
	*(p->command) = COM_START;
}

Inline void
raw_port_sendstop(RPORT *p)
{
	*(p->command) = COM_SSTOP;
}

/*
 *  IRC $B$H(B ACI $B$N@_Dj>uBV$rI=<($9$k(B ($B%G%P%C%0MQ(B)$B!%(B
 */
#ifdef DEBUG

Inline void
raw_port_stat(RPORT *p)
{
	syslog(LOG_NOTICE, "IRC_VMR = %x", irc_read(IRC_VMR));
	syslog(LOG_NOTICE, "IRC_TMR = %x", irc_read(IRC_TMR));
	syslog(LOG_NOTICE, "IRC_IMR = %x", irc_read(IRC_IMR));
	syslog(LOG_NOTICE, "IRC_IRR = %x", irc_read(IRC_IRR));
	syslog(LOG_NOTICE, "ACI_COMMAND = %x", *(p->command));
	syslog(LOG_NOTICE, "ACI_MODE1 = %x", *(p->mode));
	syslog(LOG_NOTICE, "ACI_MODE2 = %x", *(p->mode));
	syslog(LOG_NOTICE, "ACI_STATUS = %x", *(p->status));
	syslog(LOG_NOTICE, "ACI_DATA = %x", *(p->data));
}

#endif DEBUG

#endif /* _SYS_SERIAL_ */
