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
 *  @(#) $Id: itron_errno.h,v 1.5 1996/02/17 09:26:52 hiro Exp $
 */

#ifndef _ITRON_ERRNO_
#define _ITRON_ERRNO_

/* 
 *  ITRON のエラーコードの定義
 */

#define	E_OK		0		/* 正常終了 */

#define E_SYS		(-5)		/* システムエラー */
#define	E_NOMEM		(-10)		/* メモリ不足 */
#define	E_NOSPT		(-17)		/* 未サポート機能 */
#define	E_INOSPT	(-18)		/* ITRON/FILE での未サポート機能 */
#define	E_RSFN		(-20)		/* 予約機能番号 */
#define	E_RSATR		(-24)		/* 予約属性 */
#define	E_PAR		(-33)		/* パラメターエラー */
#define	E_ID		(-35)		/* 不正ID番号 */
#define	E_NOEXS		(-52)		/* オブジェクトが存在していない */
#define	E_OBJ		(-63)		/* オブジェクトの状態が不正 */
#define	E_MACV		(-65)		/* メモリアクセス不能/アクセス権違反 */
#define	E_OACV		(-66)		/* オブジェクトアクセス権違反 */
#define	E_CTX		(-69)		/* コンテキストエラー */
#define	E_QOVR		(-73)		/* キューイングのオーバーフロー */
#define	E_DLT		(-81)		/* 待ちオブジェクトが削除された */
#define	E_TMOUT		(-85)		/* ポーリング失敗/タイムアウト */
#define	E_RLWAI		(-86)		/* 待ち状態強制解除 */

#define	EN_NOND		(-113)		/* 対象ノードが存在しない */
#define	EN_OBJNO	(-114)		/* アクセスできないオブジェクト番号 */
#define	EN_PROTO	(-115)		/* 未サポートプロトコル */
#define	EN_RSFN		(-116)		/* 未サポートシステムコール */
#define	EN_COMM		(-117)		/* 対象ノードからの応答がない */
#define	EN_RLWAI	(-118)		/* 接続機能に関わる待ち状態強制解除 */
#define	EN_PAR		(-119)		/* 要求時のパラメータエラー */
#define	EN_RPAR		(-120)		/* リターン時のパラメータエラー */
#define	EN_CTXID	(-121)		/* 接続機能を使えないコンテキスト */
#define	EN_EXEC		(-122)		/* 対象ノードにおける資源不足 */
#define	EN_NOSPT	(-123)		/* 未サポート接続機能 */

#define EV_FULL		(-225)		/* オブジェクトテーブルフル */

#endif /* _ITRON_ERRNO_ */
