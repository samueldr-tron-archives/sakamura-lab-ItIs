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
 *  @(#) $Id: itron.h,v 1.11 1998/01/30 09:54:01 hiro Exp $
 */

#ifndef _ITRON_
#define _ITRON_

/*
 *  $BHFMQE*$J%G!<%?%?%$%W(B
 */

typedef	char		B;		/* $BId9fIU$-(B 8$B%S%C%H@0?t(B */
typedef	short		H;		/* $BId9fIU$-(B 16$B%S%C%H@0?t(B */
typedef	int		W;		/* $BId9fIU$-(B 32$B%S%C%H@0?t(B */
typedef	unsigned char	UB;		/* $BId9fL5$7(B 8$B%S%C%H@0?t(B */
typedef	unsigned short  UH;		/* $BId9fL5$7(B 16$B%S%C%H@0?t(B */
typedef	unsigned int	UW;		/* $BId9fL5$7(B 32$B%S%C%H@0?t(B */

typedef	char		VB;		/* $B7?$,0lDj$7$J$$(B 8$B%S%C%H$N%G!<%?(B */
typedef	short		VH;		/* $B7?$,0lDj$7$J$$(B 16$B%S%C%H$N%G!<%?(B */
typedef	int		VW;		/* $B7?$,0lDj$7$J$$(B 32$B%S%C%H$N%G!<%?(B */
typedef	void		*VP;		/* $B7?$,0lDj$7$J$$%G!<%?$X$N%]%$%s%?(B */

typedef	void		(*FP)();	/* $B%W%m%0%i%`%9%?!<%H%"%I%l%90lHL(B */

/*
 *  ITRON $B$K0MB8$7$?%G!<%?%?%$%W(B
 */

typedef int		INT;		/* $BId9fIU$-@0?t(B */
typedef unsigned int	UINT;		/* $BId9fL5$7@0?t(B */

typedef INT		BOOL;		/* $B%V!<%kCM(B */
typedef INT		FN;		/* $B5!G=%3!<%I(B */
	/* $B$3$NDj5A$O!$&L(BITRON3.0$B;EMM$K$"$C$F$$$J$$!%(B*/
typedef	INT		ID;		/* $B%*%V%8%'%/%H$N(BID$BHV9f(B */
typedef	INT		BOOL_ID;	/* $B%V!<%kCM$^$?$O(B ID$BHV9f(B */
typedef	INT		HNO;		/* $B%O%s%I%iHV9f(B */
typedef	INT		RNO;		/* $B%i%s%G%VHV9f(B */
typedef	INT		NODE;		/* $B%N!<%IHV9f(B */
typedef	UINT		ATR;		/* $B%*%V%8%'%/%H(B/$B%O%s%I%iB0@-(B */
typedef	INT		ER;		/* $B%(%i!<%3!<%I(B */
typedef	INT		PRI;		/* $B%?%9%/M%@hEY(B */
typedef	INT		TMO;		/* $B%?%$%`%"%&%H;XDj(B */
typedef long long	SYSTIME;	/* $B%7%9%F%`%/%m%C%/(B */
typedef SYSTIME		CYCTIME;	/* $B<~4|5/F0%O%s%I%i5/F0<~4|(B */
typedef SYSTIME		ALMTIME;	/* $B%"%i!<%`%O%s%I%i5/F0;~9o(B */
typedef INT 		DLYTIME;	/* $B%?%9%/CY1d;~4V(B */
/* ItIs $BFH<+$N7?(B */
typedef long long	SYSUTIME;	/* $B%7%9%F%`;~9o(B ($B@-G=I>2AMQ(B) */

/*
 *  $B6&DLDj?t(B
 */

#define NADR		((VP) -1)	/* $B%"%I%l%9$d%]%$%s%?CM$,L58z(B */
#define TRUE		1		/* $B??(B */
#define FALSE		0		/* $B56(B */

#define TA_NULL		0		/* $BFCJL$JB0@-$r;XDj$7$J$$(B */

#define TA_ASM		0x00000000	/* $B%"%;%s%V%i$K$h$k%W%m%0%i%`(B */
#define TA_HLNG		0x00000001	/* $B9b5i8@8l$K$h$k%W%m%0%i%`(B */

#define TA_TFIFO	0x00000000	/* $BBT$A%?%9%/$r(B FIFO $B$G4IM}(B */
#define TA_TPRI		0x00000001	/* $BBT$A%?%9%/$rM%@hEY=g$G4IM}(B */

#define TMO_POL		0		/* $B%]!<%j%s%0(B */
#define TMO_FEVR	(-1)		/* $B1J5WBT$A(B */

/*
 *  $B%?%9%/4IM}4X78(B
 */

/* cre_tsk */
typedef struct t_ctsk {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	tskatr;		/* $B%?%9%/B0@-(B */
	FP	task;		/* $B%?%9%/5/F0%"%I%l%9(B */
	PRI	itskpri;	/* $B%?%9%/5/F0;~M%@hEY(B */
	INT	stksz;		/* $B%f!<%6%9%?%C%/%5%$%:(B */
	/* ItIs $BFH<+$N%U%#!<%k%I(B */
	INT	sstksz;		/* $B%7%9%F%`%9%?%C%/%5%$%:(B */
} T_CTSK;

#define TA_COP0		0x00008000	/* ID=0 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP1		0x00004000	/* ID=1 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP2		0x00002000	/* ID=2 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP3		0x00001000	/* ID=3 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP4		0x00000800	/* ID=4 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP5		0x00000400	/* ID=5 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP6		0x00000200	/* ID=6 $B$N%3%W%m%;%C%5$r;HMQ(B */
#define TA_COP7		0x00000100	/* ID=7 $B$N%3%W%m%;%C%5$r;HMQ(B */

/* ItIs $BFH<+$NB0@-(B */
#define TA_SSTKSZ	0x00010000	/* $B%7%9%F%`%9%?%C%/%5%$%:$r;XDj(B */

/* ItIs $BFH<+$NB0@-(B (TRON$B;EMM%A%C%W(B) */
#define TA_RNG0		0x00000000	/* $B%j%s%0%l%Y%k(B0 $B$G<B9T(B */
#define TA_RNG1		0x01000000	/* $B%j%s%0%l%Y%k(B1 $B$G<B9T(B */
#define TA_RNG2		0x02000000	/* $B%j%s%0%l%Y%k(B2 $B$G<B9T(B */
#define TA_RNG3		0x03000000	/* $B%j%s%0%l%Y%k(B3 $B$G<B9T(B */

/* ItIs $BFH<+$NB0@-(B (68040) */
#define TA_SMODE	0x00000000	/* $B%9!<%Q%P%$%6%b!<%I$G<B9T(B */
#define TA_UMODE	0x01000000	/* $B%f!<%6%b!<%I$G<B9T(B */

#define TSK_SELF	0		/* $B<+%?%9%/;XDj(B */

#define TPRI_INI	0	/* $B%?%9%/5/F0;~M%@hEY$r;XDj(B (chg_pri) */
#define TPRI_RUN	0	/* $B<B9TCf$N:G9bM%@hEY$r;XDj(B (rot_rdq) */

/* ref_tsk */
typedef	struct t_rtsk {
	VP	exinf;		/* $B3HD%>pJs(B */
	PRI	tskpri;		/* $B8=:_$NM%@hEY(B */
	UINT	tskstat;	/* $B%?%9%/>uBV(B */
	UINT	tskwait;	/* $BBT$AMW0x(B */
	ID	wid;		/* $BBT$A%*%V%8%'%/%H(BID */
	INT	wupcnt;		/* $B5/>2MW5a%-%e!<%$%s%0?t(B */
	INT	suscnt;		/* SUSPEND$BMW5a%M%9%H?t(B */
} T_RTSK;

#define TTS_RUN		0x00000001	/* RUN */
#define TTS_RDY		0x00000002	/* READY */
#define TTS_WAI		0x00000004	/* WAIT */
#define TTS_SUS		0x00000008	/* SUSPEND */
#define TTS_WAS		0x0000000c	/* WAIT-SUSPEND */
#define TTS_DMT		0x00000010	/* DORMANT */

#define TTW_SLP		0x00000001	/* $B5/>2BT$A$K$h$kBT$A(B */
#define TTW_DLY		0x00000002	/* $B%?%9%/$NCY1d$K$h$kBT$A(B */
#define TTW_NOD		0x00000008	/* $B@\B35!G=1~EzBT$A(B */
#define TTW_FLG		0x00000010	/* $B%$%Y%s%H%U%i%0BT$A(B */
#define TTW_SEM		0x00000020	/* $B%;%^%U%)BT$A(B */
#define TTW_MBX		0x00000040	/* $B%a%$%k%\%C%/%9BT$A(B */
#define TTW_SMBF	0x00000080	/* $B%a%C%;!<%8%P%C%U%!Aw?.BT$A(B */
#define TTW_MBF		0x00000100	/* $B%a%C%;!<%8%P%C%U%!<u?.BT$A(B */
#define TTW_CAL		0x00000200	/* $B%i%s%G%V8F=PBT$A(B */
#define TTW_ACP		0x00000400	/* $B%i%s%G%V<uIUBT$A(B */
#define TTW_RDV		0x00000800	/* $B%i%s%G%V=*N;BT$A(B */
#define TTW_MPL		0x00001000	/* $B2DJQD9%a%b%j%W!<%kBT$A(B */
#define TTW_MPF		0x00002000	/* $B8GDjD9%a%b%j%W!<%kBT$A(B */
/* ItIs $BFH<+$NDj5A(B */
#define TTW_PIS		0x00010000	/* $BM%@hEY7Q>5%;%^%U%)BT$A(B */
#define TTW_TMB		0x00020000	/* $B%?%9%/IUB0%a%$%k%\%C%/%9BT$A(B */

/*
 *  $B%;%^%U%)4X78(B
 */

/* cre_sem */
typedef	struct t_csem {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	sematr;		/* $B%;%^%U%)B0@-(B */
	INT	isemcnt;	/* $B%;%^%U%)$N=i4|%+%&%s%HCM(B */
	INT	maxsem;		/* $B%;%^%U%)$N:GBg%+%&%s%HCM(B */
} T_CSEM;

/* ref_sem */
typedef	struct t_rsem {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	INT	semcnt;		/* $B8=:_$N%;%^%U%)%+%&%s%HCM(B */
} T_RSEM;

/*
 *  $B%$%Y%s%H%U%i%04X78(B
 */

/* cre_flg */
typedef	struct t_cflg {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	flgatr;		/* $B%$%Y%s%H%U%i%0B0@-(B */
	UINT	iflgptn;	/* $B%$%Y%s%H%U%i%0$N=i4|%Q%?!<%s(B */
} T_CFLG;

#define TA_WSGL		0x00000000	/* $BJ#?t%?%9%/$NBT$A$r5v$5$J$$(B */
#define TA_WMUL		0x00000008	/* $BJ#?t%?%9%/$NBT$A$r5v$9(B */

#define TWF_ANDW	0x00000000	/* AND$BBT$A(B */
#define TWF_ORW		0x00000002	/* OR$BBT$A(B */
#define TWF_CLR		0x00000001	/* $B%/%j%";XDj(B */

/* ref_flg */
typedef	struct t_rflg {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	UINT	flgptn;		/* $B8=:_$N%$%Y%s%H%U%i%0%Q%?!<%s(B */
} T_RFLG;

/*
 *  $B%a%$%k%\%C%/%94X78(B
 */

/* cre_mbx */
typedef	struct t_cmbx {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	mbxatr;		/* $B%a%$%k%\%C%/%9B0@-(B */
} T_CMBX;

#define TA_MFIFO	0x00000000	/* $B%a%C%;!<%8$r(B FIFO $B$G4IM}(B */
#define TA_MPRI		0x00000002	/* $B%a%C%;!<%8$rM%@hEY=g$G4IM}(B */

typedef struct t_msg {
	VP	msgque[1];	/* $B%a%C%;!<%8%-%e!<$N$?$a$N%(%j%"(B */
	PRI	msgpri;		/* $B%a%C%;!<%8M%@hEY(B */
	VB	msgcont[0];	/* $B%a%C%;!<%8K\BN(B */
} T_MSG;

/* ref_mbx */
typedef	struct t_rmbx {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	T_MSG	*pk_msg;	/* $B<!$K<u?.$5$l$k%a%C%;!<%8(B */
} T_RMBX;

/*
 *  $B%a%C%;!<%8%P%C%U%!4X78(B
 */

/* cre_mbf */
typedef	struct t_cmbf {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	mbfatr;		/* $B%a%C%;!<%8%P%C%U%!B0@-(B */
	INT	bufsz;		/* $B%a%C%;!<%8%P%C%U%!$N%5%$%:(B */
	INT	maxmsz;		/* $B%a%C%;!<%8$N:GBgD9(B */
} T_CMBF;

#define TMBF_OS		(-4)	/* OS $B$N%(%i!<%m%0MQ$N%a%C%;!<%8%P%C%U%!(B */
#define TMBF_DB		(-3)	/* $B%G%P%C%0MQ$N%a%C%;!<%8%P%C%U%!(B */

/* ref_mbf */
typedef struct t_rmbf {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $B<u?.BT$A%?%9%/$NM-L5(B */
	BOOL_ID	stsk;		/* $BAw?.BT$A%?%9%/$NM-L5(B */
	INT	msgsz;		/* $B<!$K<u?.$5$l$k%a%C%;!<%8$N%5%$%:(B */
	INT	frbufsz;	/* $B6u$-%P%C%U%!$N%5%$%:(B */
} T_RMBF;

/*
 *  $B%i%s%G%V!$%i%s%G%VMQ%]!<%H4X78(B
 */

/* cre_por */
typedef	struct t_cpor {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	poratr;		/* $B%]!<%HB0@-(B */
	INT	maxcmsz;	/* $B8F=P%a%C%;!<%8$N:GBgD9(B */
	INT	maxrmsz;	/* $BJVEz%a%C%;!<%8$N:GBgD9(B */
} T_CPOR;

/* ref_por */
typedef struct t_rpor {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $B8F=PBT$A%?%9%/$NM-L5(B */
	BOOL_ID	atsk;		/* $B<uIUBT$A%?%9%/$NM-L5(B */
} T_RPOR;

/*
 *  $BM%@hEY7Q>5%;%^%U%)4X78(B (ItIs $BFH<+$NDj5A(B)
 */

/* vcre_pis */
typedef	struct t_cpis {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	pisatr;		/* $BM%@hEY7Q>5%;%^%U%)B0@-(B */
} T_CPIS;

/* vref_pis */
typedef	struct t_rpis {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	BOOL_ID	pistsk;		/* $B%;%^%U%)<hF@%?%9%/$NM-L5(B */
} T_RPIS;

/*
 *  $B%?%9%/IUB0%a%$%k%\%C%/%94X78(B (ItIs $BFH<+$NDj5A(B)
 */

/* vref_tmb */
typedef	struct t_rtmb {
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	T_MSG	*pk_msg;	/* $B<!$K<u?.$5$l$k%a%C%;!<%8(B */
} T_RTMB;

/*
 *  $B3d9~$_4IM}4X78(B
 */

/* def_int */
typedef struct t_dint {
	ATR	intatr;		/* $B3d9~$_%O%s%I%iB0@-(B */
	FP	inthdr;		/* $B3d9~$_%O%s%I%i%"%I%l%9(B */
	/* ItIs $BFH<+$N%U%#!<%k%I(B (TRON$B;EMM%A%C%W(B) */
	UINT	eitatr;		/* EIT$BB0@-(B */
} T_DINT;

/* ItIs $BFH<+$NB0@-(B (TRON$B;EMM%A%C%W(B) */
#define TA_EITATR	0x00010000	/* EIT$BB0@-$r;XDj(B */

/*
 *  $B%a%b%j%W!<%k4X78(B
 */

/* cre_mpl */
typedef	struct t_cmpl {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	mplatr;		/* $B%a%b%j%W!<%kB0@-(B */
	INT	mplsz;		/* $B%a%b%j%W!<%kA4BN$N%5%$%:(B */
} T_CMPL;

/* ItIs $BFH<+$NB0@-(B */
#define TA_HFIT		0x00000000	/* half-fit $B%"%k%4%j%:%`(B */
#define TA_FFIT		0x00010000	/* first-fit $B%"%k%4%j%:%`(B */

#define TMPL_OS		(-4)		/* OS$BMQ%a%b%j%W!<%k(B */

/* ref_mpl */
typedef struct t_rmpl {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	INT	frsz;		/* $B6u$-NN0h$N9g7W%5%$%:(B */
	INT	maxsz;		/* $B:GBg$NO"B36u$-NN0h$N%5%$%:(B */
} T_RMPL;

/* cre_mpf */
typedef	struct t_cmpf {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	mpfatr;		/* $B%a%b%j%W!<%kB0@-(B */
	INT	mpfcnt;		/* $B%a%b%j%W!<%kA4BN$N%V%m%C%/?t(B */
	INT	blfsz;		/* $B8GDjD9%a%b%j%V%m%C%/$N%5%$%:(B */
} T_CMPF;

/* ref_mpf */
typedef struct t_rmpf {
	VP	exinf;		/* $B3HD%>pJs(B */
	BOOL_ID	wtsk;		/* $BBT$A%?%9%/$NM-L5(B */
	INT	frbcnt;		/* $B6u$-NN0h$N%V%m%C%/?t(B */
} T_RMPF;

/*
 *  $B;~4V4IM}4X78(B
 */

/* def_cyc */
typedef struct t_dcyc {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	cycatr;		/* $B<~4|5/F0%O%s%I%iB0@-(B */
	FP	cychdr;		/* $B<~4|5/F0%O%s%I%i%"%I%l%9(B */
	UINT	cycact;		/* $B<~4|5/F0%O%s%I%i3h@->uBV(B */
	CYCTIME	cyctim;		/* $B<~4|5/F0;~4V4V3V(B */
} T_DCYC;

#define TCY_OFF		0x00000000	/* $B<~4|5/F0%O%s%I%i$r5/F0$7$J$$(B */
#define TCY_ON		0x00000001	/* $B<~4|5/F0%O%s%I%i$r5/F0$9$k(B */
#define TCY_INI		0x00000002	/* $B<~4|$N%+%&%s%H$r=i4|2=$9$k(B */

/* ref_cyc */
typedef struct t_rcyc {
	VP	exinf;		/* $B3HD%>pJs(B */
	CYCTIME	lfttim;		/* $B<!$N%O%s%I%i5/F0$^$G$N;D$j;~4V(B */
	UINT	cycact;		/* $B<~4|5/F0%O%s%I%i3h@->uBV(B */
} T_RCYC;

/* def_alm */
typedef struct t_dalm {
	VP	exinf;		/* $B3HD%>pJs(B */
	ATR	almatr;		/* $B%"%i!<%`%O%s%I%iB0@-(B */
	FP	almhdr;		/* $B%"%i!<%`%O%s%I%i%"%I%l%9(B */
	UINT	tmmode;		/* $B5/F0;~9o;XDj%b!<%I(B */
	ALMTIME	almtim;		/* $B%O%s%I%i5/F0;~9o(B */
} T_DALM;

#define TTM_ABS		0x00000000	/* $B@dBP;~9o$G$N;XDj(B */
#define TTM_REL		0x00000001	/* $BAjBP;~9o$G$N;XDj(B */

/* ref_alm */
typedef struct t_ralm {
	VP	exinf;		/* $B3HD%>pJs(B */
	ALMTIME	lfttim;		/* $B%O%s%I%i5/F0$^$G$N;D$j;~4V(B */
} T_RALM;

/*
 *  $B%7%9%F%`4IM}4X78(B
 */

/* get_ver */
typedef struct t_ver {
	UH	maker;		/* $B%a!<%+!<(B */
	UH	id;		/* $B7A<0HV9f(B */
	UH	spver;		/* $B;EMM=q%P!<%8%g%s(B */
	UH	prver;		/* $B@=IJ%P!<%8%g%s(B */
	UH	prno[4];	/* $B@=IJ4IM}>pJs(B */
	UH	cpu;		/* CPU$B>pJs(B */
	UH	var;		/* $B%P%j%(!<%7%g%s5-=R;R(B */
} T_VER;

/* ref_sys */
typedef struct t_rsys {
	INT	sysstat;	/* $B%7%9%F%`>uBV(B */
	ID	runtskid;	/* $B<B9T>uBV$K$"$k%?%9%/$N(B ID */
	ID	schedtskid;	/* $B<B9T>uBV$K$9$Y$-%?%9%/$N(B ID */
} T_RSYS;

#define TTS_TSK		0	/* $B%?%9%/It<B9TCf(B */
#define TTS_DDSP	1	/* $B%G%#%9%Q%C%A6X;_Cf(B */
#define TTS_LOC		3	/* $B3d9~$_$*$h$S%G%#%9%Q%C%A6X;_Cf(B */
#define TTS_INDP	4	/* $B%?%9%/FHN)It<B9TCf(B */
/* ItIs $BFH<+$NDj5A(B */
#define TTS_QTSK	8	/* $B=`%?%9%/It<B9TCf(B */

/* ref_cfg */
typedef struct t_rcfg {
	/* ItIs $BFH<+$N%U%#!<%k%I(B */
} T_RCFG;

/* def_svc */
typedef struct t_dsvc {
	ATR	svcatr;		/* $B3HD%(BSVC$B%O%s%I%iB0@-(B */
	FP	svchdr;		/* $B3HD%(BSVC$B%O%s%I%i%"%I%l%9(B */
} T_DSVC;

/* def_exc */
typedef struct t_dexc {
	ATR	excatr;		/* $BNc30%O%s%I%iB0@-(B */
	FP	exchdr;		/* $BNc30%O%s%I%i%"%I%l%9(B */
} T_DEXC;

/*
 *  $B%M%C%H%o!<%/4IM}4X78(B
 */

#define TND_SELF	0	/* $B<+%N!<%I;XDj(B */
#define TND_OTHR	(-1)	/* $B%G%U%)%k%H$NB>%N!<%I;XDj(B */

#endif /* _ITRON_ */
