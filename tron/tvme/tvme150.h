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
 *  @(#) $Id: tvme150.h,v 1.4 1998/01/30 09:57:28 hiro Exp $
 */

#ifndef _TVME150_
#define _TVME150_

/*
 *  TVME-150 CPUボードのハードウェア資源の定義
 */

typedef	unsigned char	byte;
typedef volatile byte	IOREG;		/* I/Oレジスタの型 */

/*
 *  低速の I/Oデバイスを操作するための関数
 */

Inline void
cpu_wait_io(void)
{
	int	i;

	for (i = 0; i < 10; i++);
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
 *  Interrupt Handler (SCB68155) のレジスタ
 */
#define	SCB_R0		((IOREG *) 0xfffb0043)
#define	SCB_R1		((IOREG *) 0xfffb0047)
#define	SCB_R2		((IOREG *) 0xfffb004b)
#define	SCB_R3		((IOREG *) 0xfffb004f)
#define	SCB_R4		((IOREG *) 0xfffb0053)
#define	SCB_R5		((IOREG *) 0xfffb0057)
#define	SCB_R6		((IOREG *) 0xfffb005b)
#define	SCB_R7		((IOREG *) 0xfffb005f)

#define	LRQ1_BIT	0x40		/* ローカル割込み 1 */
#define	LRQ2_BIT	0x20		/* ローカル割込み 2 */
#define	LRQ3_BIT	0x10		/* ローカル割込み 3 */
#define	LRQ4_BIT	0x08		/* ローカル割込み 4 */
#define	LRQ5_BIT	0x04		/* ローカル割込み 5 */
#define	LRQ6_BIT	0x02		/* ローカル割込み 6 */

#define INT_VECTOR_BASE	0x80		/* 割込みのベクトル番号 */
#define	INT_VECTOR(n)	(INT_VECTOR_BASE + 7 - (n))

Inline void
cpu_wait_scb(void)
{
	Asm("nop; nop");
}

Inline void
scb_assign(IOREG *addr, byte val)
{
	*addr = val;
	cpu_wait_scb();
}

Inline void
scb_or_assign(IOREG *addr, byte val)
{
	byte	n;

	n = *addr;
	cpu_wait_scb();
	*addr = n | val;
	cpu_wait_scb();
}

Inline void
scb_and_assign(IOREG *addr, byte val)
{
	byte	n;

	n = *addr;
	cpu_wait_scb();
	*addr = n & val;
	cpu_wait_scb();
}

/*
 *  CIO (Z8536A) のレジスタ
 */
#define	CIO_PORTC	((IOREG *) 0xfffb0000)
#define	CIO_PORTB	((IOREG *) 0xfffb0004)
#define	CIO_PORTA	((IOREG *) 0xfffb0008)
#define	CIO_CNTRL	((IOREG *) 0xfffb000c)

#define	CIOA_MICR	0x00		/* マスタ制御レジスタ */
#define	CIOA_MCCR	0x01

#define	CIOA_CTCSR1	0x0a		/* クロック/タイマ1 の制御レジスタ */
#define	CIOA_CTCCR1H	0x10
#define	CIOA_CTCCR1L	0x11
#define	CIOA_CTTCR1H	0x16
#define	CIOA_CTTCR1L	0x17
#define	CIOA_CTMSR1	0x1c

Inline byte
cio_read(int reg)
{
	*CIO_CNTRL = reg;
	return(*CIO_CNTRL);
}

Inline void
cio_write(int reg, int val)
{
	*CIO_CNTRL = reg;
	*CIO_CNTRL = val;
}

/*
 *  SCC (Z8530) のレジスタ
 */
#define	SCC_CNTRLB	((IOREG *) 0xfffb0010)
#define	SCC_DATAB	((IOREG *) 0xfffb0014)
#define	SCC_CNTRLA	((IOREG *) 0xfffb0018)
#define	SCC_DATAA	((IOREG *) 0xfffb001c)

#define	SCC_WR0		0x00		/* SCC 書き込みレジスタ */
#define	SCC_WR1		0x01
#define	SCC_WR2		0x02
#define	SCC_WR3		0x03
#define	SCC_WR4		0x04
#define	SCC_WR5		0x05
#define	SCC_WR9		0x09
#define	SCC_WR10	0x0a
#define	SCC_WR11	0x0b
#define	SCC_WR12	0x0c
#define	SCC_WR13	0x0d
#define	SCC_WR14	0x0e
#define	SCC_WR15	0x0f

#define	SCC_RR0		0x00		/* SCC 読み出しレジスタ */

Inline byte
scc_read(IOREG *addr, int reg)
{
	io_write(addr, reg);
	return(io_read(addr));
}

Inline void
scc_write(IOREG *addr, int reg, int val)
{
	io_write(addr, reg);
	io_write(addr, val);
}

/*
 *  モニタ呼び出しルーチン
 */

Inline void
tvme_exit(void)
{
	Asm("mov #-1, r0; trapa #15"
	  : /* no output */
	  : /* no input */
	  : "r0");
}

Inline int
tvme_getc(void)
{
	int	c;

	Asm("mov #1, r0; mov #2, r1; trapa #15; mov r0.b %0"
	  : "=g"(c)
	  : /* no input */
	  : "r0", "r1");
	  return(c);
}

Inline int
tvme_putc(int c)
{
	Asm("mov #3, r0; mov #2, r1; mov %0.b, r2; trapa #15"
	  : /* no output */
	  : "g"(c)
	  : "r0", "r1", "r2");
	  return(c);
}

#endif /* _TVME150_ */
