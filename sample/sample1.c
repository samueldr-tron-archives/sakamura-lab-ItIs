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
 *  サンプルプログラム(１)
 *
 *  ItIs の基本的な動作を確認するためのサンプルプログラム．カーネルと
 *  リンクして初期起動タスクとして動作させることも，独立にリンクして動
 *  作させることも可能である．
 *
 *  実行時の注意:
 *  (1) サンプルプログラムのメインタスクを HIGH_PRIORITY より高い優先
 *  	度で実行すること (念のためメインタスクの優先度を MAIN_PRIORITY 
 *	に変更している)．
 *  (2) サンプルプログラムのメインタスクの ID を，サンプルプログラム内
 *  	で生成するタスクの ID (TASK1_ID〜TASK3_ID) とぶつからないよう
 *	にすること．
 *
 *  プログラムの概要:
 *
 *  メインタスクが起動されると，タスクID が TASK1_ID〜TASK3_ID の3つの
 *  タスクを初期優先度 15 で生成・起動する．これらの並列実行されるタス
 *  クは，TASK_LOOP 回空ループを実行する度に，タスクが実行中であること
 *  をあらわすメッセージを表示する．
 *
 *  3つのタスクを起動した後，メインタスクは，コンソールからの文字入力
 *  を待ち (文字入力を待っている間は，起動したタスクが実行されている)，
 *  文字が入力されると，入力された文字に対応した処理を実行する．入力さ
 *  れた文字と処理の関係は下の通り．Control-C が入力されると，起動した
 *  3つのタスクを終了・削除し，サンプルプログラムを終了する．
 *
 *  '1' : 以降のコマンドはタスクID が TASK1_ID のタスクに対して行う
 *  '2' : 以降のコマンドはタスクID が TASK2_ID のタスクに対して行う
 *  '3' : 以降のコマンドはタスクID が TASK3_ID のタスクに対して行う
 *  '>' : タスクの優先度を HIGH_PRIORITY にする
 *  '=' : タスクの優先度を MID_PRIORITY にする
 *  '<' : タスクの優先度を LOW_PRIORITY にする
 *  's' : タスクを sus_tsk により強制待ち状態にする
 *  'm' : タスクの強制待ち状態を rsm_tsk により解除する
 *  'M' : タスクの強制待ち状態を frsm_tsk により強制解除する
 *  'r' : 各優先度レベルのレディキューを回転させる
 *  'R' : タスクの状態を参照・表示する
 *  'a' : 各優先度レベルのレディキューを回転させる周期起動ハンドラを2
 *	  秒周期で起動する
 *  'A' : 周期起動ハンドラを解除する
 *  'v' : 発行したシステムコールを表示する
 *  'q' : 発行したシステムコールを表示しない (デフォルト)
 */

#include <itis_services.h>
#include <itis_syslog.h>
#include <itis_ioctl.h>
#ifndef LINK_KERNEL
#include <svc_serial.h>
#endif /* LINK_KERNEL */

#define TASK_LOOP	2500000		/* タスク内でのループ回数 */

#define TASK1_ID	1		/* 並列に実行されるタスクのID */
#define TASK2_ID	2
#define TASK3_ID	3

#define MAIN_PRIORITY	10		/* メインタスクの優先度 */
#define HIGH_PRIORITY	14		/* 並列に実行されるタスクの優先度 */
#define MID_PRIORITY	15
#define LOW_PRIORITY	16

#define	STACK_SIZE	8192		/* タスクのスタックサイズ */

#define CYCNO		1		/* 周期起動ハンドラの番号 */

#define CONSOLE_PORT	0		/* コンソールのポート番号 */

#ifdef LINK_KERNEL
/*
 *  カーネルとリンクして初期起動タスクとして動作させる場合
 */
#define MAIN		first_task
#define TSK_INIT	(-5)
#define FINISH()	wup_tsk(TSK_INIT)

#else /* LINK_KERNEL */
/*
 *  独立にリンクして動作させる場合
 */
#define MAIN		main
#define FINISH()	exd_tsk()

#endif /* LINK_KERNEL */

/*
 *  並列に実行されるタスク
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
	exd_tsk();			/* これが実行されることはない */
}

/*
 *  タスクの生成と起動
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
 *  タスクの強制終了と削除
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
 *  タスク状態の参照と表示
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
 *  周期起動ハンドラ
 *
 *  HIGH_PRIORITY, MID_PRIORITY, LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void cyclic_handler()
{
	rot_rdq(HIGH_PRIORITY);
	rot_rdq(MID_PRIORITY);
	rot_rdq(LOW_PRIORITY);
}

/*
 *  メインタスク
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
	pk_dcyc.cyctim = 2000;		/* 2秒 */

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
