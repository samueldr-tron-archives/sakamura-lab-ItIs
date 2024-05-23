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
 *  @(#) $Id: check.h,v 1.9 1996/02/17 09:32:50 hiro Exp $
 */

#ifndef _CHECK_
#define _CHECK_

/*
 *  エラーチェック用マクロの定義
 */

/*
 *  オブジェクトID の範囲のチェック (E_ID)
 */

#ifdef CHK_ID

#define CHECK_TSKID(tskid) {					\
	if (!CHK_TSKID(tskid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_TSKID_SELF(tskid) {				\
	if (!((!in_indp() && (tskid) == TSK_SELF)		\
				|| CHK_TSKID(tskid))) {		\
		return(E_ID);					\
	}							\
}

#define CHECK_SEMID(semid) {					\
	if (!CHK_SEMID(semid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_FLGID(flgid) {					\
	if (!CHK_FLGID(flgid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_MBXID(mbxid) {					\
	if (!CHK_MBXID(mbxid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_MBFID(mbfid) {					\
	if (!CHK_MBFID(mbfid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_PORID(porid) {					\
	if (!CHK_PORID(porid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_PISID(pisid) {					\
	if (!CHK_PISID(pisid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_MPLID(mplid) {					\
	if (!CHK_MPLID(mplid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_MPLID_OS(mplid) {					\
	if (!CHK_MPLID(mplid) || mplid == TMPL_OS) {		\
		return(E_ID);					\
	}							\
}

#define CHECK_MPFID(mpfid) {					\
	if (!CHK_MPFID(mpfid)) {				\
		return(E_ID);					\
	}							\
}

#else /* CHK_ID */
#define CHECK_TSKID(tskid)
#define CHECK_TSKID_SELF(tskid)
#define CHECK_SEMID(semid)
#define CHECK_FLGID(flgid)
#define CHECK_MBXID(mbxid)
#define CHECK_MBFID(mbfid)
#define CHECK_PORID(porid)
#define CHECK_PISID(pisid)
#define CHECK_MPLID(mplid)
#define CHECK_MPLID_OS(mplid)
#define CHECK_MPFID(mpfid)
#endif /* CHK_ID */

/*
 *  オブジェクトのアクセス権のチェック (E_OACV)
 */

#ifdef CHK_OACV

#define CHECK_TSKACV(tskid) {					\
	if (SYS_TSKID(tskid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_SEMACV(semid) {					\
	if (SYS_SEMID(semid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_FLGACV(flgid) {					\
	if (SYS_FLGID(flgid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_MBXACV(mbxid) {					\
	if (SYS_MBXID(mbxid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_MBFACV(mbfid) {					\
	if (SYS_MBFID(mbfid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_PORACV(porid) {					\
	if (SYS_PORID(porid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_PISACV(pisid) {					\
	if (SYS_PISID(pisid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_MPLACV(mplid) {					\
	if (SYS_MPLID(mplid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#define CHECK_MPFACV(mpfid) {					\
	if (SYS_MPFID(mpfid) && !in_sysmode()) {		\
		return(E_OACV);					\
	}							\
}

#else /* CHK_OACV */
#define CHECK_TSKACV(tskid)
#define CHECK_SEMACV(semid)
#define CHECK_FLGACV(flgid)
#define CHECK_MBXACV(mbxid)
#define CHECK_MBFACV(mbfid)
#define CHECK_PORACV(porid)
#define CHECK_PISACV(pisid)
#define CHECK_MPLACV(mplid)
#define CHECK_MPFACV(mpfid)
#endif /* CHK_OACV */

/*
 *  自タスクを指定していないかのチェック (E_OBJ)
 */
#ifdef CHK_SELF
#define CHECK_NONSELF(tskid) {					\
	if (!in_indp() && (tskid) == ctxtsk->tskid) {		\
		return(E_OBJ);					\
	}							\
}
#else /* CHK_SELF */
#define CHECK_NONSELF(tskid)
#endif /* CHK_SELF */

/*
 *  優先度値のチェック (E_PAR)
 */

#ifdef CHK_PAR

#define CHECK_PRI(pri) {					\
	if (!CHK_PRI(pri)) {					\
		return(E_PAR);					\
	}							\
}

#define CHECK_PRI_INI(pri) {					\
	if ((pri) != TPRI_INI && !CHK_PRI(pri)) {		\
		return(E_PAR);					\
	}							\
}

#define CHECK_PRI_RUN(pri) {					\
	if ((pri) != TPRI_RUN && !CHK_PRI(pri)) {		\
		return(E_PAR);					\
	}							\
}

#else /* CHK_PAR */
#define CHECK_PRI(pri)
#define CHECK_PRI_INI(pri)
#define CHECK_PRI_RUN(pri)
#endif /* CHK_PAR */

/*
 *  タイムアウト指定値のチェック (E_PAR)
 */
#ifdef CHK_PAR
#define CHECK_TMOUT(tmout) {					\
	if (!((tmout) >= TMO_FEVR)) {				\
		return(E_PAR);					\
	}							\
}
#else /* CHK_PAR */
#define CHECK_TMOUT(tmout)
#endif /* CHK_PAR */

/*
 *  その他のパラメータエラーのチェック (E_PAR)
 */
#ifdef CHK_PAR
#define CHECK_PAR(exp) {					\
	if (!(exp)) {						\
		return(E_PAR);					\
	}							\
}
#else /* CHK_PAR */
#define CHECK_PAR(exp)
#endif /* CHK_PAR */

/*
 *  予約属性エラーのチェック (E_RSATR)
 */
#ifdef CHK_RSATR
#define CHECK_RSATR(atr, maxatr) {				\
	if ((atr) & ~(maxatr)) {				\
	        return(E_RSATR);				\
	}							\
}
#else /* CHK_RSATR */
#define CHECK_RSATR(atr, maxatr)
#endif /* CHK_RSATR */

/*
 *  未サポート機能のチェック (E_NOSPT)
 */
#ifdef CHK_NOSPT
#define CHECK_NOSPT(exp) {					\
	if (!(exp)) {						\
		return(E_NOSPT);				\
	}							\
}
#else /* CHK_NOSPT */
#define CHECK_NOSPT(exp)
#endif /* CHK_NOSPT */

/*
 *  タスク独立部実行中でないかのチェック (E_CTX)
 */
#ifdef CHK_CTX
#define CHECK_INTSK() {						\
	if (in_indp()) {					\
		return(E_CTX);					\
	}							\
}
#else /* CHK_CTX */
#define CHECK_INTSK()
#endif /* CHK_CTX */

/*
 *  ディスパッチ禁止状態でないかのチェック (E_CTX)
 */

#ifdef CHK_CTX

#define CHECK_DISPATCH() {					\
	if (in_ddsp()) {					\
		return(E_CTX);					\
	}							\
}

#define CHECK_DISPATCH_POL(tmout) {				\
	if ((tmout) != TMO_POL && in_ddsp()) {			\
		return(E_CTX);					\
	}							\
}

#else /* CHK_CTX */
#define CHECK_DISPATCH()
#define CHECK_DISPATCH_POL(tmout)
#endif /* CHK_CTX */

/*
 *  その他のコンテキストエラーのチェック (E_CTX)
 */
#ifdef CHK_CTX
#define CHECK_CTX(exp) {					\
	if (!(exp)) {						\
		return(E_CTX);					\
	}							\
}
#else /* CHK_CTX */
#define CHECK_CTX(exp)
#endif /* CHK_CTX */

#endif /* _CHECK_ */
