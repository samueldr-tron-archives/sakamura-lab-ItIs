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
 *  @(#) $Id: strerror.c,v 1.8 1998/01/30 09:58:31 hiro Exp $
 */

#include <itis_services.h>

/*
 *  エラーメッセージ文字列を返す関数
 */
char *
itron_strerror(ER ercd)
{
	switch (ercd) {
	case E_OK:
		return("E_OK (normal completion)");

	case E_SYS:
		return("E_SYS (system error)");
	case E_NOMEM:
		return("E_NOMEM (memory exhausted)");
	case E_NOSPT:
		return("E_NOSPT (unsupported function)");
	case E_INOSPT:
		return("E_INOSPT (unsupported function of ITRON/FILE)");
	case E_RSFN:
		return("E_RSFN (reserved function code)");
	case E_RSATR:
		return("E_RSATR (reserved attribute)");
	case E_PAR:
		return("E_PAR (parameter error)");
	case E_ID:
		return("E_ID (illegal ID number)");
	case E_NOEXS:
		return("E_NOEXS (object not exist)");
	case E_OBJ:
		return("E_OBJ (illegal object status)");
	case E_MACV:
		return("E_MACV (memory access violation)");
	case E_OACV:
		return("E_OACV (object access violation)");
	case E_CTX:
		return("E_CTX (context error)");
	case E_QOVR:
		return("E_QOVR (queuing/nesting overflow)");
	case E_DLT:
		return("E_DLT (object being waited for is deleted)");
	case E_TMOUT:
		return("E_TMOUT (polling error/time-out)");
	case E_RLWAI:
		return("E_RLWAI (forced release from wait state)");

	case EN_NOND:
		return("EN_NOND (no destination node)");
	case EN_OBJNO:
		return("EN_OBJNO (illegal object number in destination node)");
	case EN_PROTO:
		return("EN_PROTO (unsupported protocol in destination node)");
	case EN_RSFN:
		return("EN_RSFN (unsupported function in destination node)");
	case EN_COMM:
		return("EN_COMM (communication error)");
	case EN_RLWAI:
		return("EN_RLWAI (forced release in connection function)");
	case EN_PAR:
		return("EN_PAR (parameter error in request packet)");
	case EN_RPAR:
		return("EN_RPAR (parameter error in reply packet)");
	case EN_CTXID:
		return("EN_CTXID (remote access from illegal context)");
	case EN_EXEC:
		return("EN_EXEC (resource exhausted in destination node)");
	case EN_NOSPT:
		return("EN_NOSPT (unsupported connection function)");

	case EV_FULL:
		return("EV_FULL (object table full)");

	default:
		return("unknown error");
	}
}
