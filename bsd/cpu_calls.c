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
 *  @(#) $Id: cpu_calls.c,v 1.5 1998/07/06 06:29:28 hiro Exp $
 */

/* 
 *  CPU に依存するシステムコール
 */

#include "itis_kernel.h"
#include "task.h"
#include "check.h"

/*
 *  ディスパッチの禁止/許可機能
 */

#ifndef _i_dis_dsp

SYSCALL ER
i_dis_dsp(void)
{
	CHECK_CTX(!in_loc());
	sigsetmask(SIGMASK_DDSP);
	return(E_OK);
}

#endif /* _i_dis_dsp */
#ifndef _i_ena_dsp

SYSCALL ER
i_ena_dsp(void)
{
	CHECK_CTX(!in_loc());
	sigsetmask(SIGMASK_TASK);
	return(E_OK);
}

#endif /* _i_ena_dsp */

/*
 *  割込みの禁止/許可機能
 */

#ifndef _i_loc_cpu

SYSCALL ER
i_loc_cpu(void)
{
	CHECK_INTSK();
	sigsetmask(SIGMASK_LOC);
	return(E_OK);
}

#endif /* _i_loc_cpu */
#ifndef _i_unl_cpu

SYSCALL ER
i_unl_cpu(void)
{
	CHECK_INTSK();
	sigsetmask(SIGMASK_TASK);
	return(E_OK);
}

#endif /* _i_unl_cpu */

/*
 *  IMASK の設定/参照機能
 */

#ifndef _i_chg_ims

SYSCALL ER
i_chg_ims(UINT mask)
{
	if (in_indp()) {
		CHECK_PAR((mask & SIGMASK_INDP) == SIGMASK_INDP);
	}
	else if (mask & ~SIGMASK_DDSP) {
		CHECK_PAR((mask & SIGMASK_DDSP) == SIGMASK_DDSP);
	}
	sigsetmask(mask);
	return(E_OK);
}

#endif /* _i_chg_ims */
#ifndef _i_ref_ims

SYSCALL ER
i_ref_ims(UINT *p_mask)
{
	*p_mask = sigblock(0);
	return(E_OK);
}

#endif /* _i_ref_ims */
