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
 *  @(#) $Id: isysconf.h,v 1.15 1997/01/10 13:36:22 hiro Exp $
 */

#ifndef _ISYSCONF_
#define _ISYSCONF_

/*
 *  システム構成設定ファイルに対するデフォルト値の定義
 */

/*
 *  システムメモリプールサイズのデフォルト値
 */
#ifndef SYS_MPL_SIZE
#define SYS_MPL_SIZE	0x60000
#endif /* SYS_MPL_SIZE */

/*
 *  優先度継承セマフォの仕様のデフォルト値
 */
#ifndef PRISEM_SPEC
#define PRISEM_SPEC	1
#endif /* PRISEM_SPEC */

/*
 *  システム構成に関する補助定義
 */

/*
 *  システムメモリプールを使うかどうかの定義
 */
#if defined(NUM_MPLID) && CHK_MPLID(TMPL_OS)
#define USE_TMPL_OS
#endif

/*
 *  システムログ用メッセージバッファを使うかどうかの定義
 */
#if defined(NUM_MBFID) && CHK_MBFID(TMBF_OS)
#define USE_TMBF_OS
#endif

/*
 *  優先度継承セマフォの仕様に関連する定義
 */
#ifdef NUM_PISID

#if PRISEM_SPEC == 1
#define PRISEM_SPEC1
#else
#if PRISEM_SPEC == 2
#define PRISEM_SPEC2
#else
#error unknown priority inheritance semaphore specification.
#endif
#endif

#endif /* NUM_PISID */

/*
 *  タスク待ち解除時の処理フックを使うかどうかの定義
 *
 *  仕様1 の優先度継承セマフォを使う場合には，タスク待ち解除時の処理フッ
 *  クが必要．
 */
#ifdef PRISEM_SPEC1
#define USE_REL_WAI_HOOK
#endif /* PRISEM_SPEC1 */

/*
 *  使用しない機能ブロックの決定
 */

#ifdef NUM_SEMID
#define USE_SEM
#endif

#ifdef NUM_FLGID
#define USE_FLG
#endif

#ifdef NUM_MBXID
#define USE_MBX
#endif

#ifdef NUM_MBFID
#define USE_MBF
#endif

#ifdef NUM_PORID
#define USE_POR
#endif

#ifdef NUM_PISID
#define USE_PIS
#endif

#ifdef NUM_MPLID
#define USE_MPL
#endif

#ifdef NUM_MPFID
#define USE_MPF
#endif

#ifdef NUM_CYC
#define USE_CYC
#endif

#ifdef NUM_ALM
#define USE_ALM
#endif

#ifdef NUM_SVC
#define USE_SVC
#endif

/*
 *  使用しないシステムコールに関する定義
 */

#ifndef USE_SEM
#define _i_cre_sem	_no_support
#define _i_vcre_sem	_no_support
#define _i_del_sem	_no_support
#define _i_sig_sem	_no_support
#define _i_wai_sem	_no_support
#define _i_preq_sem	_no_support
#define _i_twai_sem	_no_support
#define _i_ref_sem	_no_support
#endif /* USE_SEM */

#ifndef USE_FLG
#define _i_cre_flg	_no_support
#define _i_vcre_flg	_no_support
#define _i_del_flg	_no_support
#define _i_set_flg	_no_support
#define _i_clr_flg	_no_support
#define _i_wai_flg	_no_support
#define _i_pol_flg	_no_support
#define _i_twai_flg	_no_support
#define _i_ref_flg	_no_support
#endif /* USE_FLG */

#ifndef USE_MBX
#define _i_cre_mbx	_no_support
#define _i_vcre_mbx	_no_support
#define _i_del_mbx	_no_support
#define _i_snd_msg	_no_support
#define _i_rcv_msg	_no_support
#define _i_prcv_msg	_no_support
#define _i_trcv_msg	_no_support
#define _i_ref_mbx	_no_support
#endif /* USE_MBX */

#ifndef USE_MBF
#define _i_cre_mbf	_no_support
#define _i_vcre_mbf	_no_support
#define _i_del_mbf	_no_support
#define _i_snd_mbf	_no_support
#define _i_psnd_mbf	_no_support
#define _i_tsnd_mbf	_no_support
#define _i_rcv_mbf	_no_support
#define _i_prcv_mbf	_no_support
#define _i_trcv_mbf	_no_support
#define _i_ref_mbf	_no_support
#endif /* USE_MBF */

#ifndef USE_POR
#define _i_cre_por	_no_support
#define _i_vcre_por	_no_support
#define _i_del_por	_no_support
#define _i_cal_por	_no_support
#define _i_pcal_por	_no_support
#define _i_tcal_por	_no_support
#define _i_acp_por	_no_support
#define _i_pacp_por	_no_support
#define _i_tacp_por	_no_support
#define _i_fwd_por	_no_support
#define _i_rpl_rdv	_no_support
#define _i_ref_por	_no_support
#endif /* USE_POR */

#ifndef USE_PIS
#define _i_vcre_pis	_no_support
#define _i_vvcre_pis	_no_support
#define _i_vdel_pis	_no_support
#define _i_vsig_pis	_no_support
#define _i_vwai_pis	_no_support
#define _i_vpreq_pis	_no_support
#define _i_vtwai_pis	_no_support
#define _i_vref_pis	_no_support
#endif /* USE_PIS */

#ifndef USE_TASK_MAILBOX
#define _i_vsnd_tmb	_no_support
#define _i_vtrcv_tmb	_no_support
#define _i_vprcv_tmb	_no_support
#define _i_vrcv_tmb	_no_support
#define _i_vref_tmb	_no_support
#endif /* USE_TASK_MAILBOX */

#ifndef USE_MPL
#define _i_cre_mpl	_no_support
#define _i_vcre_mpl	_no_support
#define _i_del_mpl	_no_support
#define _i_get_blk	_no_support
#define _i_pget_blk	_no_support
#define _i_tget_blk	_no_support
#define _i_rel_blk	_no_support
#define _i_ref_mpl	_no_support
#endif /* USE_MPL */

#ifndef USE_MPF
#define _i_cre_mpf	_no_support
#define _i_vcre_mpf	_no_support
#define _i_del_mpf	_no_support
#define _i_get_blf	_no_support
#define _i_pget_blf	_no_support
#define _i_tget_blf	_no_support
#define _i_rel_blf	_no_support
#define _i_ref_mpf	_no_support
#endif /* USE_MPF */

#ifndef USE_CYC
#define _i_def_cyc	_no_support
#define _i_vdef_cyc	_no_support
#define _i_act_cyc	_no_support
#define _i_ref_cyc	_no_support
#endif /* USE_CYC */

#ifndef USE_ALM
#define _i_def_alm	_no_support
#define _i_vdef_alm	_no_support
#define _i_ref_alm	_no_support
#endif /* USE_ALM */

#ifndef USE_SVC
#define _i_def_svc	_no_support
#endif /* USE_SVC */

#ifndef USE_AUTO_ID
#define _i_vcre_tsk	_no_support
#define _i_vcre_sem	_no_support
#define _i_vcre_flg	_no_support
#define _i_vcre_mbx	_no_support
#define _i_vcre_mbf	_no_support
#define _i_vcre_por	_no_support
#define _i_vvcre_pis	_no_support
#define _i_vcre_mpl	_no_support
#define _i_vcre_mpf	_no_support
#define _i_vdef_cyc	_no_support
#define _i_vdef_alm	_no_support
#endif /* USE_AUTO_ID */

#ifndef USE_DEBUG_SUPPORT
#define _i_vget_reg	_no_support
#define _i_vset_reg	_no_support
#endif /* USE_DEBUG_SUPPORT */

#ifndef USE_VGET_TIM
#define _i_vget_tim	_no_support
#endif /* USE_VGET_TIM */

#endif /* _ISYSCONF_ */
