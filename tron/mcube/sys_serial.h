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
 *  @(#) $Id: sys_serial.h,v 1.5 1998/01/30 09:57:04 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  MCUBE$BMQ(B $BDc%l%Y%k%7%j%"%k(BI/O $B4XO"$NDj5A(B
 */

#include "mcube.h"

/*
 *  $B%7%j%"%k%]!<%H$N%O!<%I%&%'%"0MB8>pJs$NDj5A(B
 */
typedef struct raw_serial_port_descripter {
	BOOL	*initflag;	/* $B=i4|2=:Q%U%i%0$X$N%]%$%s%?(B */
	IOREG	*data;		/* $B%G!<%?%l%8%9%?$NHVCO(B */
	IOREG	*cntrl;		/* $B%3%s%H%m!<%k%l%8%9%?$NHVCO(B */

	byte	com_reset;	/* $B%]!<%H%j%;%C%H%3%^%s%I(B */
	byte	mode3_def;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B (WR3) */
	byte	mode4_def;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B (WR4) */
	byte	mode5_def;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B (WR5) */
	byte	mode12_def;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B (WR12) */
	byte	mode13_def;	/* $B%G%U%)!<%k%H$N%b!<%I@_DjCM(B (WR13) */
} RPORT;

/*
 *  SCC$B%3%s%H%m!<%k%l%8%9%?$N@_DjCM(B
 */

#define MODE4_DEF	0x44		/* $B%9%H%C%W%S%C%H(B 1bit, $B%Q%j%F%#$J$7(B */
#define MODE3_DEF	0xc1		/* $B%G!<%?(B 8bit, $B<u?.%$%M!<%V%k(B */
#define MODE3_INIT_MASK	0xfe		/* $B<u?.%G%#%9%(!<%V%k(B */
#define MODE5_DEF	0xea		/* $B%G!<%?(B 8bit, $BAw?.%$%M!<%V%k(B */
#define MODE5_INIT_MASK	0x75		/* $BAw?.%G%#%9%(!<%V%k(B */

#define MODE10_DEF	0x00		/* NRZ */
#define MODE11_DEF	0x56		/* $B%/%m%C%/$K4X$9$k@_Dj(B */
#define MODE12_DEF	10		/* 9600bps */
#define MODE13_DEF	0		/* 9600bps */
#define MODE14_DEF	0x03		/* $B%\!<%l!<%H%8%'%M%l!<%?(B */

#define MODE1_DEF	0x13		/* $B3F3d9~$_$r5v2D(B */
#define MODE1_DOWN	0x00		/* $BA43d9~$_$r6X;_(B */
#define MODE15_DEF	0x00		/* $B$=$NB>$N3d9~$_$N6X;_(B */
#define MODE9_INIT	0x02		/* $B%^%9%?!<3d9~$_6X;_(B */
#define MODE9_DEF	0x0a		/* $B%^%9%?!<3d9~$_5v2D(B */
#define MODE9_DOWN	0x02		/* $B%^%9%?!<3d9~$_6X;_(B */

#define MODE4_KBD	0x45		/* $B%9%H%C%W%S%C%H(B 1bit, $B4q?t%Q%j%F%#(B */
#define MODE3_KBD	0xc1		/* $B%G!<%?(B 8bit, $B<u?.%$%M!<%V%k(B */
#define MODE5_KBD	0x68		/* $B%G!<%?(B 8bit, $BAw?.%$%M!<%V%k(B */
#define MODE12_KBD	4		/* 19200bps */
#define MODE13_KBD	0		/* 19200bps */

/*
 *  $B3d9~$_%Y%/%?$H%O%s%I%i%"%I%l%9$N<h$j=P$7(B
 */
#define raw_int_vector(p)	IRC1_VECTOR(3)
#define raw_int_handler(p)	int_handler_scc

/*
 *  $B3d9~$_%O%s%I%i$N%(%s%H%j(B ($BA0J}@k8@(B)
 */
static void	int_handler_scc(void);

static void	serial_int_handler(int portid);

/*
 *  $BDc%l%Y%k%]!<%H>pJs4IM}%V%m%C%/$N=i4|CM(B
 */

#define NUM_PORT	2	/* $B%5%]!<%H$9$k%7%j%"%k%]!<%H$N?t(B */

static int	initflag[2] = { -1, 0 } ;	/* $B=i4|2=:Q%U%i%0(B */

#define RAWPORT1	{ &initflag[0], SCC_DATAB, SCC_CNTRLB,		\
			  0x40, MODE3_DEF, MODE4_DEF, MODE5_DEF,	\
			  MODE12_DEF, MODE13_DEF }
