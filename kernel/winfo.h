/**
 * 
 * 	    ItIs - ITRON Implementation by Sakamura Lab
 * 
 * Copyright (C) 1989-1996 by Sakamura Lab, the University of Tokyo, JAPAN
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: winfo.h,v 1.9 1997/01/10 13:36:23 hiro Exp $
 */

#ifndef _WINFO_
#define _WINFO_

/*
 *  同期・通信オブジェクト毎に必要な待ち情報の定義
 */

/*
 *  イベントフラグ待ち (TTW_FLG)
 */
typedef struct {
	UINT	waiptn;		/* 待ちビットパターン */
	UINT	wfmode;		/* 待ちモード */
	UINT	*p_flgptn;	/* 待ち解除時ビットパターンを入れるアドレス */
} WINFO_FLG;

/*
 *  メイルボックス待ち (TTW_MBX)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* メッセージパケットの先頭を入れるアドレス */
} WINFO_MBX;

/*
 *  メッセージバッファ受信/送信待ち (TTW_MBF, TTW_SMBF)
 */
typedef struct {
	VP	msg;		/* 受信メッセージを入れるアドレス */
	INT	*p_msgsz;	/* 受信メッセージのサイズを入れるアドレス */
} WINFO_MBF;

typedef struct {
	VP	msg;		/* 送信メッセージの先頭アドレス */
	INT	msgsz;		/* 送信メッセージのサイズ */
} WINFO_SMBF;

/*
 *  ランデブ呼出/受付/終了待ち (TTW_CAL, TTW_ACP, TTW_RDV)
 */
typedef struct {
	UINT	calptn;		/* 呼出側選択条件を表すビットパターン */
	VP	msg;		/* メッセージを入れるアドレス */
	INT	cmsgsz;		/* 呼出メッセージのサイズ */
	INT	*p_rmsgsz;	/* 返答メッセージのサイズを入れるアドレス */
} WINFO_CAL;

typedef struct {
	UINT	acpptn;		/* 受付側選択条件を表すビットパターン */
	VP	msg;		/* 呼出メッセージを入れるアドレス */
	RNO	*p_rdvno;	/* ランデブ番号を入れるアドレス */
	INT	*p_cmsgsz;	/* 呼出メッセージのサイズを入れるアドレス */
} WINFO_ACP;

typedef struct {
	RNO	rdvno;		/* ランデブ番号 */
	VP	msg;		/* メッセージを入れるアドレス */
	INT	maxrmsz;	/* 返答メッセージの最大長 */
	INT	*p_rmsgsz;	/* 返答メッセージのサイズを入れるアドレス */
} WINFO_RDV;

/*
 *  タスク付属メイルボックス待ち (TTW_TMB)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* メッセージパケットの先頭を入れるアドレス */
} WINFO_TMB;

/*
 *  可変長メモリプール待ち (TTW_MPL)
 */
typedef struct {
	INT	blksz;		/* メモリブロックサイズ */
	VP	*p_blk;		/* メモリブロックの先頭を入れるアドレス */
} WINFO_MPL;

/*
 *  固定長メモリプール待ち (TTW_MPF)
 */
typedef struct {
	VP	*p_blf;		/* メモリブロックの先頭を入れるアドレス */
} WINFO_MPF;

/*
 *  タスクコントロールブロック中に持つ待ち情報の定義
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
 *  待ち仕様構造体の定義
 */

typedef const struct {
	UINT	tskwait;			/* 待ち要因 */
	void	(*chg_pri_hook)(TCB *, INT);	/* タスク優先度変更時の処理 */
	void	(*rel_wai_hook)(TCB *);		/* タスク待ち解除時の処理 */
} WSPEC;

#endif /* _WINFO_ */
