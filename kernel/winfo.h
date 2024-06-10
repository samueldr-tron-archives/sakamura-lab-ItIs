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
 *  @(#) $Id: winfo.h,v 1.10 1998/01/30 09:52:55 hiro Exp $
 */

#ifndef _WINFO_
#define _WINFO_

/*
 *  $BF14|!&DL?.%*%V%8%'%/%HKh$KI,MW$JBT$A>pJs$NDj5A(B
 */

/*
 *  $B%$%Y%s%H%U%i%0BT$A(B (TTW_FLG)
 */
typedef struct {
	UINT	waiptn;		/* $BBT$A%S%C%H%Q%?!<%s(B */
	UINT	wfmode;		/* $BBT$A%b!<%I(B */
	UINT	*p_flgptn;	/* $BBT$A2r=|;~%S%C%H%Q%?!<%s$rF~$l$k%"%I%l%9(B */
} WINFO_FLG;

/*
 *  $B%a%$%k%\%C%/%9BT$A(B (TTW_MBX)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* $B%a%C%;!<%8%Q%1%C%H$N@hF,$rF~$l$k%"%I%l%9(B */
} WINFO_MBX;

/*
 *  $B%a%C%;!<%8%P%C%U%!<u?.(B/$BAw?.BT$A(B (TTW_MBF, TTW_SMBF)
 */
typedef struct {
	VP	msg;		/* $B<u?.%a%C%;!<%8$rF~$l$k%"%I%l%9(B */
	INT	*p_msgsz;	/* $B<u?.%a%C%;!<%8$N%5%$%:$rF~$l$k%"%I%l%9(B */
} WINFO_MBF;

typedef struct {
	VP	msg;		/* $BAw?.%a%C%;!<%8$N@hF,%"%I%l%9(B */
	INT	msgsz;		/* $BAw?.%a%C%;!<%8$N%5%$%:(B */
} WINFO_SMBF;

/*
 *  $B%i%s%G%V8F=P(B/$B<uIU(B/$B=*N;BT$A(B (TTW_CAL, TTW_ACP, TTW_RDV)
 */
typedef struct {
	UINT	calptn;		/* $B8F=PB&A*Br>r7o$rI=$9%S%C%H%Q%?!<%s(B */
	VP	msg;		/* $B%a%C%;!<%8$rF~$l$k%"%I%l%9(B */
	INT	cmsgsz;		/* $B8F=P%a%C%;!<%8$N%5%$%:(B */
	INT	*p_rmsgsz;	/* $BJVEz%a%C%;!<%8$N%5%$%:$rF~$l$k%"%I%l%9(B */
} WINFO_CAL;

typedef struct {
	UINT	acpptn;		/* $B<uIUB&A*Br>r7o$rI=$9%S%C%H%Q%?!<%s(B */
	VP	msg;		/* $B8F=P%a%C%;!<%8$rF~$l$k%"%I%l%9(B */
	RNO	*p_rdvno;	/* $B%i%s%G%VHV9f$rF~$l$k%"%I%l%9(B */
	INT	*p_cmsgsz;	/* $B8F=P%a%C%;!<%8$N%5%$%:$rF~$l$k%"%I%l%9(B */
} WINFO_ACP;

typedef struct {
	RNO	rdvno;		/* $B%i%s%G%VHV9f(B */
	VP	msg;		/* $B%a%C%;!<%8$rF~$l$k%"%I%l%9(B */
	INT	maxrmsz;	/* $BJVEz%a%C%;!<%8$N:GBgD9(B */
	INT	*p_rmsgsz;	/* $BJVEz%a%C%;!<%8$N%5%$%:$rF~$l$k%"%I%l%9(B */
} WINFO_RDV;

/*
 *  $B%?%9%/IUB0%a%$%k%\%C%/%9BT$A(B (TTW_TMB)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* $B%a%C%;!<%8%Q%1%C%H$N@hF,$rF~$l$k%"%I%l%9(B */
} WINFO_TMB;

/*
 *  $B2DJQD9%a%b%j%W!<%kBT$A(B (TTW_MPL)
 */
typedef struct {
	INT	blksz;		/* $B%a%b%j%V%m%C%/%5%$%:(B */
	VP	*p_blk;		/* $B%a%b%j%V%m%C%/$N@hF,$rF~$l$k%"%I%l%9(B */
} WINFO_MPL;

/*
 *  $B8GDjD9%a%b%j%W!<%kBT$A(B (TTW_MPF)
 */
typedef struct {
	VP	*p_blf;		/* $B%a%b%j%V%m%C%/$N@hF,$rF~$l$k%"%I%l%9(B */
} WINFO_MPF;

/*
 *  $B%?%9%/%3%s%H%m!<%k%V%m%C%/Cf$K;}$DBT$A>pJs$NDj5A(B
 */
typedef union {
#ifdef USE_FLG
	WINFO_FLG	flg;
#endif /* USE_FLG */
#ifdef USE_MBX
	WINFO_MBX	mbx;
#endif /* USE_MBX */
#ifdef USE_MBF
	WINFO_MBF	mbf;
	WINFO_SMBF	smbf;
#endif /* USE_MBF */
#ifdef USE_POR
	WINFO_CAL	cal;
	WINFO_ACP	acp;
	WINFO_RDV	rdv;
#endif /* USE_POR */
#ifdef USE_TASK_MAILBOX
	WINFO_TMB	tmb;
#endif /* USE_TASK_MAILBOX */
#ifdef USE_MPL
	WINFO_MPL	mpl;
#endif /* USE_MPL */
#ifdef USE_MPF
	WINFO_MPF	mpf;
#endif /* USE_MPF */
} WINFO;

/*
 *  $BBT$A;EMM9=B$BN$NDj5A(B
 */

typedef const struct {
	UINT	tskwait;			/* $BBT$AMW0x(B */
	void	(*chg_pri_hook)(TCB *, INT);	/* $B%?%9%/M%@hEYJQ99;~$N=hM}(B */
	void	(*rel_wai_hook)(TCB *);		/* $B%?%9%/BT$A2r=|;~$N=hM}(B */
} WSPEC;

#endif /* _WINFO_ */
