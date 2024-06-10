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
 *  @(#) $Id: sys_util.c,v 1.4 1998/01/30 09:56:45 hiro Exp $
 */

#include "itis_kernel.h"
#include "h32sbc.h"

/*
 *  $B%?!<%2%C%H%7%9%F%`0MB8(B $B=i4|2=%k!<%A%s(B
 */
void
sys_initialize(void)
{
}

/*
 *  $B%?!<%2%C%H%7%9%F%`$N=*N;%k!<%A%s(B
 *
 *  ItIs $B$N%7%9%F%`$r=*N;$9$k;~$K;H$&!%DL>o$O%b%K%?8F$S=P$7$G<B8=$9$k!%(B
 */
void
sys_exit(void)
{
	h32_exit();
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
			h32_putc('\r');
		}
		h32_putc(c);
	}
	return(len);
}
