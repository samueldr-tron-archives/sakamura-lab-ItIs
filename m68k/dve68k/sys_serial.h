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
 *  @(#) $Id: sys_serial.h,v 1.2 1998/06/18 07:44:08 hiro Exp $
 */

#ifndef _SYS_SERIAL_
#define _SYS_SERIAL_

/*
 *  DVE68K/40 CPU$B%\!<%IMQ(B $BDc%l%Y%k%7%j%"%k(BI/O $B4XO"$NDj5A(B
 */

#include "dve68k.h"

/*
 *  $B%7%j%"%k%]!<%H$N%O!<%I%&%'%"0MB8>pJs$NDj5A(B
 */
typedef struct raw_serial_port_descripter {
	INT	*initflag;	/* $B=i4|2=:Q%U%i%0$X$N%]%$%s%?(B */
	IOREG	*data;		/* $B%G!<%?%l%8%9%?$NHVCO(B */
	IOREG	*cntrl;		/* $B%3%s%H%m!<%k%l%8%9%?$NHVCO(B */
	BOOL	sendflag;	/* $BAw?.3d9~$_%$%M!<%V%k%U%i%0(B */

	byte	cr3_def;	/* $B%G%U%)!<%k%H$N@_DjCM(B (CR3) */
	byte	cr4_def;	/* $B%G%U%)!<%k%H$N@_DjCM(B (CR4) */
	byte	cr5_def;	/* $B%G%U%)!<%k%H$N@_DjCM(B (CR5) */
	byte	brg2_def;	/* $B%G%U%)!<%k%H$N@_DjCM(B ($B%\!<%l!<%H2<0L(B) */
	byte	brg1_def;	/* $B%G%U%)!<%k%H$N@_DjCM(B ($B%\!<%l!<%H>e0L(B) */
} RPORT;

/*
 *  MPSC$B%3%s%H%m!<%k%l%8%9%?$N@_DjCM(B
 */

#define MPSC_RESET	0x18		/* $B%]!<%H%j%;%C%H%3%^%s%I(B */
#define MPSC_EOI	0x38		/* EOI (End of Interrupt) */

#define CR3_DEF		0xc1		/* $B%G!<%?(B 8bit, $B<u?.%$%M!<%V%k(B */
#define CR4_DEF		0x44		/* $B%9%H%C%W%S%C%H(B 1bit, $B%Q%j%F%#$J$7(B */
#define CR5_DEF		0xea		/* $B%G!<%?(B 8bit, $BAw?.%$%M!<%V%k(B */
#define BRG2_DEF	0x1e		/* 9600bps ($B2<0L(B) */
#define BRG1_DEF	0x00		/* 9600bps ($B>e0L(B) */

#define CR10_DEF	0x00		/* NRZ */
#define CR14_DEF	0x07		/* $B%\!<%l!<%H%8%'%M%l!<%?%$%M!<%V%k(B */
#define CR15_DEF	0x56		/* $B%\!<%l!<%H%8%'%M%l!<%?;HMQ(B */

#define CR1_ALL		0x12		/* $BA43d9~$_$r5v2D(B */
#define CR1_RECV	0x10		/* $B<u?.3d9~$_$N$_5v2D(B */
#define CR1_DOWN	0x00		/* $BA43d9~$_$r6X;_(B */

/*
 *  $B3d9~$_%Y%/%?$H%O%s%I%i%"%I%l%9$N<h$j=P$7(B
 */
#define raw_int_vector(p)	GP0_VEC
#define raw_int_handler(p)	int_handler_mpsc

/*
 *  $B3d9~$_%O%s%I%i$N%(%s%H%j(B ($BA0J}@k8@(B)
 */
static void	int_handler_mpsc(void);

static void	serial_int_handler(int portid);

/*
 *  $BDc%l%Y%k%]!<%H>pJs4IM}%V%m%C%/$N=i4|CM(B
 */

#define NUM_PORT	2	/* $B%5%]!<%H$9$k%7%j%"%k%]!<%H$N?t(B */

static BOOL	initflag[2] = { 0, 0 } ;	/* $B=i4|2=:Q%U%i%0(B */

#define RAWPORT1	{ &initflag[0], MPSC_DATAA, MPSC_CNTRLA, 0,	\
			  CR3_DEF, CR4_DEF, CR5_DEF, BRG2_DEF, BRG1_DEF }
