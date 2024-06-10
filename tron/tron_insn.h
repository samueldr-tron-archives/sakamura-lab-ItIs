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
 *  @(#) $Id: tron_insn.h,v 1.5 1998/01/30 09:56:33 hiro Exp $
 */

#ifndef	_TRON_INSN_
#define	_TRON_INSN_

/*
 *  $B@)8f%l%8%9%?$NA`:n4X?t(B
 */

/*
 *  PSW $B$N8=:_CM$NFI$_=P$7(B
 */
Inline VW
current_psw(void)
{
	VW	psw;

	Asm("stc %1, %0" : "=g"(psw) : "m"(CREG(PSW)));
	return(psw);
}

/*
 *  PSW $B$N8=:_CM$NJQ99(B
 */
Inline void
set_psw(VW psw)
{
	Asm("ldc %0, %1" : : "g"(psw), "m"(CREG(PSW)));
}

/*
 *  IMASK $B$N8=:_CM$NFI$_=P$7(B
 *
 *  IMASK $B$NCM$H$7$F!$(B0 $B0J>e(B 15 $B0J2<$NCM$,JV$k!%(B
 */
#define imask_psw(psw)	((psw >> 16) & 15)
#define current_imask() (imask_psw(current_psw()))

/*
 *  IMASK $B$N8=:_CM$NJQ99(B
 *
 *  imask $B$O!$(B0 $B0J>e(B 15 $B0J2<$G$"$k$3$H!%(B
 */
Inline void
set_imask(INT imask)
{
	Asm("ldc %0, %1" : : "g"(imask << 16), "m"(CREG(IMASK)));
}

/*
 *  $B3d9~$_%^%9%/>uBV$rFbItI=8=$GJV$9!%(B
 */
Inline INT
current_intmask(void)
{
	INT	intmask;

	Asm("stc %1, %0" : "=g"(intmask) : "m"(CREG(IMASK)));
	return(intmask);
}

/*
 *  NMI $B$r=|$/$9$Y$F$N3d9~$_$r6X;_$9$k!%(B
 */
Inline void
disint(void)
{
	Asm("ldc #0, %0" : : "m"(CREG(IMASK)));
}

/*
 *  current_intmask $B$,JV$7$?3d9~$_%^%9%/>uBV$rEO$7$F!$3d9~$_6X;_A0$N>u(B
 *  $BBV$KLa$9!%(B
 *
 *  clobber$B%j%9%H$K(B memory $B$rF~$l$F$$$k$N$O!$(Benaint $B$ND>8e$K%?%9%/%G%#(B
 *  $B%9%Q%C%A$,5/$3$j!$M=4|$7$J$$%a%b%j%(%j%"$,=q$-49$($i$l$k2DG=@-$,$"(B
 *  $B$k$3$H$r%3%s%Q%$%i$KCN$i$;$k$?$a$G$"$k!%6qBNE*$K$O!$%(%i!<%3!<%I$r(B
 *  $BF~$l$kJQ?t$,LdBj$K$J$k!%(B
 */
Inline void
enaint(INT intmask)
{
	Asm("ldc %0, %1" : : "g"(intmask), "m"(CREG(IMASK)) : "memory");
}

/*
 *  DIR ($BCY1d3d9~$_MW5a%l%8%9%?(B) $B$N8=:_CM$NFI$_=P$7(B
 */
Inline VW
current_dir(void)
{
	VW	dir;

	Asm("stc %1, %0" : "=g"(dir) : "m"(CREG(DIR)));
	return(dir);
}

/*
 *  DIR ($BCY1d3d9~$_MW5a%l%8%9%?(B) $B$N8=:_CM$NJQ99(B
 *
 *  dir $B$O!$(B0 $B0J>e(B 15 $B0J2<$G$"$k$3$H!%(B
 */
Inline void
set_dir(INT dir)
{
	Asm("ldc %0, %1" : : "g"(dir), "m"(CREG(DIR)));
}

/*
 *  EIT$B4XO"$NDj5A(B
 */

#define EITVEC_BUSERR	0x11		/* $B%P%9%(%i!<$N%Y%/%?HV9f(B */
#define EITVEC_TRAPA1	0x21		/* TRAPA #1 $B$N%Y%/%?HV9f(B */
#define EITVEC_TRAPA2	0x22		/* TRAPA #2 $B$N%Y%/%?HV9f(B */
#define EITVEC_TRAPA3	0x23		/* TRAPA #3 $B$N%Y%/%?HV9f(B */
#define EITVEC_TRAPA4	0x24		/* TRAPA #4 $B$N%Y%/%?HV9f(B */
#define EITVEC_DI14	0x5e		/* DI=14 $B$N%Y%/%?HV9f(B */

#define EITATR(smode, imask)	(((smode) << 31) + ((imask) << 16))

/*
 *  EIT$B%Y%/%?%F!<%V%k$N9=B$$NDj5A(B
 */
typedef struct eit_vector_entry {
	UINT	eitatr;			/* EIT$BB0@-(B */
	FP	eithdr;			/* EIT$B%O%s%I%i$N%"%I%l%9(B */
} EITVE;

/*
 *  EIT$B%Y%/%?%F!<%V%k%Y!<%9(B (EITVB) $B$N@_Dj(B
 *
 *  $B=i4|2==hM}$NCf$G;H$&$?$a$N4X?t!%(B
 */
Inline void
set_eitvb(EITVE *eitvb)
{
	Asm("ldc %0, %1" : : "r"(eitvb), "m"(CREG(EITVB)));
}

/*
 *  EIT$B%Y%/%?%F!<%V%k%Y!<%9(B (EITVB) $B$N8=:_CM$NFI$_=P$7(B
 *
 *  EITVB_ITIS $B$,Dj5A$5$l$F$$$k;~$O!$=i4|2==hM}$NCf$G(B EITVB$B%l%8%9%?$r(B
 *  EITVB_ITIS $B$K@_Dj$9$k$N$G!$(BEITVB_ITIS $B$rJV$;$P$h$$!%$=$&$G$J$$>l9g(B
 *  $B$O!$(BCPU $B$N(B EITVB$B%l%8%9%?$rFI$_=P$9!%(B
 */
#ifdef EITVB_ITIS
#define current_eitvb()	((EITVE *) EITVB_ITIS)
#else /* EITVB_ITIS */

Inline EITVE
*current_eitvb(void)
{
	EITVE	*eitvb;

	Asm("stc %1, %0" : "=r"(eitvb) : "m"(CREG(EITVB)));
	return(eitvb);
}

#endif /* EITVB_ITIS */

/*
 *  EIT$B%O%s%I%i$N@_Dj(B
 *
 *  $B%Y%/%H%kHV9f(B eitvec $B$N(B EIT$B%Y%/%?%F!<%V%k%(%s%H%j$N!$(BEIT$BB0@-(B (EIT$B%O(B
 *  $B%s%I%i<B9T;~$N!$%9%?%C%/%b!<%I!$%"%I%l%9JQ49%b!<%I!$%G%P%C%0%b!<%I!$(B
 *  $B3d9~$_%^%9%/$r7h$a$k(B) $B$r(B eitatr$B!$(BEIT$B%O%s%I%i3+;OHVCO$r(B eithdr $B$K@_(B
 *  $BDj$9$k!%(B
 */
Inline void
define_eit(INT eitvec, UINT eitatr, FP eithdr)
{
	EITVE	*eitvb = current_eitvb();

	eitvb[eitvec].eitatr = eitatr;
	eitvb[eitvec].eithdr = eithdr;
}

/*
 *  $B%a%b%j%V%m%C%/A`:n%i%$%V%i%j(B
 *
 *  $B4X?t$N;EMM$O!$(BANSI C $B%i%$%V%i%j$N;EMM$HF1$8!%I8=`%i%$%V%i%j$N$b$N$r(B
 *  $B;H$C$?J}$,8zN($,NI$$2DG=@-$,$"$k!%(B
 */

Inline VP
memcpy(VP dest, VP src, UINT len)
{
#ifdef tron
	if (len > 0) {
		register VP	src_r0 asm("r0") = src;
		register VP	dest_r1 asm("r1") = dest;
		register UINT	len_r2 asm("r2") = len;

		asm("smov/f.b"
		  : /* no output */
		  : "g"(src_r0), "g"(dest_r1), "g"(len_r2)
		  : "r0", "r1", "r2", "memory");
	}
#else /* tron */
	VB	*d = dest;
	VB	*s = src;

	while (len-- > 0) {
		*d++ = *s++;
	}
#endif /* tron */
	return(dest);
}

Inline VP
memset(VP str, INT ch, UINT len)
{
#ifdef tron
	if (len > 0) {
		register VP	str_r1 asm("r1") = str;
		register UINT	len_r2 asm("r2") = len;
		register INT	ch_r3 asm("r3") = ch;

		asm("sstr.b"
		  : /* no output */
		  : "g"(str_r1), "g"(len_r2), "g"(ch_r3)
		  : "r1", "r2", "memory");
	}
#else /* tron */
	VB	*s = str;

	while (len-- > 0) {
		*s++ = ch;
	}
#endif /* tron */
	return(str);
}

#endif /* _TRON_INSN_ */
