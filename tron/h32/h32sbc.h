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
 *  @(#) $Id: h32sbc.h,v 1.4 1996/02/17 09:43:38 hiro Exp $
 */

#ifndef _H32SBC_
#define _H32SBC_

/*
 *  H32SBCシステム CPUボードのハードウェア資源の定義
 */

typedef	unsigned char	byte;
typedef volatile byte	IOREG;		/* I/Oレジスタの型 */

/*
 *  ACI (Asynchronous Communication Interface) のレジスタ
 */

#define	ACI0_DATA	((IOREG *) 0xffef0803)
#define	ACI0_STATUS	((IOREG *) 0xffef0807)
#define	ACI0_MODE	((IOREG *) 0xffef080b)
#define	ACI0_COMMAND	((IOREG *) 0xffef080f)

#define	ACI1_DATA	((IOREG *) 0xffef0c03)
#define	ACI1_STATUS	((IOREG *) 0xffef0c07)
#define	ACI1_MODE	((IOREG *) 0xffef0c0b)
#define	ACI1_COMMAND	((IOREG *) 0xffef0c0f)

/*
 *  IRC (Interrupt Request Controller) のレジスタ
 */

#define	IRC_LMR(n)	((IOREG *) (0xffef03ff+(n)*4))
#define	IRC_VMR		((IOREG *) 0xffef041b)
#define	IRC_VSR		((IOREG *) 0xffef041f)
#define	IRC_VNR		((IOREG *) 0xffef0423)
#define	IRC_TMR		((IOREG *) 0xffef0427)
#define	IRC_IMR		((IOREG *) 0xffef042b)
#define	IRC_IRR		((IOREG *) 0xffef042f)
#define	IRC_BMR		((IOREG *) 0xffef0433)
#define	IRC_BRR		((IOREG *) 0xffef0437)

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

#define	INT_VECTOR(n)	(0x40+(n))	/* レベルn の割込みのベクタ番号 */

Inline void
cpu_wait_irc(void)
{
	int	n;

	for (n = 0; n < 20; n++);
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
 *  PI/T (Parallel Interface Timer) のレジスタ
 */

#define	PIT_PGCR	((IOREG *) 0xffef0003)
#define	PIT_PSRR	((IOREG *) 0xffef0007)
#define	PIT_PADDR	((IOREG *) 0xffef000b)
#define	PIT_PBDDR	((IOREG *) 0xffef000f)
#define	PIT_PCDDR	((IOREG *) 0xffef0013)
#define	PIT_PIVR	((IOREG *) 0xffef0017)
#define	PIT_PACR	((IOREG *) 0xffef001b)
#define	PIT_PBCR	((IOREG *) 0xffef001f)
#define	PIT_PADR	((IOREG *) 0xffef0023)
#define	PIT_PBDR	((IOREG *) 0xffef0027)
#define	PIT_PAAR	((IOREG *) 0xffef002b)
#define	PIT_PBAR	((IOREG *) 0xffef002f)
#define	PIT_PCDR	((IOREG *) 0xffef0033)
#define	PIT_PSR		((IOREG *) 0xffef0037)

#define	PIT_TCR		((IOREG *) 0xffef0043)
#define	PIT_TIVR	((IOREG *) 0xffef0047)

#define	PIT_CPRH	((IOREG *) 0xffef004f)
#define	PIT_CPRM	((IOREG *) 0xffef0053)
#define	PIT_CPRL	((IOREG *) 0xffef0057)

#define	PIT_CNTRH	((IOREG *) 0xffef005f)
#define	PIT_CNTRM	((IOREG *) 0xffef0063)
#define	PIT_CNTRL	((IOREG *) 0xffef0067)

#define	PIT_TSR		((IOREG *) 0xffef006b)

/*
 *  低速の I/Oデバイスを操作するための関数
 */

Inline void
cpu_wait(void)
{
	Asm("nop; nop; nop");
}

Inline void
io_assign(IOREG *addr, byte val)
{
	*addr = val;
	cpu_wait();
}

Inline void
io_or_assign(IOREG *addr, byte val)
{
	byte	n;

	n = *addr;
	cpu_wait();
	*addr = n | val;
	cpu_wait();
}

Inline void
io_and_assign(IOREG *addr, byte val)
{
	byte	n;

	n = *addr;
	cpu_wait();
	*addr = n & val;
	cpu_wait();
}

/*
 *  EMS モニタ呼び出しルーチン
 */

Inline void
h32_exit(void)
{
	Asm("mov #0, r0; trapa #15"
	  : /* no output */
	  : /* no input */
	  : "r0");
}

Inline int
h32_getc(void)
{
	int	c;

	Asm("mov #2, r0; trapa #15; mov r0.b %0"
	  : "=g"(c)
	  : /* no input */
	  : "r0");
	  return(c);
}

Inline int
h32_putc(int c)
{
	Asm("mov #3, r0; mov %0.b, r1; trapa #15"
	  : /* no output */
	  : "g"(c)
	  : "r0", "r1");
	  return(c);
}

#endif /* _H32SBC_ */
