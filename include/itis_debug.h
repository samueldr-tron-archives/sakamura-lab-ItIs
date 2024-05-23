/**
 * 
 * 	    ItIs - ITRON Implementation by Sakamura Lab
 * 
 * Copyright (C) 1989-1997 by Sakamura Lab, the University of Tokyo, JAPAN
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
 *  @(#) $Id: itis_debug.h,v 1.8 1998/01/30 07:05:14 hiro Exp $
 */

#ifndef _ITIS_DEBUG_
#define _ITIS_DEBUG_

/*
 *  �f�o�b�N�T�|�[�g�@�\�֌W�̒�`
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
	VW      r14;		/* R14 (�t���[���|�C���^) */
	VW      sp;		/* �X�^�b�N�|�C���^ */
} T_REGS;

typedef	struct {
	VW	pc;		/* PC (�v���O�����J�E���^) */
	VW	psw;		/* PSW */
} T_EIT;

typedef struct t_cregs {
	VW	csw;		/* CSW */
	VW	sp0;		/* SP0 (�����O0 �̃X�^�b�N�|�C���^) */
	VW	sp1;		/* SP1 (�����O1 �̃X�^�b�N�|�C���^) */
	VW	sp2;		/* SP2 (�����O2 �̃X�^�b�N�|�C���^) */
	VW	sp3;		/* SP3 (�����O3 �̃X�^�b�N�|�C���^) */
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
	VW      a6;		/* A6 (�t���[���|�C���^) */
	VW      sp;		/* �X�^�b�N�|�C���^ */
} T_REGS;

typedef	struct {
	VW	pc;		/* PC (�v���O�����J�E���^) */
	VH	sr;		/* SR */
} T_EIT;

typedef struct t_cregs {
	VW	msp;		/* MSP (�X�[�p�o�C�U�X�^�b�N�|�C���^) */
	VW	usp;		/* USP (���[�U�X�^�b�N�|�C���^) */
} T_CREGS;

#else /* mc68000 */

typedef void	T_REGS;
typedef void	T_EIT;
typedef void	T_CREGS;

#endif /* mc68000 */
#endif /* tron */

#endif /* _ITIS_DEBUG_ */
