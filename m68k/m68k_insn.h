/**
 * 
 * 	ItIs - An ITRON Implementation for Research and Education
 * 
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
 * 3. Neither the name of the university nor the name of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: m68k_insn.h,v 1.2 1998/06/18 07:45:23 hiro Exp $
 */

#ifndef	_M68K_INSN_
#define	_M68K_INSN_

/*
 *  $B@)8f%l%8%9%?$NA`:n4X?t(B
 */

/*
 *  SR $B$N8=:_CM$NFI$_=P$7(B
 */
Inline VH
current_sr(void)
{
	VH	sr;

	Asm("move.w sr, %0" : "=g"(sr));
	return(sr);
}

/*
 *  SR $B$N8=:_CM$NJQ99(B
 *
 *  clobber$B%j%9%H$K(B memory $B$rF~$l$F$$$k$N$O!$(Bset_sr $B$ND>8e$K%?%9%/%G%#(B
 *  $B%9%Q%C%A$,5/$3$j!$M=4|$7$J$$%a%b%j%(%j%"$,=q$-49$($i$l$k2DG=@-$,$"(B
 *  $B$k$3$H$r%3%s%Q%$%i$KCN$i$;$k$?$a$G$"$k!%6qBNE*$K$O!$%(%i!<%3!<%I$r(B
 *  $BF~$l$kJQ?t$,LdBj$K$J$k!%(B
 */
Inline void
set_sr(VH sr)
{
	Asm("move.w %0, sr" : : "g"(sr) : "memory");
}

/*
 *  $B3d9~$_%^%9%/>uBV$rFbItI=8=$GJV$9!%(B
 */
Inline INT
current_intmask(void)
{
	return(current_sr() & 0x0700);
}

/*
 *  NMI $B$r=|$/$9$Y$F$N3d9~$_$r6X;_$9$k!%(B
 */
Inline void
disint(void)
{
	Asm("ori.w #0x0700, sr");
}

/*
 *  current_intmask $B$,JV$7$?3d9~$_%^%9%/>uBV$rEO$7$F!$3d9~$_6X;_A0$N>u(B
 *  $BBV$KLa$9!%(B
 */
Inline void
enaint(INT intmask)
{
	set_sr((current_sr() & ~0x0700) | intmask);
}

/*
 *  intmask $B$+$i!$(BCPU$B%m%C%/Cf$+$I$&$+$rH=JL$9$k!%%m%C%/Cf$G$"$k>l9g$K(B
 *  $B??$H$J$k!%(B
 */
#define intmask_lock(intmask)	((intmask) > 0)

/*
 *  IPM $B$N8=:_CM$NFI$_=P$7(B
 *
 *  IPM $B$NCM$H$7$F!$(B0 $B0J>e(B 7 $B0J2<$NCM$,JV$k!%(B
 */
Inline INT
current_ipm(void)
{
	return(current_intmask() >> 16);
}

/*
 *  IPM $B$N8=:_CM$NJQ99(B
 *
 *  ipm $B$O!$(B0 $B0J>e(B 7 $B0J2<$G$"$k$3$H!%(B
 */
Inline void
set_ipm(INT ipm)
{
	set_sr((current_sr() & ~0x0700) | (ipm << 16));
}

/*
 *  $BNc304XO"$NDj5A(B
 */

#define EXCVEC_AFAULT	0x02		/* Access Fault $B$N%Y%/%?HV9f(B */

#define EXCVEC_TRAP1	0x21		/* TRAP #1 $B$N%Y%/%?HV9f(B */
#define EXCVEC_TRAP2	0x22		/* TRAP #2 $B$N%Y%/%?HV9f(B */
#define EXCVEC_TRAP6	0x26		/* TRAP #6 $B$N%Y%/%?HV9f(B */
#define EXCVEC_TRAP7	0x27		/* TRAP #7 $B$N%Y%/%?HV9f(B */

/*
 *  $BNc30%Y%/%?%F!<%V%k$N9=B$$NDj5A(B
 */
typedef struct exc_vector_entry {
	FP	exchdr;			/* $BNc30%O%s%I%i$N%"%I%l%9(B */
} EXCVE;

/*
 *  $B%Y%/%?%Y!<%9%l%8%9%?(B (VBR) $B$N@_Dj(B
 *
 *  $B=i4|2==hM}$NCf$G;H$&$?$a$N4X?t!%(B
 */
Inline void
set_vbr(EXCVE *vbr)
{
	Asm("movec.l %0, vbr" : : "r"(vbr));
}

/*
 *  $B%Y%/%?%Y!<%9%l%8%9%?(B (VBR) $B$N8=:_CM$NFI$_=P$7(B
 *
 *  EXCVT_ITIS $B$,Dj5A$5$l$F$$$k;~$O!$=i4|2==hM}$NCf$G(B VBR $B$r(B EXCVT_ITIS
 *  $B$K@_Dj$9$k$N$G!$(BEXCVT_ITIS $B$rJV$;$P$h$$!%$=$&$G$J$$>l9g$O!$(BCPU $B$N(B 
 *  VBR $B%l%8%9%?$rFI$_=P$9!%(B
 */
#ifdef EXCVT_ITIS
#define current_vbr()	((EXCVE *) EXCVT_ITIS)
#else /* EXCVT_ITIS */

Inline EXCVE
*current_vbr(void)
{
	EXCVE	*vbr;

	Asm("movec.l vbr, %0" : "=r"(vbr));
	return(vbr);
}

#endif /* EXCVT_ITIS */

/*
 *  $BNc30%O%s%I%i$N@_Dj(B
 *
 *  $B%Y%/%H%kHV9f(B excvec $B$NNc30%O%s%I%i3+;OHVCO$r(B exchdr $B$K@_Dj$9$k!%(B
 */
Inline void
define_exc(INT excvec, FP exchdr)
{
	EXCVE	*excvt = current_vbr();

	excvt[excvec].exchdr = exchdr;
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
	VB	*d = dest;
	VB	*s = src;

	while (len-- > 0) {
		*d++ = *s++;
	}
	return(dest);
}

Inline VP
memset(VP str, INT ch, UINT len)
{
	VB	*s = str;

	while (len-- > 0) {
		*s++ = ch;
	}
	return(str);
}

#endif /* _M68K_INSN_ */