#define RAWPORT2	{ &initflag[1], SCC_DATAA, SCC_CNTRLA,		\
			  0x80, MODE3_KBD, MODE4_KBD, MODE5_KBD,	\
			  MODE12_KBD, MODE13_KBD }

#define IOCTL_PORT2	{ 0, 0, 0, 0 }

/*
 *  $B%7%j%"%k(B I/O $B%]!<%H$N=i4|2=(B
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  SCC $B$N@_Dj(B
	 */
	io_write(p->cntrl, SCC_WR0);			/* WR0 $B;XDj(B */
	if (*(p->initflag) == 0) {
		scc_write(p->cntrl, SCC_WR9, p->com_reset);

		scc_write(p->cntrl, SCC_WR4, p->mode4_def);
		scc_write(p->cntrl, SCC_WR3, (p->mode3_def & MODE3_INIT_MASK));
		scc_write(p->cntrl, SCC_WR5, (p->mode5_def & MODE5_INIT_MASK));

		scc_write(p->cntrl, SCC_WR9, MODE9_INIT);
		scc_write(p->cntrl, SCC_WR10, MODE10_DEF);
		scc_write(p->cntrl, SCC_WR11, MODE11_DEF);
		scc_write(p->cntrl, SCC_WR12, p->mode12_def);
		scc_write(p->cntrl, SCC_WR13, p->mode13_def);
		scc_write(p->cntrl, SCC_WR14, MODE14_DEF);

		scc_write(p->cntrl, SCC_WR3, p->mode3_def);
		scc_write(p->cntrl, SCC_WR5, p->mode5_def);
	}

	/*
	 *  IRC $B$N@_Dj(B
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		irc_assign(IRC1_LMR(3), LIR3_BIT);
		irc_and_assign(IRC1_TMR, ~LIR3_BIT);	/* $B%l%Y%k%H%j%,(B */
		irc_and_assign(IRC1_IMR, ~LIR3_BIT);	/* $B3d9~$_%^%9%/2r=|(B */
	}

	/*
	 *  SCC $B$N3d9~$_4X78$N@_Dj(B
	 */
	scc_write(p->cntrl, SCC_WR15, MODE15_DEF);
	scc_write(p->cntrl, SCC_WR1, MODE1_DEF);
	io_write(p->cntrl, 0x28);
	scc_write(p->cntrl, SCC_WR9, MODE9_DEF);

	*(p->initflag) = 1;				/* $B=i4|2=%U%i%0@_Dj(B */
	return(0);
}

/*
 *  $B%7%j%"%k(B I/O $B%]!<%H$N%7%c%C%H%@%&%s(B
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	*(p->initflag) = -1;				/* $B=i4|2=%U%i%0@_Dj(B */

	/*
	 *  SCC $B$N3d9~$_4X78$N@_Dj(B
	 */
	scc_write(p->cntrl, SCC_WR1, MODE1_DOWN);
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		scc_write(p->cntrl, SCC_WR9, MODE9_DOWN);
	}

	/*
	 *  IRC $B$N@_Dj(B
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		irc_or_assign(IRC1_IMR, LIR3_BIT);	/* $B3d9~$_%^%9%/@_Dj(B */
	}
}

/*
 *  $B3d9~$_%O%s%I%i$N%(%s%H%j(B
 */
static void
int_handler_scc(void)
{
	if (initflag[0] > 0) {
		serial_int_handler(1);
	}
	if (initflag[1] > 0) {
		serial_int_handler(2);
	}
}

/*
 *  $B3d9~$_%/%j%"=hM}(B
 */
Inline void
raw_port_clear_int(RPORT *p)
{
	io_write(p->cntrl, 0x10);
}

/*
 *  $BJ8;z$r<u?.$7$?$+!)(B
 */
Inline BOOL
raw_port_getready(RPORT *p)
{
	return(scc_read(p->cntrl, SCC_RR0) & 0x01);
}

/*
 *  $BJ8;z$rAw?.$G$-$k$+!)(B
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(scc_read(p->cntrl, SCC_RR0) & 0x04);
}

/*
 *  $B<u?.$7$?J8;z$N<h$j=P$7(B
 */
Inline byte
raw_port_getchar(RPORT *p)
{
	return(io_read(p->data));
}

/*
 *  $BAw?.$9$kJ8;z$N=q$-9~$_(B
 */
Inline void
raw_port_putchar(RPORT *p, byte c)
{
	io_write(p->data, c);
}

/*
 *  $BAw?.@)8f4X?t(B
 */
Inline void
raw_port_sendstart(RPORT *p)
{
}

Inline void
raw_port_sendstop(RPORT *p)
{
	io_write(p->cntrl, 0x28);
}

#endif /* _SYS_SERIAL_ */
