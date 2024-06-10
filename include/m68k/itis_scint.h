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
 *  @(#) $Id: itis_scint.h,v 1.2 1998/07/06 11:04:44 hiro Exp $
 */

#ifndef	_ITIS_SCINT_
#define	_ITIS_SCINT_

/*
 *  CPU依存に決まるシステムコール名/パラメータ名の設定
 */
#define chg_iXX	chg_ipm
#define ref_iXX	ref_ipm
#define iXXXX	imask
#define p_iXXXX	p_imask

#ifdef LINK_KERNEL
#define chg_ipm	i_chg_ipm
#define ref_ipm	i_ref_ipm
#endif /* LINK_KERNEL */

/*
 *  ソフトウェア割込みによるシステムコールインタフェースの定義
 */
#ifndef LINK_KERNEL

#define REGISTER(reg)	register INT reg asm(#reg)

#define SYS_CAL_0(FNCD) \
	REGISTER(d0) = FNCD;						\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0)							\
	  : "d0", "d1", "a0", "a1");					\
	return(d0)

#define SYS_CAL_1(FNCD, PAR1) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1)						\
	  : "d0", "d1", "a0", "a1");					\
	return(d0)

#define SYS_CAL_2(FNCD, PAR1, PAR2) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2)					\
	  : "d0", "d1", "a0", "a1");					\
	return(d0)

#define SYS_CAL_3(FNCD, PAR1, PAR2, PAR3) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	REGISTER(d3) = (INT) PAR3;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3)				\
	  : "d0", "d1", "a0", "a1");					\
	return(d0)

#define SYS_CAL_4(FNCD, PAR1, PAR2, PAR3, PAR4) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	REGISTER(d3) = (INT) PAR3;					\
	REGISTER(d4) = (INT) PAR4;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3), "g"(d4)			\
	  : "d0", "d1", "a0", "a1");					\
	return(d0)

#define SYS_CAL_5(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	REGISTER(d3) = (INT) PAR3;					\
	REGISTER(d4) = (INT) PAR4;					\
	REGISTER(d5) = (INT) PAR5;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3), "g"(d4), "g"(d5)	\
	  : "d0", "d1", "a0", "a1");					\
	return(d0)

#define SYS_CAL_6(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5, PAR6) \
	REGISTER(d0) = FNCD;						  \
	REGISTER(d1) = (INT) PAR1;					  \
	REGISTER(d2) = (INT) PAR2;					  \
	REGISTER(d3) = (INT) PAR3;					  \
	REGISTER(d4) = (INT) PAR4;					  \
	REGISTER(d5) = (INT) PAR5;					  \
	REGISTER(d6) = (INT) PAR6;					  \
	Asm("trap #1"							  \
	  : "g="(d0)							  \
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3), "g"(d4), "g"(d5), "g"(d6) \
	  : "d0", "d1", "a0", "a1");					  \
	return(d0)

#define SYS_CAL_0M(FNCD) \
	REGISTER(d0) = FNCD;						\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0)							\
	  : "d0", "d1", "a0", "a1", "memory");				\
	return(d0)

#define SYS_CAL_1M(FNCD, PAR1) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1)						\
	  : "d0", "d1", "a0", "a1", "memory");				\
	return(d0)

#define SYS_CAL_2M(FNCD, PAR1, PAR2) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2)					\
	  : "d0", "d1", "a0", "a1", "memory");				\
	return(d0)

#define SYS_CAL_3M(FNCD, PAR1, PAR2, PAR3) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	REGISTER(d3) = (INT) PAR3;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3)				\
	  : "d0", "d1", "a0", "a1", "memory");				\
	return(d0)

#define SYS_CAL_4M(FNCD, PAR1, PAR2, PAR3, PAR4) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	REGISTER(d3) = (INT) PAR3;					\
	REGISTER(d4) = (INT) PAR4;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3), "g"(d4)			\
	  : "d0", "d1", "a0", "a1", "memory");				\
	return(d0)

#define SYS_CAL_5M(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5) \
	REGISTER(d0) = FNCD;						\
	REGISTER(d1) = (INT) PAR1;					\
	REGISTER(d2) = (INT) PAR2;					\
	REGISTER(d3) = (INT) PAR3;					\
	REGISTER(d4) = (INT) PAR4;					\
	REGISTER(d5) = (INT) PAR5;					\
	Asm("trap #1"							\
	  : "g="(d0)							\
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3), "g"(d4), "g"(d5)	\
	  : "d0", "d1", "a0", "a1", "memory");				\
	return(d0)

#define SYS_CAL_6M(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5, PAR6) \
	REGISTER(d0) = FNCD;						  \
	REGISTER(d1) = (INT) PAR1;					  \
	REGISTER(d2) = (INT) PAR2;					  \
	REGISTER(d3) = (INT) PAR3;					  \
	REGISTER(d4) = (INT) PAR4;					  \
	REGISTER(d5) = (INT) PAR5;					  \
	REGISTER(d6) = (INT) PAR6;					  \
	Asm("trap #1"							  \
	  : "g="(d0)							  \
	  : "0"(d0), "g"(d1), "g"(d2), "g"(d3), "g"(d4), "g"(d5), "g"(d6) \
	  : "d0", "d1", "a0", "a1", "memory");				  \
	return(d0)

#define SYS_CAL_0V(FNCD) \
	REGISTER(d0) = FNCD;						\
	Asm("trap #1"							\
	  : /* not output */						\
	  : "g"(d0)							\
	  : "d0", "d1", "a0", "a1");					\
	return

#define SYS_CAL_1V(FNCD, PAR1) \
	REGISTER(d0) = FNCD;						\
	REGISTER(r1) = (INT) PAR1;					\
	Asm("trap #1"							\
	  : /* not output */						\
	  : "g"(d0), "g"(d1)						\
	  : "d0", "d1", "a0", "a1");					\
	return

#endif /* LINK_KERNEL */
#endif /* _ITIS_SCINT_ */
