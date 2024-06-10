/**
 * 
 * 	    ItIs - ITRON Implementation by Sakamura Lab
 * 
 * Copyright (C) 1989-1996 by Sakamura Lab, the University of Tokyo, JAPAN
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: mcube.h,v 1.4 1996/02/17 09:43:56 hiro Exp $
 */

#ifndef _MCUBE_
#define _MCUBE_

/*
 *  MCUBE 基本ボードのハードウェア資源の定義
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

	for (i = 0; i < 20; i++);
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
 *  IRC (MB92421) のレジスタ
 */

#define	IRC1_LMR(n)	((IOREG *) (0xffbffd3f+(n)*4))
#define	IRC1_VMR	((IOREG *) 0xffbffd5b)
#define	IRC1_VSR	((IOREG *) 0xffbffd5f)
#define	IRC1_VNR	((IOREG *) 0xffbffd63)
#define	IRC1_TMR	((IOREG *) 0xffbffd67)
#define	IRC1_IMR	((IOREG *) 0xffbffd6b)
#define	IRC1_IRR	((IOREG *) 0xffbffd6f)
#define	IRC1_BMR	((IOREG *) 0xffbffd73)
#define	IRC1_BRR	((IOREG *) 0xffbffd77)

#define	IRC2_LMR(n)	((IOREG *) (0xffbffd7f+(n)*4))
#define	IRC2_VMR	((IOREG *) 0xffbffd9b)
#define	IRC2_VSR	((IOREG *) 0xffbffd9f)
#define	IRC2_VNR	((IOREG *) 0xffbffda3)
#define	IRC2_TMR	((IOREG *) 0xffbffda7)
#define	IRC2_IMR	((IOREG *) 0xffbffdab)
#define	IRC2_IRR	((IOREG *) 0xffbffdaf)
#define	IRC2_BMR	((IOREG *) 0xffbffdb3)
#define	IRC2_BRR	((IOREG *) 0xffbffdb7)

#define	LIR0_BIT	0x01		/* ローカル割込み 0 */
#define	LIR1_BIT	0x02		/* ローカル割込み 1 */
#define	LIR2_BIT	0x04		/* ローカル割込み 2 */
#define	LIR3_BIT	0x08		/* ローカル割込み 3 */
#define	LIR4_BIT	0x10		/* ローカル割込み 4 */
#define	LIR5_BIT	0x20		/* ローカル割込み 5 */
#define	LIR6_BIT	0x40		/* ローカル割込み 6 */

#define	BIR0_BIT	0x01		/* バス割込み 0 */
#define	BIR1_BIT	0x02		/* バス割込み 1 */
#define	BIR2_BIT	0x04		/* バス割込み 2 */
#define	BIR3_BIT	0x08		/* バス割込み 3 */
#define	BIR4_BIT	0x10		/* バス割込み 4 */
#define	BIR5_BIT	0x20		/* バス割込み 5 */
#define	BIR6_BIT	0x40		/* バス割込み 6 */

#define	IRC1_VECTOR(n)	(0x40+(n))	/* レベルn の割込みのベクタ番号 */
#define IRC2_VECTOR_BASE 0x80
#define	IRC2_VECTOR(n)	(IRC2_VECTOR_BASE+(n))

Inline void
cpu_wait_irc(void)
{
	Asm("nop; nop");
}

Inline void
irc_assign(IOREG *addr, byte val)
{
	*addr = val;
	cpu_wait_irc();
}

Inline void
irc_or_assign(IOREG *addr, byte val)
{
	byte	n;

	n = *addr;
	cpu_wait_irc();
	*addr = n | val;
	cpu_wait_irc();
}

Inline void
irc_and_assign(IOREG *addr, byte val)
{
	byte	n;

	cpu_wait_irc();
	n = *addr;
	cpu_wait_irc();
	*addr = n & val;
}

Inline byte
irc_read(IOREG *addr)
{
	byte	val;

	val = *addr;
	cpu_wait_irc();
	return(val);
}

/*
 *  タイマ (MB89254) のレジスタ
 */

#define TMR_CNT0	((IOREG *) 0xffbffee3)
#define TMR_CNT1	((IOREG *) 0xffbffee7)
#define TMR_CNT2	((IOREG *) 0xffbffeeb)
#define TMR_CNTL	((IOREG *) 0xffbffeef)

Inline void
cpu_wait_tmr(void)
{
	int	i;

	for (i = 0; i < 6; i++);
}

Inline byte
tmr_read(IOREG *addr)
{
	byte	val;

	val = *addr;
	cpu_wait_tmr();
	return(val);
}

Inline void
tmr_write(IOREG *addr, byte val)
{
	*addr = val;
	cpu_wait_tmr();
}

/*
 *  SCC (Z85C30) のレジスタ
 */
#define	SCC_CNTRLB	((IOREG *) 0xffbfff03)
#define	SCC_DATAB	((IOREG *) 0xffbfff07)
#define	SCC_CNTRLA	((IOREG *) 0xffbffef3)
#define	SCC_DATAA	((IOREG *) 0xffbffef7)

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
mcube_exit(void)
{
	Asm("mov #0, r0; trapa #15"
	  : /* no output */
	  : /* no input */
	  : "r0");
}

Inline int
mcube_getc(void)
{
	int	c;

	Asm("mov #0x11, r0; trapa #15; mov r0, %0"
	  : "=g"(c)
	  : /* no input */
	  : "r0");
	  return(c);
}

Inline int
mcube_putc(int c)
{
	Asm("mov #0x12, r0; mov %0, r1; trapa #15"
	  : /* no output */
	  : "g"(c)
	  : "r0", "r1");
	  return(c);
}

#endif /* _MCUBE_ */
