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
 *  @(#) $Id: itron.h,v 1.10 1998/01/30 07:09:02 hiro Exp $
 */

#ifndef _ITRON_
#define _ITRON_

/*
 *  汎用的なデータタイプ
 */

typedef	char		B;		/* 符号付き 8ビット整数 */
typedef	short		H;		/* 符号付き 16ビット整数 */
typedef	int		W;		/* 符号付き 32ビット整数 */
typedef	unsigned char	UB;		/* 符号無し 8ビット整数 */
typedef	unsigned short  UH;		/* 符号無し 16ビット整数 */
typedef	unsigned int	UW;		/* 符号無し 32ビット整数 */

typedef	char		VB;		/* 型が一定しない 8ビットのデータ */
typedef	short		VH;		/* 型が一定しない 16ビットのデータ */
typedef	int		VW;		/* 型が一定しない 32ビットのデータ */
typedef	void		*VP;		/* 型が一定しないデータへのポインタ */

typedef	void		(*FP)();	/* プログラムスタートアドレス一般 */

/*
 *  ITRON に依存したデータタイプ
 */

typedef int		INT;		/* 符号付き整数 */
typedef unsigned int	UINT;		/* 符号無し整数 */

typedef INT		BOOL;		/* ブール値 */
typedef INT		FN;		/* 機能コード */
	/* この定義は，μITRON3.0仕様にあっていない．*/
typedef	INT		ID;		/* オブジェクトのID番号 */
typedef	INT		BOOL_ID;	/* ブール値または ID番号 */
typedef	INT		HNO;		/* ハンドラ番号 */
typedef	INT		RNO;		/* ランデブ番号 */
typedef	INT		NODE;		/* ノード番号 */
typedef	UINT		ATR;		/* オブジェクト/ハンドラ属性 */
typedef	INT		ER;		/* エラーコード */
typedef	INT		PRI;		/* タスク優先度 */
typedef	INT		TMO;		/* タイムアウト指定 */
typedef long long	SYSTIME;	/* システムクロック */
typedef SYSTIME		CYCTIME;	/* 周期起動ハンドラ起動周期 */
typedef SYSTIME		ALMTIME;	/* アラームハンドラ起動時刻 */
typedef INT 		DLYTIME;	/* タスク遅延時間 */
/* ItIs 独自の型 */
typedef long long	SYSUTIME;	/* システム時刻 (性能評価用) */

/*
 *  共通定数
 */

#define NADR		((VP) -1)	/* アドレスやポインタ値が無効 */
#define TRUE		1		/* 真 */
#define FALSE		0		/* 偽 */

#define TA_NULL		0		/* 特別な属性を指定しない */

#define TA_ASM		0x00000000	/* アセンブラによるプログラム */
#define TA_HLNG		0x00000001	/* 高級言語によるプログラム */

#define TA_TFIFO	0x00000000	/* 待ちタスクを FIFO で管理 */
#define TA_TPRI		0x00000001	/* 待ちタスクを優先度順で管理 */

#define TMO_POL		0		/* ポーリング */
#define TMO_FEVR	(-1)		/* 永久待ち */

/*
 *  タスク管理関係
 */

/* cre_tsk */
typedef struct t_ctsk {
	VP	exinf;		/* 拡張情報 */
	ATR	tskatr;		/* タスク属性 */
	FP	task;		/* タスク起動アドレス */
	PRI	itskpri;	/* タスク起動時優先度 */
	INT	stksz;		/* ユーザスタックサイズ */
	/* ItIs 独自のフィールド */
	INT	sstksz;		/* システムスタックサイズ */
} T_CTSK;

#define TA_COP0		0x00008000	/* ID=0 のコプロセッサを使用 */
#define TA_COP1		0x00004000	/* ID=1 のコプロセッサを使用 */
#define TA_COP2		0x00002000	/* ID=2 のコプロセッサを使用 */
#define TA_COP3		0x00001000	/* ID=3 のコプロセッサを使用 */
#define TA_COP4		0x00000800	/* ID=4 のコプロセッサを使用 */
#define TA_COP5		0x00000400	/* ID=5 のコプロセッサを使用 */
#define TA_COP6		0x00000200	/* ID=6 のコプロセッサを使用 */
#define TA_COP7		0x00000100	/* ID=7 のコプロセッサを使用 */

/* ItIs 独自の属性 */
#define TA_SSTKSZ	0x00010000	/* システムスタックサイズを指定 */

/* ItIs 独自の属性 (TRON仕様チップ) */
#define TA_RNG0		0x00000000	/* リングレベル0 で実行 */
#define TA_RNG1		0x01000000	/* リングレベル1 で実行 */
#define TA_RNG2		0x02000000	/* リングレベル2 で実行 */
#define TA_RNG3		0x03000000	/* リングレベル3 で実行 */

/* ItIs 独自の属性 (68040) */
#define TA_SMODE	0x00000000	/* スーパバイザモードで実行 */
#define TA_UMODE	0x01000000	/* ユーザモードで実行 */

#define TSK_SELF	0		/* 自タスク指定 */

#define TPRI_INI	0	/* タスク起動時優先度を指定 (chg_pri) */
#define TPRI_RUN	0	/* 実行中の最高優先度を指定 (rot_rdq) */

/* ref_tsk */
typedef	struct t_rtsk {
	VP	exinf;		/* 拡張情報 */
	PRI	tskpri;		/* 現在の優先度 */
	UINT	tskstat;	/* タスク状態 */
	UINT	tskwait;	/* 待ち要因 */
	ID	wid;		/* 待ちオブジェクトID */
	INT	wupcnt;		/* 起床要求キューイング数 */
	INT	suscnt;		/* SUSPEND要求ネスト数 */
} T_RTSK;

#define TTS_RUN		0x00000001	/* RUN */
#define TTS_RDY		0x00000002	/* READY */
#define TTS_WAI		0x00000004	/* WAIT */
#define TTS_SUS		0x00000008	/* SUSPEND */
#define TTS_WAS		0x0000000c	/* WAIT-SUSPEND */
#define TTS_DMT		0x00000010	/* DORMANT */

#define TTW_SLP		0x00000001	/* 起床待ちによる待ち */
#define TTW_DLY		0x00000002	/* タスクの遅延による待ち */
#define TTW_NOD		0x00000008	/* 接続機能応答待ち */
#define TTW_FLG		0x00000010	/* イベントフラグ待ち */
#define TTW_SEM		0x00000020	/* セマフォ待ち */
#define TTW_MBX		0x00000040	/* メイルボックス待ち */
#define TTW_SMBF	0x00000080	/* メッセージバッファ送信待ち */
#define TTW_MBF		0x00000100	/* メッセージバッファ受信待ち */
#define TTW_CAL		0x00000200	/* ランデブ呼出待ち */
#define TTW_ACP		0x00000400	/* ランデブ受付待ち */
#define TTW_RDV		0x00000800	/* ランデブ終了待ち */
#define TTW_MPL		0x00001000	/* 可変長メモリプール待ち */
#define TTW_MPF		0x00002000	/* 固定長メモリプール待ち */
/* ItIs 独自の定義 */
#define TTW_PIS		0x00010000	/* 優先度継承セマフォ待ち */
#define TTW_TMB		0x00020000	/* タスク付属メイルボックス待ち */

/*
 *  セマフォ関係
 */

/* cre_sem */
typedef	struct t_csem {
	VP	exinf;		/* 拡張情報 */
	ATR	sematr;		/* セマフォ属性 */
	INT	isemcnt;	/* セマフォの初期カウント値 */
	INT	maxsem;		/* セマフォの最大カウント値 */
} T_CSEM;

/* ref_sem */
typedef	struct t_rsem {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	INT	semcnt;		/* 現在のセマフォカウント値 */
} T_RSEM;

/*
 *  イベントフラグ関係
 */

/* cre_flg */
typedef	struct t_cflg {
	VP	exinf;		/* 拡張情報 */
	ATR	flgatr;		/* イベントフラグ属性 */
	UINT	iflgptn;	/* イベントフラグの初期パターン */
} T_CFLG;

#define TA_WSGL		0x00000000	/* 複数タスクの待ちを許さない */
#define TA_WMUL		0x00000008	/* 複数タスクの待ちを許す */

#define TWF_ANDW	0x00000000	/* AND待ち */
#define TWF_ORW		0x00000002	/* OR待ち */
#define TWF_CLR		0x00000001	/* クリア指定 */

/* ref_flg */
typedef	struct t_rflg {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	UINT	flgptn;		/* 現在のイベントフラグパターン */
} T_RFLG;

/*
 *  メイルボックス関係
 */

/* cre_mbx */
typedef	struct t_cmbx {
	VP	exinf;		/* 拡張情報 */
	ATR	mbxatr;		/* メイルボックス属性 */
} T_CMBX;

#define TA_MFIFO	0x00000000	/* メッセージを FIFO で管理 */
#define TA_MPRI		0x00000002	/* メッセージを優先度順で管理 */

typedef struct t_msg {
	VP	msgque[1];	/* メッセージキューのためのエリア */
	PRI	msgpri;		/* メッセージ優先度 */
	VB	msgcont[0];	/* メッセージ本体 */
} T_MSG;

/* ref_mbx */
typedef	struct t_rmbx {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	T_MSG	*pk_msg;	/* 次に受信されるメッセージ */
} T_RMBX;

/*
 *  メッセージバッファ関係
 */

/* cre_mbf */
typedef	struct t_cmbf {
	VP	exinf;		/* 拡張情報 */
	ATR	mbfatr;		/* メッセージバッファ属性 */
	INT	bufsz;		/* メッセージバッファのサイズ */
	INT	maxmsz;		/* メッセージの最大長 */
} T_CMBF;

#define TMBF_OS		(-4)	/* OS のエラーログ用のメッセージバッファ */
#define TMBF_DB		(-3)	/* デバッグ用のメッセージバッファ */

/* ref_mbf */
typedef struct t_rmbf {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 受信待ちタスクの有無 */
	BOOL_ID	stsk;		/* 送信待ちタスクの有無 */
	INT	msgsz;		/* 次に受信されるメッセージのサイズ */
	INT	frbufsz;	/* 空きバッファのサイズ */
} T_RMBF;

/*
 *  ランデブ，ランデブ用ポート関係
 */

/* cre_por */
typedef	struct t_cpor {
	VP	exinf;		/* 拡張情報 */
	ATR	poratr;		/* ポート属性 */
	INT	maxcmsz;	/* 呼出メッセージの最大長 */
	INT	maxrmsz;	/* 返答メッセージの最大長 */
} T_CPOR;

/* ref_por */
typedef struct t_rpor {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 呼出待ちタスクの有無 */
	BOOL_ID	atsk;		/* 受付待ちタスクの有無 */
} T_RPOR;

/*
 *  優先度継承セマフォ関係 (ItIs 独自の定義)
 */

/* vcre_pis */
typedef	struct t_cpis {
	VP	exinf;		/* 拡張情報 */
	ATR	pisatr;		/* 優先度継承セマフォ属性 */
} T_CPIS;

/* vref_pis */
typedef	struct t_rpis {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	BOOL_ID	pistsk;		/* セマフォ取得タスクの有無 */
} T_RPIS;

/*
 *  タスク付属メイルボックス関係 (ItIs 独自の定義)
 */

/* vref_tmb */
typedef	struct t_rtmb {
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	T_MSG	*pk_msg;	/* 次に受信されるメッセージ */
} T_RTMB;

/*
 *  割込み管理関係
 */

/* def_int */
typedef struct t_dint {
	ATR	intatr;		/* 割込みハンドラ属性 */
	FP	inthdr;		/* 割込みハンドラアドレス */
	/* ItIs 独自のフィールド (TRON仕様チップ) */
	UINT	eitatr;		/* EIT属性 */
} T_DINT;

/* ItIs 独自の属性 (TRON仕様チップ) */
#define TA_EITATR	0x00010000	/* EIT属性を指定 */

/*
 *  メモリプール関係
 */

/* cre_mpl */
typedef	struct t_cmpl {
	VP	exinf;		/* 拡張情報 */
	ATR	mplatr;		/* メモリプール属性 */
	INT	mplsz;		/* メモリプール全体のサイズ */
} T_CMPL;

/* ItIs 独自の属性 */
#define TA_HFIT		0x00000000	/* half-fit アルゴリズム */
#define TA_FFIT		0x00010000	/* first-fit アルゴリズム */

#define TMPL_OS		(-4)		/* OS用メモリプール */

/* ref_mpl */
typedef struct t_rmpl {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	INT	frsz;		/* 空き領域の合計サイズ */
	INT	maxsz;		/* 最大の連続空き領域のサイズ */
} T_RMPL;

/* cre_mpf */
typedef	struct t_cmpf {
	VP	exinf;		/* 拡張情報 */
	ATR	mpfatr;		/* メモリプール属性 */
	INT	mpfcnt;		/* メモリプール全体のブロック数 */
	INT	blfsz;		/* 固定長メモリブロックのサイズ */
} T_CMPF;

/* ref_mpf */
typedef struct t_rmpf {
	VP	exinf;		/* 拡張情報 */
	BOOL_ID	wtsk;		/* 待ちタスクの有無 */
	INT	frbcnt;		/* 空き領域のブロック数 */
} T_RMPF;

/*
 *  時間管理関係
 */

/* def_cyc */
typedef struct t_dcyc {
	VP	exinf;		/* 拡張情報 */
	ATR	cycatr;		/* 周期起動ハンドラ属性 */
	FP	cychdr;		/* 周期起動ハンドラアドレス */
	UINT	cycact;		/* 周期起動ハンドラ活性状態 */
	CYCTIME	cyctim;		/* 周期起動時間間隔 */
} T_DCYC;

#define TCY_OFF		0x00000000	/* 周期起動ハンドラを起動しない */
#define TCY_ON		0x00000001	/* 周期起動ハンドラを起動する */
#define TCY_INI		0x00000002	/* 周期のカウントを初期化する */

/* ref_cyc */
typedef struct t_rcyc {
	VP	exinf;		/* 拡張情報 */
	CYCTIME	lfttim;		/* 次のハンドラ起動までの残り時間 */
	UINT	cycact;		/* 周期起動ハンドラ活性状態 */
} T_RCYC;

/* def_alm */
typedef struct t_dalm {
	VP	exinf;		/* 拡張情報 */
	ATR	almatr;		/* アラームハンドラ属性 */
	FP	almhdr;		/* アラームハンドラアドレス */
	UINT	tmmode;		/* 起動時刻指定モード */
	ALMTIME	almtim;		/* ハンドラ起動時刻 */
} T_DALM;

#define TTM_ABS		0x00000000	/* 絶対時刻での指定 */
#define TTM_REL		0x00000001	/* 相対時刻での指定 */

/* ref_alm */
typedef struct t_ralm {
	VP	exinf;		/* 拡張情報 */
	ALMTIME	lfttim;		/* ハンドラ起動までの残り時間 */
} T_RALM;

/*
 *  システム管理関係
 */

/* get_ver */
typedef struct t_ver {
	UH	maker;		/* メーカー */
	UH	id;		/* 形式番号 */
	UH	spver;		/* 仕様書バージョン */
	UH	prver;		/* 製品バージョン */
	UH	prno[4];	/* 製品管理情報 */
	UH	cpu;		/* CPU情報 */
	UH	var;		/* バリエーション記述子 */
} T_VER;

/* ref_sys */
typedef struct t_rsys {
	INT	sysstat;	/* システム状態 */
	ID	runtskid;	/* 実行状態にあるタスクの ID */
	ID	schedtskid;	/* 実行状態にすべきタスクの ID */
} T_RSYS;

#define TTS_TSK		0	/* タスク部実行中 */
#define TTS_DDSP	1	/* ディスパッチ禁止中 */
#define TTS_LOC		3	/* 割込みおよびディスパッチ禁止中 */
#define TTS_INDP	4	/* タスク独立部実行中 */
/* ItIs 独自の定義 */
#define TTS_QTSK	8	/* 準タスク部実行中 */

/* ref_cfg */
typedef struct t_rcfg {
	/* ItIs 独自のフィールド */
} T_RCFG;

/* def_svc */
typedef struct t_dsvc {
	ATR	svcatr;		/* 拡張SVCハンドラ属性 */
	FP	svchdr;		/* 拡張SVCハンドラアドレス */
} T_DSVC;

/* def_exc */
typedef struct t_dexc {
	ATR	excatr;		/* 例外ハンドラ属性 */
	FP	exchdr;		/* 例外ハンドラアドレス */
} T_DEXC;

/*
 *  ネットワーク管理関係
 */

#define TND_SELF	0	/* 自ノード指定 */
#define TND_OTHR	(-1)	/* デフォルトの他ノード指定 */

#endif /* _ITRON_ */
