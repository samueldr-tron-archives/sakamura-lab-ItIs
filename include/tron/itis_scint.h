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
 *  @(#) $Id: itis_scint.h,v 1.5 1998/01/30 09:54:45 hiro Exp $
 */

#ifndef	_ITIS_SCINT_
#define	_ITIS_SCINT_

/*
 *  CPU依存のシステムコール名/パラメータ名の設定
 */
#define chg_iXX	chg_ims
#define ref_iXX	ref_ims
#define iXXXX	imask
#define p_iXXXX	p_imask

#ifdef LINK_KERNEL
#define chg_ims	i_chg_ims
#define ref_ims	i_ref_ims
#endif /* LINK_KERNEL */

/*
 *  ソフトウェア割込みによるシステムコールインタフェースの定義
 */
#ifndef LINK_KERNEL

#define REGISTER(reg)	register INT reg asm(#reg)

#define SYS_CAL_0(FNCD) \
	REGISTER(r0);							\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "g"(r6)							\
	  : "r1", "r2", "r3", "r4", "r5", "r6");			\
	return(r0)

#define SYS_CAL_1(FNCD, PAR1) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r6)						\
	  : "r1", "r2", "r3", "r4", "r5", "r6");			\
	return(r0)

#define SYS_CAL_2(FNCD, PAR1, PAR2) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r6)					\
	  : "r1", "r2", "r3", "r4", "r5", "r6");			\
	return(r0)

#define SYS_CAL_3(FNCD, PAR1, PAR2, PAR3) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r2) = (INT) PAR3;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r6)				\
	  : "r1", "r2", "r3", "r4", "r5", "r6");			\
	return(r0)

#define SYS_CAL_4(FNCD, PAR1, PAR2, PAR3, PAR4) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r2) = (INT) PAR3;					\
	REGISTER(r3) = (INT) PAR4;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r3), "g"(r6)			\
	  : "r1", "r2", "r3", "r4", "r5", "r6");			\
	return(r0)

#define SYS_CAL_5(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r2) = (INT) PAR3;					\
	REGISTER(r3) = (INT) PAR4;					\
	REGISTER(r4) = (INT) PAR5;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r3), "g"(r4), "g"(r6)	\
	  : "r1", "r2", "r3", "r4", "r5", "r6");			\
	return(r0)

#define SYS_CAL_6(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5, PAR6) \
	REGISTER(r0) = (INT) PAR1;					  \
	REGISTER(r1) = (INT) PAR2;					  \
	REGISTER(r2) = (INT) PAR3;					  \
	REGISTER(r3) = (INT) PAR4;					  \
	REGISTER(r4) = (INT) PAR5;					  \
	REGISTER(r5) = (INT) PAR6;					  \
	REGISTER(r6) = FNCD;						  \
	Asm("trapa #1"							  \
	  : "g="(r0)							  \
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r3), "g"(r4), "g"(r5), "g"(r6) \
	  : "r1", "r2", "r3", "r4", "r5", "r6");			  \
	return(r0)

#define SYS_CAL_0M(FNCD) \
	REGISTER(r0);							\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "g"(r6)							\
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		\
	return(r0)

#define SYS_CAL_1M(FNCD, PAR1) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r6)						\
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		\
	return(r0)

#define SYS_CAL_2M(FNCD, PAR1, PAR2) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r6)					\
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		\
	return(r0)

#define SYS_CAL_3M(FNCD, PAR1, PAR2, PAR3) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r2) = (INT) PAR3;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r6)				\
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		\
	return(r0)

#define SYS_CAL_4M(FNCD, PAR1, PAR2, PAR3, PAR4) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r2) = (INT) PAR3;					\
	REGISTER(r3) = (INT) PAR4;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r3), "g"(r6)			\
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		\
	return(r0)

#define SYS_CAL_5M(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r1) = (INT) PAR2;					\
	REGISTER(r2) = (INT) PAR3;					\
	REGISTER(r3) = (INT) PAR4;					\
	REGISTER(r4) = (INT) PAR5;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : "g="(r0)							\
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r3), "g"(r4), "g"(r6)	\
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		\
	return(r0)

#define SYS_CAL_6M(FNCD, PAR1, PAR2, PAR3, PAR4, PAR5, PAR6) \
	REGISTER(r0) = (INT) PAR1;					  \
	REGISTER(r1) = (INT) PAR2;					  \
	REGISTER(r2) = (INT) PAR3;					  \
	REGISTER(r3) = (INT) PAR4;					  \
	REGISTER(r4) = (INT) PAR5;					  \
	REGISTER(r5) = (INT) PAR6;					  \
	REGISTER(r6) = FNCD;						  \
	Asm("trapa #1"							  \
	  : "g="(r0)							  \
	  : "0"(r0), "g"(r1), "g"(r2), "g"(r3), "g"(r4), "g"(r5), "g"(r6) \
	  : "r1", "r2", "r3", "r4", "r5", "r6", "memory");		  \
	return(r0)

#define SYS_CAL_0V(FNCD) \
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : /* not output */						\
	  : "g"(r6)							\
	  : "r0", "r1", "r2", "r3", "r4", "r5", "r6");			\
	return

#define SYS_CAL_1V(FNCD, PAR1) \
	REGISTER(r0) = (INT) PAR1;					\
	REGISTER(r6) = FNCD;						\
	Asm("trapa #1"							\
	  : /* not output */						\
	  : "g"(r0), "g"(r6)						\
	  : "r0", "r1", "r2", "r3", "r4", "r5", "r6");			\
	return

#endif /* LINK_KERNEL */
#endif /* _ITIS_SCINT_ */
