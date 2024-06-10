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
 *  @(#) $Id: misc_calls.c,v 1.17 1998/07/06 04:42:10 hiro Exp $
 */

/* 
 *  その他のシステムコール
 */

#include "itis_kernel.h"
#include "task.h"
#include "check.h"
#include "patchlevel.h"

/*
 *  システム状態参照
 */

#ifndef _i_ref_sys

SYSCALL ER
i_ref_sys(T_RSYS *pk_rsys)
{
	if (in_indp()) {
		pk_rsys->sysstat = TTS_INDP;
	}
	else {
		if (in_qtsk()) {
			pk_rsys->sysstat = TTS_QTSK;
		}
		else {
			pk_rsys->sysstat = TTS_TSK;
		}
		if (in_loc()) {
			pk_rsys->sysstat |= TTS_LOC;
		}
		else if (in_ddsp()) {
			pk_rsys->sysstat |= TTS_DDSP;
		}
	}
	pk_rsys->runtskid = (ctxtsk ? ctxtsk->tskid : FALSE);
	pk_rsys->schedtskid = (schedtsk ? schedtsk->tskid : FALSE);
	return(E_OK);
}

#endif /* _i_ref_sys */

/*
 *  コンフィグレーション情報参照
 */

#ifndef _i_ref_cfg

SYSCALL ER
i_ref_cfg(T_RCFG *pk_rcfg)
{
	return(E_OK);
}

#endif /* _i_ref_cfg */

/*
 *  拡張SVC 管理機能
 */

#ifdef USE_SVC

/*
 *  拡張SVC 管理ブロックの定義
 */

typedef ER	(*SVC)();	/* 拡張SVCハンドラアドレスの型 */

typedef struct extended_svc_control_block {
	ATR	svcatr;		/* 拡張SVCハンドラ属性 */
	SVC	svchdr;		/* 拡張SVCハンドラアドレス */
} SVCCB;

SVCCB	svccb_table[NUM_SVC];

#define get_svccb(fn)	(&(svccb_table[(fn)-1]))

extern ER	no_support(void);

/* 
 *  拡張SVC 管理ブロックの初期化
 */
void
extended_svc_initialize(void)
{
	INT	i;

	for (i = 0; i < NUM_SVC; i++) {
		svccb_table[i].svchdr = no_support;
	}
}

/*
 *  拡張SVC の定義
 */

#ifndef _i_def_svc

SYSCALL ER
i_def_svc(FN s_fncd, T_DSVC *pk_dsvc)
{
	SVCCB	*svccb;
	ER	ercd = E_OK;

	CHECK_PAR(0 < s_fncd && s_fncd <= NUM_SVC);
	if (pk_dsvc != NADR) {
		CHECK_RSATR(pk_dsvc->svcatr, TA_HLNG);
	}
	svccb = get_svccb(s_fncd);

	BEGIN_CRITICAL_SECTION;
	if (pk_dsvc == NADR) {
		svccb->svchdr = no_support;
	}
	else {
		svccb->svcatr = pk_dsvc->svcatr;
		svccb->svchdr = (SVC) pk_dsvc->svchdr;
	}
	END_CRITICAL_SECTION;
	return(ercd);
}

#endif /* _i_def_svc */

/*
 *  拡張SVCハンドラへの分岐ルーチン
 *
 *  s_fncd で指定する拡張SVCハンドラへ分岐する．ientry.s の中から呼ば
 *  れる．ctxtsk->sysmode に，準タスク部実行中であることを記録する．
 */
ER
svc_ientry(INT par1, INT par2, INT par3, INT par4, INT par5, INT par6,
		FN s_fncd)
{
	ER	ercd;

	if (s_fncd > NUM_SVC) {
		return(E_RSFN);
	}

	if (!in_indp()) {
		ctxtsk->sysmode++;
		ercd = (*(get_svccb(s_fncd)->svchdr))
				(par1, par2, par3, par4, par5, par6);
		ctxtsk->sysmode--;
	}
	else {
		ercd = (*(get_svccb(s_fncd)->svchdr))
				(par1, par2, par3, par4, par5, par6);
	}
	return(ercd);
}

#endif /* USE_SVC */

/*
 *  バージョン情報参照
 */

#ifndef _i_get_ver

SYSCALL ER
i_get_ver(T_VER *pk_ver)
{
	pk_ver->maker = 0x0000;		/* 実験用システム */
	pk_ver->id = 0x5004;		/* (深い意味はない) */
	pk_ver->spver = 0x5302;		/* μITRON, Ver.3.02 */
	pk_ver->prver = (MAJOR_REL << 12) + (MINOR_REL << 4) + PATCH_LEVEL;
					/* Release X.YY.Z */
	memset(pk_ver->prno, 0, sizeof(pk_ver->prno));
					/* (未使用) */
	pk_ver->cpu = CPU_CODE;		/* CPUコード (cpu_conf.h で定義) */
	pk_ver->var = 0xc000;		/* レベルE */
	return(E_OK);
}

#endif /* _i_get_ver */
