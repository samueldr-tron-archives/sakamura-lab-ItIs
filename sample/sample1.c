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
 *  @(#) $Id: sample1.c,v 1.2 1998/07/06 11:07:06 hiro Exp $
 */

/* 
 *  $B%5%s%W%k%W%m%0%i%`(B($B#1(B)
 *
 *  ItIs $B$N4pK\E*$JF0:n$r3NG'$9$k$?$a$N%5%s%W%k%W%m%0%i%`!%%+!<%M%k$H(B
 *  $B%j%s%/$7$F=i4|5/F0%?%9%/$H$7$FF0:n$5$;$k$3$H$b!$FHN)$K%j%s%/$7$FF0(B
 *  $B:n$5$;$k$3$H$b2DG=$G$"$k!%(B
 *
 *  $B<B9T;~$NCm0U(B:
 *  (1) $B%5%s%W%k%W%m%0%i%`$N%a%$%s%?%9%/$r(B HIGH_PRIORITY $B$h$j9b$$M%@h(B
 *  	$BEY$G<B9T$9$k$3$H(B ($BG0$N$?$a%a%$%s%?%9%/$NM%@hEY$r(B MAIN_PRIORITY 
 *	$B$KJQ99$7$F$$$k(B)$B!%(B
 *  (2) $B%5%s%W%k%W%m%0%i%`$N%a%$%s%?%9%/$N(B ID $B$r!$%5%s%W%k%W%m%0%i%`Fb(B
 *  	$B$G@8@.$9$k%?%9%/$N(B ID (TASK1_ID$B!A(BTASK3_ID) $B$H$V$D$+$i$J$$$h$&(B
 *	$B$K$9$k$3$H!%(B
 *
 *  $B%W%m%0%i%`$N35MW(B:
 *
 *  $B%a%$%s%?%9%/$,5/F0$5$l$k$H!$%?%9%/(BID $B$,(B TASK1_ID$B!A(BTASK3_ID $B$N(B3$B$D$N(B
 *  $B%?%9%/$r=i4|M%@hEY(B 15 $B$G@8@.!&5/F0$9$k!%$3$l$i$NJBNs<B9T$5$l$k%?%9(B
 *  $B%/$O!$(BTASK_LOOP $B2s6u%k!<%W$r<B9T$9$kEY$K!$%?%9%/$,<B9TCf$G$"$k$3$H(B
 *  $B$r$"$i$o$9%a%C%;!<%8$rI=<($9$k!%(B
 *
 *  3$B$D$N%?%9%/$r5/F0$7$?8e!$%a%$%s%?%9%/$O!$%3%s%=!<%k$+$i$NJ8;zF~NO(B
 *  $B$rBT$A(B ($BJ8;zF~NO$rBT$C$F$$$k4V$O!$5/F0$7$?%?%9%/$,<B9T$5$l$F$$$k(B)$B!$(B
 *  $BJ8;z$,F~NO$5$l$k$H!$F~NO$5$l$?J8;z$KBP1~$7$?=hM}$r<B9T$9$k!%F~NO$5(B
 *  $B$l$?J8;z$H=hM}$N4X78$O2<$NDL$j!%(BControl-C $B$,F~NO$5$l$k$H!$5/F0$7$?(B
 *  3$B$D$N%?%9%/$r=*N;!&:o=|$7!$%5%s%W%k%W%m%0%i%`$r=*N;$9$k!%(B
 *
 *  '1' : $B0J9_$N%3%^%s%I$O%?%9%/(BID $B$,(B TASK1_ID $B$N%?%9%/$KBP$7$F9T$&(B
 *  '2' : $B0J9_$N%3%^%s%I$O%?%9%/(BID $B$,(B TASK2_ID $B$N%?%9%/$KBP$7$F9T$&(B
 *  '3' : $B0J9_$N%3%^%s%I$O%?%9%/(BID $B$,(B TASK3_ID $B$N%?%9%/$KBP$7$F9T$&(B
 *  '>' : $B%?%9%/$NM%@hEY$r(B HIGH_PRIORITY $B$K$9$k(B
 *  '=' : $B%?%9%/$NM%@hEY$r(B MID_PRIORITY $B$K$9$k(B
 *  '<' : $B%?%9%/$NM%@hEY$r(B LOW_PRIORITY $B$K$9$k(B
 *  's' : $B%?%9%/$r(B sus_tsk $B$K$h$j6/@)BT$A>uBV$K$9$k(B
 *  'm' : $B%?%9%/$N6/@)BT$A>uBV$r(B rsm_tsk $B$K$h$j2r=|$9$k(B
 *  'M' : $B%?%9%/$N6/@)BT$A>uBV$r(B frsm_tsk $B$K$h$j6/@)2r=|$9$k(B
 *  'r' : $B3FM%@hEY%l%Y%k$N%l%G%#%-%e!<$r2sE>$5$;$k(B
 *  'R' : $B%?%9%/$N>uBV$r;2>H!&I=<($9$k(B
 *  'a' : $B3FM%@hEY%l%Y%k$N%l%G%#%-%e!<$r2sE>$5$;$k<~4|5/F0%O%s%I%i$r(B2
 *	  $BIC<~4|$G5/F0$9$k(B
 *  'A' : $B<~4|5/F0%O%s%I%i$r2r=|$9$k(B
 *  'v' : $BH/9T$7$?%7%9%F%`%3!<%k$rI=<($9$k(B
 *  'q' : $BH/9T$7$?%7%9%F%`%3!<%k$rI=<($7$J$$(B ($B%G%U%)%k%H(B)
 */

#include <itis_services.h>
#include <itis_syslog.h>
#include <itis_ioctl.h>
#ifndef LINK_KERNEL
#include <svc_serial.h>
#endif /* LINK_KERNEL */

#define TASK_LOOP	2500000		/* $B%?%9%/Fb$G$N%k!<%W2s?t(B */

#define TASK1_ID	1		/* $BJBNs$K<B9T$5$l$k%?%9%/$N(BID */
#define TASK2_ID	2
#define TASK3_ID	3

#define MAIN_PRIORITY	10		/* $B%a%$%s%?%9%/$NM%@hEY(B */
#define HIGH_PRIORITY	14		/* $BJBNs$K<B9T$5$l$k%?%9%/$NM%@hEY(B */
#define MID_PRIORITY	15
#define LOW_PRIORITY	16

#define	STACK_SIZE	8192		/* $B%?%9%/$N%9%?%C%/%5%$%:(B */

#define CYCNO		1		/* $B<~4|5/F0%O%s%I%i$NHV9f(B */

#define CONSOLE_PORT	0		/* $B%3%s%=!<%k$N%]!<%HHV9f(B */

#ifdef LINK_KERNEL
/*
 *  $B%+!<%M%k$H%j%s%/$7$F=i4|5/F0%?%9%/$H$7$FF0:n$5$;$k>l9g(B
 */
#define MAIN		first_task
#define TSK_INIT	(-5)
#define FINISH()	wup_tsk(TSK_INIT)

#else /* LINK_KERNEL */
/*
 *  $BFHN)$K%j%s%/$7$FF0:n$5$;$k>l9g(B
 */
#define MAIN		main
#define FINISH()	exd_tsk()

#endif /* LINK_KERNEL */

/*
 *  $BJBNs$K<B9T$5$l$k%?%9%/(B
 */
void task(INT tskno)
{
	INT	i, n = 0;
	char	*graph[] = { "|", "  +", "    *" };

	while (1) {
		for (i = 0; i < TASK_LOOP; i++);
		syslog(LOG_NOTICE, "task%d is running (%03d).   %s",
				tskno, ++n, graph[tskno-1]);
	}
	exd_tsk();			/* $B$3$l$,<B9T$5$l$k$3$H$O$J$$(B */
}

/*
 *  $B%?%9%/$N@8@.$H5/F0(B
 */
ER create_start_task(ID tskid, FP task, PRI tskpri, INT stacd)
{
	ER	ercd;
	T_CTSK	ctsk;

	ctsk.exinf = 0;
	ctsk.tskatr = TA_HLNG;
	ctsk.task = task;
	ctsk.itskpri = tskpri;
	ctsk.stksz = STACK_SIZE;
	ercd = cre_tsk(tskid, &ctsk);
	if (ercd != E_OK) {
		syslog(LOG_NOTICE, "erron %d with cre_tsk(%d)", ercd, tskid);
		return(ercd);
	}
	ercd = sta_tsk(tskid, stacd);
	if (ercd != E_OK) {
		syslog(LOG_NOTICE, "error %d with sta_tsk(%d)", ercd, tskid);
	}
	return(ercd);
}

/*
 *  $B%?%9%/$N6/@)=*N;$H:o=|(B
 */
ER terminate_delete_task(ID tskid)
{
	ER	ercd;

	ercd = ter_tsk(tskid);
	if (ercd != E_OK) {
		syslog(LOG_NOTICE, "error %d with ter_tsk(%d)", ercd, tskid);
		return(ercd);
	}
	ercd = del_tsk(tskid);
	if (ercd != E_OK) {
		syslog(LOG_NOTICE, "error %d with del_tsk(%d)", ercd, tskid);
	}
	return(ercd);
}

/*
 *  $B%?%9%/>uBV$N;2>H$HI=<((B
 */
void refer_task(ID tskid)
{
	T_RTSK	pk_rtsk;

	ref_tsk(&pk_rtsk, tskid);
	syslog(LOG_NOTICE, "#result of ref_tsk(%d):", tskid);
	syslog(LOG_NOTICE, " exinf=0x%x, taskpri=%d, tskstat=%x,"
			" tskwait=0x%x, wid=%d, suscnt=%d, wupcnt=%d",
			pk_rtsk.exinf, pk_rtsk.tskpri, pk_rtsk.tskstat,
			pk_rtsk.tskwait, pk_rtsk.wid,
			pk_rtsk.suscnt, pk_rtsk.wupcnt);
}

/*
 *  $B<~4|5/F0%O%s%I%i(B
 *
 *  HIGH_PRIORITY, MID_PRIORITY, LOW_PRIORITY $B$N3FM%@hEY$N%l%G%#%-%e!<(B
 *  $B$r2sE>$5$;$k!%(B
 */
void cyclic_handler()
{
	rot_rdq(HIGH_PRIORITY);
	rot_rdq(MID_PRIORITY);
	rot_rdq(LOW_PRIORITY);
}

/*
 *  $B%a%$%s%?%9%/(B
 */
MAIN(INT stacd)
{
	char	buf[1];
	ID	tskid = TASK1_ID;
	T_DCYC	pk_dcyc;
    
	syslog(LOG_NOTICE, "Sample task starts (stacd = %d).", stacd);

	chg_pri(TSK_SELF, MAIN_PRIORITY);
	serial_ioctl(CONSOLE_PORT, ECHO, 0);

	create_start_task(TASK1_ID, &task, MID_PRIORITY, 1);
	create_start_task(TASK2_ID, &task, MID_PRIORITY, 2);
	create_start_task(TASK3_ID, &task, MID_PRIORITY, 3);

	pk_dcyc.exinf = 0;
	pk_dcyc.cycatr = TA_HLNG;
	pk_dcyc.cychdr = (FP) cyclic_handler;
	pk_dcyc.cycact = TCY_ON;
	pk_dcyc.cyctim = 2000;		/* 2$BIC(B */

	do {
		serial_read(0, buf, 1);
		switch (buf[0]) {
		case '1':
			tskid = TASK1_ID;
			break;
		case '2':
			tskid = TASK2_ID;
			break;
		case '3':
			tskid = TASK3_ID;
			break;
		case '>':
			chg_pri(tskid, HIGH_PRIORITY);
			syslog(LOG_INFO, "#chg_pri(%d, HIGH_PRIORITY)", tskid);
			break;
		case '-':
			chg_pri(tskid, MID_PRIORITY);
			syslog(LOG_INFO, "#chg_pri(%d, MID_PRIORITY)", tskid);
			break;
		case '<':
			chg_pri(tskid, LOW_PRIORITY);
			syslog(LOG_INFO, "#chg_pri(%d, LOW_PRIORITY)", tskid);
			break;
		case 's':
			sus_tsk(tskid);
			syslog(LOG_INFO, "#sus_tsk(%d)", tskid);
			break;
		case 'm':
			rsm_tsk(tskid);
			syslog(LOG_INFO, "#rsm_tsk(%d)", tskid);
			break;
		case 'M':
			frsm_tsk(tskid);
			syslog(LOG_INFO, "#frsm_tsk(%d)", tskid);
			break;
		case 'r':
			rot_rdq(HIGH_PRIORITY);
			rot_rdq(MID_PRIORITY);
			rot_rdq(LOW_PRIORITY);
			syslog(LOG_INFO, "#rot_rdq(all priorities)");
			break;
		case 'R':
			refer_task(tskid);
			break;
		case 'a':
			def_cyc(CYCNO, &pk_dcyc);
			break;
		case 'A':
			def_cyc(CYCNO, NADR);
			break;
		case 'v':
			setlogmask(LOG_UPTO(LOG_INFO));
			break;
		case 'q':
			setlogmask(LOG_UPTO(LOG_NOTICE));
			break;
		default:
			break;
		}
	} while (buf[0] != '\003');

	terminate_delete_task(TASK1_ID);
	terminate_delete_task(TASK2_ID);
	terminate_delete_task(TASK3_ID);
	def_cyc(CYCNO, NADR);

	syslog(LOG_NOTICE, "Sample task ends.");
	FINISH();
}
