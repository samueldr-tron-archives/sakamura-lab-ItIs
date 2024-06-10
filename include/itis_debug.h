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
 *  @(#) $Id: itis_debug.h,v 1.9 1998/01/30 09:53:58 hiro Exp $
 */

#ifndef _ITIS_DEBUG_
#define _ITIS_DEBUG_

/*
 *  デバックサポート機能関係の定義
 */

#ifdef tron

typedef struct {
	VW	r0;		/* R0 */
	VW      r1;		/* R1 */
	VW      r2;		/* R2 */
	VW      r3;		/* R3 */
	VW      r4;		/* R4 */
	VW      r5;		/* R5 */
	VW      r6;		/* R6 */
	VW      r7;		/* R7 */
	VW      r8;		/* R8 */
	VW      r9;		/* R9 */
	VW      r10;		/* R10 */
	VW      r11;		/* R11 */
	VW      r12;		/* R12 */
	VW      r13;		/* R13 */
	VW      r14;		/* R14 (フレームポインタ) */
	VW      sp;		/* スタックポインタ */
} T_REGS;

typedef	struct {
	VW	pc;		/* PC (プログラムカウンタ) */
	VW	psw;		/* PSW */
} T_EIT;

typedef struct t_cregs {
	VW	csw;		/* CSW */
	VW	sp0;		/* SP0 (リング0 のスタックポインタ) */
	VW	sp1;		/* SP1 (リング1 のスタックポインタ) */
	VW	sp2;		/* SP2 (リング2 のスタックポインタ) */
	VW	sp3;		/* SP3 (リング3 のスタックポインタ) */
	VW	uatb;		/* UATB */
	VW	lsid;		/* LSID */
} T_CREGS;

#else /* tron */
#ifdef mc68000

typedef struct {
	VW	d0;		/* D0 */
	VW      d1;		/* D1 */
	VW      d2;		/* D2 */
	VW      d3;		/* D3 */
	VW      d4;		/* D4 */
	VW      d5;		/* D5 */
	VW      d6;		/* D6 */
	VW      d7;		/* D7 */
	VW      a0;		/* A0 */
	VW      a1;		/* A1 */
	VW      a2;		/* A2 */
	VW      a3;		/* A3 */
	VW      a4;		/* A4 */
	VW      a5;		/* A5 */
	VW      a6;		/* A6 (フレームポインタ) */
	VW      sp;		/* スタックポインタ */
} T_REGS;

typedef	struct {
	VW	pc;		/* PC (プログラムカウンタ) */
	VH	sr;		/* SR */
} T_EIT;

typedef struct t_cregs {
	VW	msp;		/* MSP (スーパバイザスタックポインタ) */
	VW	usp;		/* USP (ユーザスタックポインタ) */
} T_CREGS;

#else /* mc68000 */

typedef void	T_REGS;
typedef void	T_EIT;
typedef void	T_CREGS;

#endif /* mc68000 */
#endif /* tron */

#endif /* _ITIS_DEBUG_ */
