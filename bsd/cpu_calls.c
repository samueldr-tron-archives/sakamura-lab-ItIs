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
 *  @(#) $Id: cpu_calls.c,v 1.3 1996/02/17 09:23:31 hiro Exp $
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
	CHECK_PAR(!(in_indp() && (mask & SIGMASK_INDP) != SIGMASK_INDP));
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
