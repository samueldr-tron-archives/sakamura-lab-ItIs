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
 *  @(#) $Id: sys_util.c,v 1.2 1998/06/18 07:44:10 hiro Exp $
 */

#include "itis_kernel.h"
#include "dve68k.h"

/*
 *  $B%W%m%;%C%5<1JL$N$?$a$NJQ?t(B ($B%^%k%A%W%m%;%C%5BP1~(B)
 */
INT	board_id;			/* $B%\!<%I(BID */
INT	board_addr;			/* $B%m!<%+%k%a%b%j$N@hF,%"%I%l%9(B */

/*
 *  $B%"%;%s%V%i%l%Y%k$N%P%9%(%i!<%O%s%I%i(B
 */
extern void	afault_handler_startup(void);

/*
 *  $B%P%9%(%i!<$rJa$^$($k$?$a$NJQ?t(B
 */
VP	buserr_catch;

/*
 *  $B%?!<%2%C%H%7%9%F%`0MB8(B $B=i4|2=%k!<%A%s(B
 */
void
sys_initialize(void)
{
	/*
	 *  $B3d9~$_4XO"$N=i4|2=(B
	 */
	*DGA_CSR21 = 0;			/* $B$9$Y$F$N3d9~$_$r%^%9%/(B */
	*DGA_CSR23 = ~0;		/* $BA4$F$N3d9~$_$r%/%j%"(B */

	*DGA_CSR19 = (G0I_VEC << 24) | (G1I_VEC << 16)
				| (SWI_VEC << 8) | SPRI_VEC;
					/* $B3d9~$_%Y%/%H%k$N@_Dj(B */

	/*
	 *  $B%"%\!<%H3d9~$_$N@_Dj(B (NMI)
	 */
	dga_set_ilv(DGA_CSR24, ABTIL_BIT, IRQ_NMI);
					/* $B3d9~$_%l%Y%k@_Dj(B */
	*DGA_CSR23 = ABT_BIT;		/* $B3d9~$_MW5a$r%/%j%"(B */
	*DGA_CSR21 |= ABT_BIT;		/* $B3d9~$_%^%9%/2r=|(B */

	/*
	 *  $B%a%b%jNN0h$N@_Dj(B
	 */
	*DGA_CSR4 = (board_addr | 0x00ff);	/* $B%m!<%+%k%a%b%j(B 16MB */
	*DGA_CSR5 = 0x0000012f;		/* VME$B$+$i3HD%%"%I%l%9%"%/%;%9(B */

	/*
	 *  $B%$%s%?%U%'!<%9%l%8%9%?(B (IFR) $B$N@_Dj(B
	 */
	*DGA_CSR3 = ((board_id << 4) | 0x3);	/* $B%Y!<%9%"%I%l%9(B */
	*DGA_IFR0 = 0x80000000;			/* SQR $B%U%i%0%/%j%"(B */
	*DGA_IFR3 = board_id;			/* IFR3 $B$r@_Dj(B */

	/*
	 *  $B%i%&%s%I%m%S%s%b!<%I$K@_Dj(B ($B%^%k%A%W%m%;%C%5BP1~(B)
	 */
	*DGA_CSR1 = (*DGA_CSR1 & 0xffeffcff)
			| (1 << 20) | ((board_id % 4) << 8);

	/*
	 *  Access Fault $B%O%s%I%i$NDj5A(B
	 */
	buserr_catch = (VP) 0;
	define_exc(EXCVEC_AFAULT, afault_handler_startup);
}

/*
 *  $B%?!<%2%C%H%7%9%F%`$N=*N;%k!<%A%s(B
 *
 *  ItIs $B$N%7%9%F%`$r=*N;$9$k;~$K;H$&!%DL>o$O%b%K%?8F$S=P$7$G<B8=$9$k!%(B
 */
void
sys_exit(void)
{
	dve68k_exit();
}

/*
 *  $B%?!<%2%C%H%7%9%F%`$NJ8;z=PNO%k!<%A%s(B
 *
 *  $B%m%0%?%9%/$,F0$$$F$$$J$$;~$K!$%7%9%F%`$+$i$N%a%C%;!<%8$r=PNO$9$k$?(B
 *  $B$a$K;H$&!%DL>o$O%b%K%?8F$S=P$7$G<B8=$9$k!%(B
 */
int
sys_write(const char *buf, unsigned int len)
{
	int	i, c;

	for (i = 0; i < len; i++) {
		if ((c = *buf++) == '\n') {
			dve68k_putc('\r');
		}
		dve68k_putc(c);
	}
	return(len);
}

/*
 *  Access Fault $B%O%s%I%i(B (68040$B0MB8(B)
 *
 *  $B%P%9%(%i!<$,5/$3$k$N$rJa$^$($?$$>l9g$O!$(Bbuserr_catch $B$K%P%9%(%i!<(B 
 *  $B$,5/$3$C$?;~$NLa$jHVCO$r@_Dj$9$k!%$?$@$7!$Ja$^$($i$l$k$N$O!$%j!<%I(B
 *  $B%"%/%;%9$K$h$C$F5/$3$C$?%P%9%(%i!<$N$_$G$"$k!%(B
 */
void
afault_handler(VH *sp)
{
	if (buserr_catch && (*(sp+6) & 0x700)==0x100) {
		*((VW *)(sp+1)) = (VW) buserr_catch;	/* $BLa$jHVCO$rJQ99(B */
		buserr_catch = (VP) 0;
	}
	else {
		i_syslog(LOG_EMERG, "Bus error occurs.");
		i_syslog(LOG_EMERG, "CSR0-1 = %x   SP = %08x",
					*DGA_CSR0_1, sp);
		i_syslog(LOG_EMERG, "SR = %04x    PC = %08x",
					*sp, *((VW *)(sp+1)));
		i_syslog(LOG_EMERG, "SSW = %04x   FA = %08x",
					*(sp+6), *((VW *)(sp+10)));
		i_syslog(LOG_EMERG, "WB3S = %04x  WB2S = %04x  WB1S = %04x",
					*(sp+7), *(sp+8), *(sp+9));
		itis_exit();
	}
}
