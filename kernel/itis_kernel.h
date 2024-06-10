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
 *  @(#) $Id: itis_kernel.h,v 1.13 1998/01/30 09:52:32 hiro Exp $
 */

#ifndef _ITIS_KERNEL_
#define _ITIS_KERNEL_

/*
 *  ItIs$B%+!<%M%k(B $BI8=`%$%s%/%k!<%I%U%!%$%k(B
 */

/*
 *  ItIs $BI8=`%$%s%/%k!<%I%U%!%$%k(B
 */
#include <itron.h>
#include <itron_errno.h>
#include <itron_fncode.h>
#include <itis_debug.h>
#include <itis_stddefs.h>
#include <itis_syslog.h>

/*
 *  $B%+!<%M%kMQ(B assert$B%^%/%m$NDj5A(B
 */
#ifndef NDEBUG

extern void	i_assert_fail(const char *expr, const char *file,
					unsigned int line);
#define assert(exp) \
	{ if (!(exp)) { i_assert_fail(#exp, __FILE__, __LINE__); }}

#else /* NDEBUG */

#define assert(exp)

#endif /* NDEBUG */

/*
 *  $B%+!<%M%k9=@.@_Dj%U%!%$%k(B
 */
#include "config.h"
#include "cpu_conf.h"
#include "sys_conf.h"
#include "isysconf.h"

/*
 *  CPU$B0MB8%$%s%/%k!<%I%U%!%$%k(B
 */
#include "cpu_status.h"

/*
 *  $B%7%9%F%`%3!<%k%W%m%H%?%$%W@k8@(B
 */
#include "isyscall.h"

/*
 *  $B%7%9%F%`=i4|2=MQ4X?t(B ($B3F%b%8%e!<%k(B)
 */
extern void	task_initialize(void);
extern void	semaphore_initialize(void);
extern void	eventflag_initialize(void);
extern void	mailbox_initialize(void);
extern void	messagebuffer_initialize(void);
extern void	rendezvous_initialize(void);
extern void	prisem_initialize(void);
extern void	memorypool_initialize(void);
extern void	fix_memorypool_initialize(void);
extern void	cyclichandler_initialize(void);
extern void	alarmhandler_initialize(void);
extern void	extended_svc_initialize(void);

/*
 *  $B%7%9%F%`=*N;MQ4X?t(B (startup.c)
 */
extern void	itis_exit(void);

/*
 *  $B%7%9%F%`%a%b%j%W!<%k$+$i$N%a%b%j%V%m%C%/$N<hF@$HJV5Q(B (mempool.c)
 *
 *  $B%7%9%F%`%a%b%j%W!<%k$O!$(BID $B$,(B TMPL_OS $B$N%a%b%j%W!<%k$H$7$F%"%/%;%9(B
 *  $B$9$k$3$H$b$G$-$k$,!$%+!<%M%kFb$+$iMQ$$$k$?$a$N%$%s%?%U%'!<%9$rMQ0U(B
 *  $B$7$F$$$k!%(Bsys_get_blk $B$O!$%a%b%j%V%m%C%/$,<h$l$J$+$C$?>l9g$K!$BT$A(B
 *  $B>uBV$K$OF~$i$:$K!$B(:B$K(B NULL $B$rJV$9!%(B
 */
extern ER	sys_cre_mpl(void);
extern VP	sys_get_blk(INT size);
extern void	sys_rel_blk(VP blk);

/*
 *  $B%+!<%M%kMQ%7%9%F%`%m%05-O?%i%$%V%i%j(B (isyslog.c)
 */
extern void	i_syslog(int class, const char *format, ...);

/*
 *  $B%?!<%2%C%H%7%9%F%`0MB8%k!<%A%s(B (sys_util.c)
 */
extern void	sys_initialize(void);
extern void	sys_exit(void);
extern int	sys_write(const char *buf, unsigned int len);

/*
 *  $B$=$NB>$NDj5A(B
 */
#define	offsetof(structure, field) ((INT) &(((structure *) 0)->field))

#define SYSCALL

#endif /* _ITIS_KERNEL_ */
