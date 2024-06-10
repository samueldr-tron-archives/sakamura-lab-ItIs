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
 *  @(#) $Id: dve68k.h,v 1.2 1998/06/18 07:44:07 hiro Exp $
 */

#ifndef _DVE68K_
#define _DVE68K_

/*
 *  DVE68K/40 CPU$B%\!<%I$N%O!<%I%&%'%";q8;$NDj5A(B
 */

typedef	unsigned char	byte;
typedef volatile byte	IOREG;		/* I/O$B%l%8%9%?$N7?(B */
typedef volatile int	LIOREG;

/*
 *  $BDcB.$N(B I/O$B%G%P%$%9$rA`:n$9$k$?$a$N4X?t(B
 */

Inline void
cpu_wait_io(void)
{
	Asm("nop");
	Asm("nop");
	Asm("nop");
	Asm("nop");
}

Inline byte
io_read(IOREG *addr)
{
	byte	val;

	val = *addr;
	cpu_wait_io();
	return(val);
}

Inline void
io_write(IOREG *addr, byte val)
{
	*addr = val;
	cpu_wait_io();
}

/*
 *  DGA-001 $B$N%l%8%9%?(B
 */

#define DGA_CSR0_1	((IOREG *) 0xfff44001)
#define DGA_CSR1	((LIOREG *) 0xfff44004)
#define DGA_CSR3	((LIOREG *) 0xfff4400c)
#define DGA_CSR4	((LIOREG *) 0xfff44010)
#define DGA_CSR5	((LIOREG *) 0xfff44014)
#define DGA_CSR12	((LIOREG *) 0xfff44030)
#define DGA_CSR13	((LIOREG *) 0xfff44034)
#define	DGA_CSR19	((LIOREG *) 0xfff4404c)
#define	DGA_CSR20	((LIOREG *) 0xfff44050)
#define	DGA_CSR21	((LIOREG *) 0xfff44054)
#define DGA_CSR23	((LIOREG *) 0xfff4405c)
#define DGA_CSR24	((LIOREG *) 0xfff44060)
#define	DGA_CSR25	((LIOREG *) 0xfff44064)
#define	DGA_IFR0	((LIOREG *) 0xfff44070)
#define DGA_IFR3	((LIOREG *) 0xfff4407c)

/*
 *  DGA $B$N3d9~$_%l%Y%k@_Dj$N$?$a$NDj5A(B
 */

#define	IRQ_NMI		(0x7)		/* $B%N%s%^%9%+%V%k3d9~$_(B */
#define	IRQ_LEVEL6	(0x6)		/* $B3d9~$_%l%Y%k(B6 */
#define	IRQ_LEVEL5	(0x5)		/* $B3d9~$_%l%Y%k(B5 */
#define	IRQ_LEVEL4	(0x4)		/* $B3d9~$_%l%Y%k(B4 */
#define	IRQ_LEVEL3	(0x3)		/* $B3d9~$_%l%Y%k(B3 */
#define	IRQ_LEVEL2	(0x2)		/* $B3d9~$_%l%Y%k(B2 */
#define	IRQ_LEVEL1	(0x1)		/* $B3d9~$_%l%Y%k(B1 */

#define ABTIL_BIT	(24)		/* $B%"%\!<%H3d9~$_(B */
#define SQRIL_BIT	(8)		/* SRQ $B3d9~$_(B */

#define TT0IL_BIT	(16)		/* $B%?%$%^(B0 $B3d9~$_(B */
#define GP0IL_BIT	(0)		/* $B%7%j%"%k(BI/O $B3d9~$_(B */

Inline void
dga_set_ilv(LIOREG *addr, int shift, int val)
{
	*addr = (*addr & ~(0x07 << shift)) | (val << shift);
} 

/*
 *  $B3d9~$_%Y%/%H%k$NDj5A(B
 */

#define	G0I_VEC		(0x40)		/* $B%0%k!<%W(B0 $B3d9~$_%Y%/%H%k(B */
#define	G1I_VEC		(0x48)		/* $B%0%k!<%W(B1 $B3d9~$_%Y%/%H%k(B */
#define	SWI_VEC		(0X50)		/* $B%=%U%H%&%'%"3d9~$_%Y%/%H%k(B */
#define	SPRI_VEC	(0x40)		/* $B%9%W%j%"%93d9~$_%Y%/%H%k(B */

#define ABT_VEC		(G0I_VEC + 6)	/* $B%"%\!<%H3d9~$_%Y%/%H%k(B */
#define SQR_VEC		(G0I_VEC + 2)	/* SQR $B3d9~$_%Y%/%H%k(B */
#define	TT0_VEC		(G1I_VEC + 4)	/* $B%?%$%^(B0 $B3d9~$_%Y%/%H%k(B */
#define	GP0_VEC		(G1I_VEC + 0)	/* $B%7%j%"%k(BI/O $B3d9~$_%Y%/%H%k(B */

/*
 *  $B3d9~$_@)8f%S%C%H$NDj5A(B
 */

#define ABT_BIT		(0x40000000)	/* $B%"%\!<%H3d9~$_%S%C%H(B */
#define SQR_BIT		(0x04000000)	/* SQR $B3d9~$_%S%C%H(B */
#define	TT0_BIT		(0x00100000)	/* $B%?%$%^(B0 $B3d9~$_%S%C%H(B */
#define	GP0_BIT		(0x00010000)	/* $B%7%j%"%k(BI/O $B3d9~$_%S%C%H(B */

/*
 *  MPSC ($B&L(BPD72001-11) $B$N%l%8%9%?(B
 */

#define	MPSC_DATAA	((IOREG *) 0xfff45003)
#define	MPSC_CNTRLA	((IOREG *) 0xfff45007)
#define	MPSC_DATAB	((IOREG *) 0xfff4500b)
#define	MPSC_CNTRLB	((IOREG *) 0xfff4500f)

#define	MPSC_CR0	0x00		/* MPSC $B%3%s%H%m!<%k%l%8%9%?(B */
#define	MPSC_CR1	0x01
#define	MPSC_CR2	0x02
#define	MPSC_CR3	0x03
#define	MPSC_CR4	0x04
#define	MPSC_CR5	0x05
#define	MPSC_CR10	0x0a
#define	MPSC_CR12	0x0c
#define	MPSC_CR14	0x0e
#define	MPSC_CR15	0x0f

#define	MPSC_SR0	0x00		/* MPSC $B%9%F!<%?%9%l%8%9%?(B */

Inline byte
mpsc_read(IOREG *addr, int reg)
{
	io_write(addr, reg);
	return(io_read(addr));
}

Inline void
mpsc_write(IOREG *addr, int reg, int val)
{
	io_write(addr, reg);
	io_write(addr, val);
}

Inline void
mpsc_write_brg(IOREG *addr, int reg, int val, int brg2, int brg1)
{
	io_write(addr, reg);
	io_write(addr, val);
	io_write(addr, brg2);
	io_write(addr, brg1);
	(void) io_read(addr);		/* $B%@%_!<%j!<%I(B */
}

/*
 *  $B%b%K%?8F$S=P$7%k!<%A%s(B
 */

Inline void
dve68k_exit(void)
{
	Asm("clr.l d0; trap #3"
	  : /* no output */
	  : /* no input */
	  : "d0");
}

Inline int
dve68k_getc(void)
{
	int	c;

	Asm("moveq.l #3, d0; trap #3; move.l d0, %0"
	  : "=g"(c)
	  : /* no input */
	  : "d0");
	  return(c);
}

Inline int
dve68k_putc(int c)
{
	Asm("moveq.l #5, d0; move.l %0, d1; trap #3"
	  : /* no output */
	  : "g"(c)
	  : "d0", "d1");
	  return(c);
}

#endif /* _DVE68K_ */