#define RAWPORT2	{ &initflag[1], MPSC_DATAB, MPSC_CNTRLB, 0,	\
			  CR3_DEF, CR4_DEF, CR5_DEF, BRG2_DEF, BRG1_DEF }

/*
 *  $B%7%j%"%k(B I/O $B%]!<%H$N=i4|2=(B
 */
Inline BOOL
raw_port_init(RPORT *p)
{
	byte	n;

	/*
	 *  MPSC $B$N@_Dj(B
	 */
	io_write(p->cntrl, MPSC_RESET);
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		mpsc_write(MPSC_CNTRLA, MPSC_CR2, 0x18);
		mpsc_write(MPSC_CNTRLB, MPSC_CR2, 0x00);
	}
	mpsc_write(p->cntrl, MPSC_CR1, CR1_RECV);
	p->sendflag = 0;
	mpsc_write(p->cntrl, MPSC_CR4, p->cr4_def);
	mpsc_write_brg(p->cntrl, MPSC_CR12, 0x01, p->brg2_def, p->brg1_def);
	mpsc_write_brg(p->cntrl, MPSC_CR12, 0x02, p->brg2_def, p->brg1_def);
	mpsc_write(p->cntrl, MPSC_CR15, CR15_DEF);
	mpsc_write(p->cntrl, MPSC_CR14, CR14_DEF);
	mpsc_write(p->cntrl, MPSC_CR10, CR10_DEF);
	mpsc_write(p->cntrl, MPSC_CR3, p->cr3_def);
	mpsc_write(p->cntrl, MPSC_CR5, p->cr5_def);

	/*
	 *  $B3d9~$_4XO"$N@_Dj(B
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		dga_set_ilv(DGA_CSR25, GP0IL_BIT, IRQ_LEVEL6);
						/* $B3d9~$_%l%Y%k@_Dj(B */
		*DGA_CSR21 |= GP0_BIT;		/* $B3d9~$_%^%9%/2r=|(B */
	}

	*(p->initflag) = 1;			/* $B=i4|2=%U%i%0@_Dj(B */
	return(0);
}

/*
 *  $B%7%j%"%k(B I/O $B%]!<%H$N%7%c%C%H%@%&%s(B
 */
Inline void
raw_port_shutdown(RPORT *p)
{
	*(p->initflag) = -1;			/* $B=i4|2=%U%i%0@_Dj(B */

	/*
	 *  MPSC $B$N3d9~$_4X78$N@_Dj(B
	 */
	mpsc_write(p->cntrl, MPSC_CR1, CR1_DOWN);
	p->sendflag = 0;

	/*
	 *  $B3d9~$_4XO"$N@_Dj(B
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		*DGA_CSR21 &= ~GP0_BIT;		/* $B3d9~$_%^%9%/@_Dj(B */
	}
}

/*
 *  $B3d9~$_%O%s%I%i$N%(%s%H%j(B
 */
static void
int_handler_mpsc(void)
{
	if (initflag[0] > 0) {
		serial_int_handler(1);
	}
	if (initflag[1] > 0) {
		serial_int_handler(2);
	}
	mpsc_write(MPSC_CNTRLA, MPSC_CR0, MPSC_EOI);
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
	return(mpsc_read(p->cntrl, MPSC_SR0) & 0x01);
}

/*
 *  $BJ8;z$rAw?.$G$-$k$+!)(B
 */
Inline BOOL
raw_port_putready(RPORT *p)
{
	return(mpsc_read(p->cntrl, MPSC_SR0) & 0x04);
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
	if (!(p->sendflag)) {
		mpsc_write(p->cntrl, MPSC_CR1, CR1_ALL);
		p->sendflag = 1;
	}
}

Inline void
raw_port_sendstop(RPORT *p)
{
	if (p->sendflag) {
		mpsc_write(p->cntrl, MPSC_CR1, CR1_RECV);
		p->sendflag = 0;
	}
}

#endif /* _SYS_SERIAL_ */
