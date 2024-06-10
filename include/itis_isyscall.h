/*  This file is generated from syscall.def by genisyscall.  */

#ifndef _ITIS_ISYSCALL_
#define _ITIS_ISYSCALL_

#include <itis_scint.h>

#ifdef LINK_KERNEL

#define unl_cpu i_unl_cpu
#define loc_cpu i_loc_cpu
#define def_svc i_def_svc
#define ref_sys i_ref_sys
#define ref_cfg i_ref_cfg
#define get_ver i_get_ver
#define cre_tsk i_cre_tsk
#define del_tsk i_del_tsk
#define ref_tsk i_ref_tsk
#define ext_tsk i_ext_tsk
#define exd_tsk i_exd_tsk
#define sta_tsk i_sta_tsk
#define get_tid i_get_tid
#define ter_tsk i_ter_tsk
#define chg_pri i_chg_pri
#define rot_rdq i_rot_rdq
#define ena_dsp i_ena_dsp
#define dis_dsp i_dis_dsp
#define rel_wai i_rel_wai
#define sus_tsk i_sus_tsk
#define rsm_tsk i_rsm_tsk
#define frsm_tsk i_frsm_tsk
#define tslp_tsk i_tslp_tsk
#define slp_tsk i_slp_tsk
#define wup_tsk i_wup_tsk
#define can_wup i_can_wup
#define cre_flg i_cre_flg
#define del_flg i_del_flg
#define ref_flg i_ref_flg
#define wai_flg i_wai_flg
#define clr_flg i_clr_flg
#define set_flg i_set_flg
#define cre_sem i_cre_sem
#define del_sem i_del_sem
#define ref_sem i_ref_sem
#define wai_sem i_wai_sem
#define sig_sem i_sig_sem
#define cre_mbx i_cre_mbx
#define del_mbx i_del_mbx
#define ref_mbx i_ref_mbx
#define rcv_msg i_rcv_msg
#define snd_msg i_snd_msg
#define def_int i_def_int
#define ena_int i_ena_int
#define dis_int i_dis_int
#define cre_mpf i_cre_mpf
#define del_mpf i_del_mpf
#define ref_mpf i_ref_mpf
#define get_blf i_get_blf
#define rel_blf i_rel_blf
#define vget_tim i_vget_tim
#define set_tim i_set_tim
#define get_tim i_get_tim
#define dly_tsk i_dly_tsk
#define def_alm i_def_alm
#define def_cyc i_def_cyc
#define ref_alm i_ref_alm
#define ref_cyc i_ref_cyc
#define act_cyc i_act_cyc
#define pget_blf i_pget_blf
#define pget_blk i_pget_blk
#define pol_flg i_pol_flg
#define preq_sem i_preq_sem
#define prcv_msg i_prcv_msg
#define psnd_mbf i_psnd_mbf
#define prcv_mbf i_prcv_mbf
#define pcal_por i_pcal_por
#define pacp_por i_pacp_por
#define cre_mpl i_cre_mpl
#define del_mpl i_del_mpl
#define ref_mpl i_ref_mpl
#define get_blk i_get_blk
#define rel_blk i_rel_blk
#define cre_por i_cre_por
#define del_por i_del_por
#define ref_por i_ref_por
#define cal_por i_cal_por
#define acp_por i_acp_por
#define rpl_rdv i_rpl_rdv
#define fwd_por i_fwd_por
#define cre_mbf i_cre_mbf
#define del_mbf i_del_mbf
#define ref_mbf i_ref_mbf
#define snd_mbf i_snd_mbf
#define rcv_mbf i_rcv_mbf
#define tget_blf i_tget_blf
#define tget_blk i_tget_blk
#define twai_flg i_twai_flg
#define twai_sem i_twai_sem
#define trcv_msg i_trcv_msg
#define tsnd_mbf i_tsnd_mbf
#define trcv_mbf i_trcv_mbf
#define tcal_por i_tcal_por
#define tacp_por i_tacp_por
#define vset_reg i_vset_reg
#define vget_reg i_vget_reg
#define vcre_tsk i_vcre_tsk
#define vcre_sem i_vcre_sem
#define vcre_flg i_vcre_flg
#define vcre_mbx i_vcre_mbx
#define vcre_mbf i_vcre_mbf
#define vcre_por i_vcre_por
#define vcre_mpl i_vcre_mpl
#define vcre_mpf i_vcre_mpf
#define vdef_cyc i_vdef_cyc
#define vdef_alm i_vdef_alm
#define vcre_pis i_vcre_pis
#define vdel_pis i_vdel_pis
#define vvcre_pis i_vvcre_pis
#define vref_pis i_vref_pis
#define vwai_pis i_vwai_pis
#define vpreq_pis i_vpreq_pis
#define vtwai_pis i_vtwai_pis
#define vsig_pis i_vsig_pis
#define vref_tmb i_vref_tmb
#define vrcv_tmb i_vrcv_tmb
#define vprcv_tmb i_vprcv_tmb
#define vtrcv_tmb i_vtrcv_tmb
#define vsnd_tmb i_vsnd_tmb

extern ER unl_cpu(void);
extern ER loc_cpu(void);
extern ER def_svc(FN s_fncd, T_DSVC *pk_dsvc);
extern ER ref_sys(T_RSYS *pk_rsys);
extern ER ref_cfg(T_RCFG *pk_rcfg);
extern ER get_ver(T_VER *pk_ver);
extern ER cre_tsk(ID tskid, T_CTSK *pk_ctsk);
extern ER del_tsk(ID tskid);
extern ER ref_tsk(T_RTSK *pk_rtsk, ID tskid);
extern void ext_tsk(void);
extern void exd_tsk(void);
extern ER sta_tsk(ID tskid, INT stacd);
extern ER get_tid(ID *p_tskid);
extern ER ter_tsk(ID tskid);
extern ER chg_pri(ID tskid, PRI tskpri);
extern ER rot_rdq(PRI tskpri);
extern ER ena_dsp(void);
extern ER dis_dsp(void);
extern ER rel_wai(ID tskid);
extern ER sus_tsk(ID tskid);
extern ER rsm_tsk(ID tskid);
extern ER frsm_tsk(ID tskid);
extern ER tslp_tsk(TMO tmout);
extern ER slp_tsk(void);
extern ER wup_tsk(ID tskid);
extern ER can_wup(INT *p_wupcnt, ID tskid);
extern ER cre_flg(ID flgid, T_CFLG *pk_cflg);
extern ER del_flg(ID flgid);
extern ER ref_flg(T_RFLG *pk_rflg, ID flgid);
extern ER wai_flg(UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode);
extern ER clr_flg(ID flgid, UINT clrptn);
extern ER set_flg(ID flgid, UINT setptn);
extern ER cre_sem(ID semid, T_CSEM *pk_csem);
extern ER del_sem(ID semid);
extern ER ref_sem(T_RSEM *pk_rsem, ID semid);
extern ER wai_sem(ID semid);
extern ER sig_sem(ID semid);
extern ER cre_mbx(ID mbxid, T_CMBX *pk_cmbx);
extern ER del_mbx(ID mbxid);
extern ER ref_mbx(T_RMBX *pk_rmbx, ID mbxid);
extern ER rcv_msg(T_MSG* *ppk_msg, ID mbxid);
extern ER snd_msg(ID mbxid, T_MSG *pk_msg);
extern ER def_int(UINT dintno, T_DINT *pk_dint);
extern ER chg_iXX(UINT iXXXX);
extern ER ref_iXX(UINT *p_iXXXX);
extern ER ena_int(UINT eintno);
extern ER dis_int(UINT eintno);
extern ER cre_mpf(ID mpfid, T_CMPF *pk_cmpf);
extern ER del_mpf(ID mpfid);
extern ER ref_mpf(T_RMPF *pk_rmpf, ID mpfid);
extern ER get_blf(VP *p_blf, ID mpfid);
extern ER rel_blf(ID mpfid, VP blf);
extern ER vget_tim(SYSUTIME *pk_utim);
extern ER set_tim(SYSTIME *pk_tim);
extern ER get_tim(SYSTIME *pk_tim);
extern ER dly_tsk(DLYTIME dlytim);
extern ER def_alm(HNO almno, T_DALM *pk_dalm);
extern ER def_cyc(HNO cycno, T_DCYC *pk_dcyc);
extern ER ref_alm(T_RALM *pk_ralm, HNO almno);
extern ER ref_cyc(T_RCYC *pk_rcyc, HNO cycno);
extern ER act_cyc(HNO cycno, UINT cycact);
extern ER pget_blf(VP *p_blf, ID mpfid);
extern ER pget_blk(VP *p_blk, ID mplid, INT blksz);
extern ER pol_flg(UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode);
extern ER preq_sem(ID semid);
extern ER prcv_msg(T_MSG* *ppk_msg, ID mbxid);
extern ER psnd_mbf(ID mbfid, VP msg, INT msgsz);
extern ER prcv_mbf(VP msg, INT *p_msgsz, ID mbfid);
extern ER pcal_por(VP msg, INT *p_rmsgsz, ID porid, UINT calptn, INT cmsgsz);
extern ER pacp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn);
extern ER cre_mpl(ID mplid, T_CMPL *pk_cmpl);
extern ER del_mpl(ID mplid);
extern ER ref_mpl(T_RMPL *pk_rmpl, ID mplid);
extern ER get_blk(VP *p_blk, ID mplid, INT blksz);
extern ER rel_blk(ID mplid, VP blk);
extern ER cre_por(ID porid, T_CPOR *pk_cpor);
extern ER del_por(ID porid);
extern ER ref_por(T_RPOR *pk_rpor, ID porid);
extern ER cal_por(VP msg, INT *p_rmsgsz, ID porid, UINT calptn, INT cmsgsz);
extern ER acp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn);
extern ER rpl_rdv(RNO rdvno, VP msg, INT rmsgsz);
extern ER fwd_por(ID porid, UINT calptn, RNO rdvno, VP msg, INT cmsgsz);
extern ER cre_mbf(ID mbfid, T_CMBF *pk_cmbf);
extern ER del_mbf(ID mbfid);
extern ER ref_mbf(T_RMBF *pk_rmbf, ID mbfid);
extern ER snd_mbf(ID mbfid, VP msg, INT msgsz);
extern ER rcv_mbf(VP msg, INT *p_msgsz, ID mbfid);
extern ER tget_blf(VP *p_blf, ID mpfid, TMO tmout);
extern ER tget_blk(VP *p_blk, ID mplid, INT blksz, TMO tmout);
extern ER twai_flg(UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode, TMO tmout);
extern ER twai_sem(ID semid, TMO tmout);
extern ER trcv_msg(T_MSG* *ppk_msg, ID mbxid, TMO tmout);
extern ER tsnd_mbf(ID mbfid, VP msg, INT msgsz, TMO tmout);
extern ER trcv_mbf(VP msg, INT *p_msgsz, ID mbfid, TMO tmout);
extern ER tcal_por(VP msg, INT *p_rmsgsz, ID porid, UINT calptn, INT cmsgsz, TMO tmout);
extern ER tacp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn, TMO tmout);
extern ER vset_reg(ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs);
extern ER vget_reg(T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs, ID tskid);
extern ER vcre_tsk(T_CTSK *pk_ctsk);
extern ER vcre_sem(T_CSEM *pk_csem);
extern ER vcre_flg(T_CFLG *pk_cflg);
extern ER vcre_mbx(T_CMBX *pk_cmbx);
extern ER vcre_mbf(T_CMBF *pk_cmbf);
extern ER vcre_por(T_CPOR *pk_cpor);
extern ER vcre_mpl(T_CMPL *pk_cmpl);
extern ER vcre_mpf(T_CMPF *pk_cmpf);
extern ER vdef_cyc(T_DCYC *pk_dcyc);
extern ER vdef_alm(T_DALM *pk_dalm);
extern ER vcre_pis(ID pisid, T_CPIS *pk_cpis);
extern ER vdel_pis(ID pisid);
extern ER vvcre_pis(T_CPIS *pk_cpis);
extern ER vref_pis(T_RPIS *pk_rpis, ID pisid);
extern ER vwai_pis(ID pisid);
extern ER vpreq_pis(ID pisid);
extern ER vtwai_pis(ID pisid, TMO tmout);
extern ER vsig_pis(ID pisid);
extern ER vref_tmb(T_RTMB *pk_rtmb, ID tskid);
extern ER vrcv_tmb(T_MSG* *ppk_msg);
extern ER vprcv_tmb(T_MSG* *ppk_msg);
extern ER vtrcv_tmb(T_MSG* *ppk_msg, TMO tmout);
extern ER vsnd_tmb(ID tskid, T_MSG *pk_msg);

#else /* LINK_KERNEL */

Inline ER
sys_cal(FN fncd, INT par1, INT par2, INT par3, INT par4, INT par5, INT par6)
{
	SYS_CAL_6M(fncd, par1, par2, par3, par4, par5, par6);
}

Inline ER
unl_cpu(void)
{
	SYS_CAL_0(TFN_UNL_CPU);
}

Inline ER
loc_cpu(void)
{
	SYS_CAL_0(TFN_LOC_CPU);
}

Inline ER
def_svc(FN s_fncd, T_DSVC *pk_dsvc)
{
	SYS_CAL_2(TFN_DEF_SVC, s_fncd, pk_dsvc);
}

Inline ER
ref_sys(T_RSYS *pk_rsys)
{
	SYS_CAL_1M(TFN_REF_SYS, pk_rsys);
}

Inline ER
ref_cfg(T_RCFG *pk_rcfg)
{
	SYS_CAL_1M(TFN_REF_CFG, pk_rcfg);
}

Inline ER
get_ver(T_VER *pk_ver)
{
	SYS_CAL_1M(TFN_GET_VER, pk_ver);
}

Inline ER
cre_tsk(ID tskid, T_CTSK *pk_ctsk)
{
	SYS_CAL_2(TFN_CRE_TSK, tskid, pk_ctsk);
}

Inline ER
del_tsk(ID tskid)
{
	SYS_CAL_1(TFN_DEL_TSK, tskid);
}

Inline ER
ref_tsk(T_RTSK *pk_rtsk, ID tskid)
{
	SYS_CAL_2M(TFN_REF_TSK, pk_rtsk, tskid);
}

Inline void
ext_tsk(void)
{
	SYS_CAL_0V(TFN_EXT_TSK);
}

Inline void
exd_tsk(void)
{
	SYS_CAL_0V(TFN_EXD_TSK);
}

Inline ER
sta_tsk(ID tskid, INT stacd)
{
	SYS_CAL_2(TFN_STA_TSK, tskid, stacd);
}

Inline ER
get_tid(ID *p_tskid)
{
	SYS_CAL_1M(TFN_GET_TID, p_tskid);
}

Inline ER
ter_tsk(ID tskid)
{
	SYS_CAL_1(TFN_TER_TSK, tskid);
}

Inline ER
chg_pri(ID tskid, PRI tskpri)
{
	SYS_CAL_2(TFN_CHG_PRI, tskid, tskpri);
}

Inline ER
rot_rdq(PRI tskpri)
{
	SYS_CAL_1(TFN_ROT_RDQ, tskpri);
}

Inline ER
ena_dsp(void)
{
	SYS_CAL_0(TFN_ENA_DSP);
}

Inline ER
dis_dsp(void)
{
	SYS_CAL_0(TFN_DIS_DSP);
}

Inline ER
rel_wai(ID tskid)
{
	SYS_CAL_1(TFN_REL_WAI, tskid);
}

Inline ER
sus_tsk(ID tskid)
{
	SYS_CAL_1(TFN_SUS_TSK, tskid);
}

Inline ER
rsm_tsk(ID tskid)
{
	SYS_CAL_1(TFN_RSM_TSK, tskid);
}

Inline ER
frsm_tsk(ID tskid)
{
	SYS_CAL_1(TFN_FRSM_TSK, tskid);
}

Inline ER
tslp_tsk(TMO tmout)
{
	SYS_CAL_1(TFN_TSLP_TSK, tmout);
}

Inline ER
slp_tsk(void)
{
	SYS_CAL_0(TFN_SLP_TSK);
}

Inline ER
wup_tsk(ID tskid)
{
	SYS_CAL_1(TFN_WUP_TSK, tskid);
}

Inline ER
can_wup(INT *p_wupcnt, ID tskid)
{
	SYS_CAL_2M(TFN_CAN_WUP, p_wupcnt, tskid);
}

Inline ER
cre_flg(ID flgid, T_CFLG *pk_cflg)
{
	SYS_CAL_2(TFN_CRE_FLG, flgid, pk_cflg);
}

Inline ER
del_flg(ID flgid)
{
	SYS_CAL_1(TFN_DEL_FLG, flgid);
}

Inline ER
ref_flg(T_RFLG *pk_rflg, ID flgid)
{
	SYS_CAL_2M(TFN_REF_FLG, pk_rflg, flgid);
}

Inline ER
wai_flg(UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
	SYS_CAL_4M(TFN_WAI_FLG, p_flgptn, flgid, waiptn, wfmode);
}

Inline ER
clr_flg(ID flgid, UINT clrptn)
{
	SYS_CAL_2(TFN_CLR_FLG, flgid, clrptn);
}

Inline ER
set_flg(ID flgid, UINT setptn)
{
	SYS_CAL_2(TFN_SET_FLG, flgid, setptn);
}

Inline ER
cre_sem(ID semid, T_CSEM *pk_csem)
{
	SYS_CAL_2(TFN_CRE_SEM, semid, pk_csem);
}

Inline ER
del_sem(ID semid)
{
	SYS_CAL_1(TFN_DEL_SEM, semid);
}

Inline ER
ref_sem(T_RSEM *pk_rsem, ID semid)
{
	SYS_CAL_2M(TFN_REF_SEM, pk_rsem, semid);
}

Inline ER
wai_sem(ID semid)
{
	SYS_CAL_1(TFN_WAI_SEM, semid);
}

Inline ER
sig_sem(ID semid)
{
	SYS_CAL_1(TFN_SIG_SEM, semid);
}

Inline ER
cre_mbx(ID mbxid, T_CMBX *pk_cmbx)
{
	SYS_CAL_2(TFN_CRE_MBX, mbxid, pk_cmbx);
}

Inline ER
del_mbx(ID mbxid)
{
	SYS_CAL_1(TFN_DEL_MBX, mbxid);
}

Inline ER
ref_mbx(T_RMBX *pk_rmbx, ID mbxid)
{
	SYS_CAL_2M(TFN_REF_MBX, pk_rmbx, mbxid);
}

Inline ER
rcv_msg(T_MSG* *ppk_msg, ID mbxid)
{
	SYS_CAL_2M(TFN_RCV_MSG, ppk_msg, mbxid);
}

Inline ER
snd_msg(ID mbxid, T_MSG *pk_msg)
{
	SYS_CAL_2(TFN_SND_MSG, mbxid, pk_msg);
}

Inline ER
def_int(UINT dintno, T_DINT *pk_dint)
{
	SYS_CAL_2(TFN_DEF_INT, dintno, pk_dint);
}

Inline ER
chg_iXX(UINT iXXXX)
{
	SYS_CAL_1(TFN_CHG_IXX, iXXXX);
}

Inline ER
ref_iXX(UINT *p_iXXXX)
{
	SYS_CAL_1M(TFN_REF_IXX, p_iXXXX);
}

Inline ER
ena_int(UINT eintno)
{
	SYS_CAL_1(TFN_ENA_INT, eintno);
}

Inline ER
dis_int(UINT eintno)
{
	SYS_CAL_1(TFN_DIS_INT, eintno);
}

Inline ER
cre_mpf(ID mpfid, T_CMPF *pk_cmpf)
{
	SYS_CAL_2(TFN_CRE_MPF, mpfid, pk_cmpf);
}

Inline ER
del_mpf(ID mpfid)
{
	SYS_CAL_1(TFN_DEL_MPF, mpfid);
}

Inline ER
ref_mpf(T_RMPF *pk_rmpf, ID mpfid)
{
	SYS_CAL_2M(TFN_REF_MPF, pk_rmpf, mpfid);
}

Inline ER
get_blf(VP *p_blf, ID mpfid)
{
	SYS_CAL_2M(TFN_GET_BLF, p_blf, mpfid);
}

Inline ER
rel_blf(ID mpfid, VP blf)
{
	SYS_CAL_2(TFN_REL_BLF, mpfid, blf);
}

Inline ER
vget_tim(SYSUTIME *pk_utim)
{
	SYS_CAL_1M(TFN_VGET_TIM, pk_utim);
}

Inline ER
set_tim(SYSTIME *pk_tim)
{
	SYS_CAL_1(TFN_SET_TIM, pk_tim);
}

Inline ER
get_tim(SYSTIME *pk_tim)
{
	SYS_CAL_1M(TFN_GET_TIM, pk_tim);
}

Inline ER
dly_tsk(DLYTIME dlytim)
{
	SYS_CAL_1(TFN_DLY_TSK, dlytim);
}

Inline ER
def_alm(HNO almno, T_DALM *pk_dalm)
{
	SYS_CAL_2(TFN_DEF_ALM, almno, pk_dalm);
}

Inline ER
def_cyc(HNO cycno, T_DCYC *pk_dcyc)
{
	SYS_CAL_2(TFN_DEF_CYC, cycno, pk_dcyc);
}

Inline ER
ref_alm(T_RALM *pk_ralm, HNO almno)
{
	SYS_CAL_2M(TFN_REF_ALM, pk_ralm, almno);
}

Inline ER
ref_cyc(T_RCYC *pk_rcyc, HNO cycno)
{
	SYS_CAL_2M(TFN_REF_CYC, pk_rcyc, cycno);
}

Inline ER
act_cyc(HNO cycno, UINT cycact)
{
	SYS_CAL_2(TFN_ACT_CYC, cycno, cycact);
}

Inline ER
pget_blf(VP *p_blf, ID mpfid)
{
	SYS_CAL_2M(TFN_PGET_BLF, p_blf, mpfid);
}

Inline ER
pget_blk(VP *p_blk, ID mplid, INT blksz)
{
	SYS_CAL_3M(TFN_PGET_BLK, p_blk, mplid, blksz);
}

Inline ER
pol_flg(UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode)
{
	SYS_CAL_4M(TFN_POL_FLG, p_flgptn, flgid, waiptn, wfmode);
}

Inline ER
preq_sem(ID semid)
{
	SYS_CAL_1(TFN_PREQ_SEM, semid);
}

Inline ER
prcv_msg(T_MSG* *ppk_msg, ID mbxid)
{
	SYS_CAL_2M(TFN_PRCV_MSG, ppk_msg, mbxid);
}

Inline ER
psnd_mbf(ID mbfid, VP msg, INT msgsz)
{
	SYS_CAL_3(TFN_PSND_MBF, mbfid, msg, msgsz);
}

Inline ER
prcv_mbf(VP msg, INT *p_msgsz, ID mbfid)
{
	SYS_CAL_3M(TFN_PRCV_MBF, msg, p_msgsz, mbfid);
}

Inline ER
pcal_por(VP msg, INT *p_rmsgsz, ID porid, UINT calptn, INT cmsgsz)
{
	SYS_CAL_5M(TFN_PCAL_POR, msg, p_rmsgsz, porid, calptn, cmsgsz);
}

Inline ER
pacp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn)
{
	SYS_CAL_5M(TFN_PACP_POR, p_rdvno, msg, p_cmsgsz, porid, acpptn);
}

Inline ER
cre_mpl(ID mplid, T_CMPL *pk_cmpl)
{
	SYS_CAL_2(TFN_CRE_MPL, mplid, pk_cmpl);
}

Inline ER
del_mpl(ID mplid)
{
	SYS_CAL_1(TFN_DEL_MPL, mplid);
}

Inline ER
ref_mpl(T_RMPL *pk_rmpl, ID mplid)
{
	SYS_CAL_2M(TFN_REF_MPL, pk_rmpl, mplid);
}

Inline ER
get_blk(VP *p_blk, ID mplid, INT blksz)
{
	SYS_CAL_3M(TFN_GET_BLK, p_blk, mplid, blksz);
}

Inline ER
rel_blk(ID mplid, VP blk)
{
	SYS_CAL_2(TFN_REL_BLK, mplid, blk);
}

Inline ER
cre_por(ID porid, T_CPOR *pk_cpor)
{
	SYS_CAL_2(TFN_CRE_POR, porid, pk_cpor);
}

Inline ER
del_por(ID porid)
{
	SYS_CAL_1(TFN_DEL_POR, porid);
}

Inline ER
ref_por(T_RPOR *pk_rpor, ID porid)
{
	SYS_CAL_2M(TFN_REF_POR, pk_rpor, porid);
}

Inline ER
cal_por(VP msg, INT *p_rmsgsz, ID porid, UINT calptn, INT cmsgsz)
{
	SYS_CAL_5M(TFN_CAL_POR, msg, p_rmsgsz, porid, calptn, cmsgsz);
}

Inline ER
acp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn)
{
	SYS_CAL_5M(TFN_ACP_POR, p_rdvno, msg, p_cmsgsz, porid, acpptn);
}

Inline ER
rpl_rdv(RNO rdvno, VP msg, INT rmsgsz)
{
	SYS_CAL_3(TFN_RPL_RDV, rdvno, msg, rmsgsz);
}

Inline ER
fwd_por(ID porid, UINT calptn, RNO rdvno, VP msg, INT cmsgsz)
{
	SYS_CAL_5(TFN_FWD_POR, porid, calptn, rdvno, msg, cmsgsz);
}

Inline ER
cre_mbf(ID mbfid, T_CMBF *pk_cmbf)
{
	SYS_CAL_2(TFN_CRE_MBF, mbfid, pk_cmbf);
}

Inline ER
del_mbf(ID mbfid)
{
	SYS_CAL_1(TFN_DEL_MBF, mbfid);
}

Inline ER
ref_mbf(T_RMBF *pk_rmbf, ID mbfid)
{
	SYS_CAL_2M(TFN_REF_MBF, pk_rmbf, mbfid);
}

Inline ER
snd_mbf(ID mbfid, VP msg, INT msgsz)
{
	SYS_CAL_3(TFN_SND_MBF, mbfid, msg, msgsz);
}

Inline ER
rcv_mbf(VP msg, INT *p_msgsz, ID mbfid)
{
	SYS_CAL_3M(TFN_RCV_MBF, msg, p_msgsz, mbfid);
}

Inline ER
tget_blf(VP *p_blf, ID mpfid, TMO tmout)
{
	SYS_CAL_3M(TFN_TGET_BLF, p_blf, mpfid, tmout);
}

Inline ER
tget_blk(VP *p_blk, ID mplid, INT blksz, TMO tmout)
{
	SYS_CAL_4M(TFN_TGET_BLK, p_blk, mplid, blksz, tmout);
}

Inline ER
twai_flg(UINT *p_flgptn, ID flgid, UINT waiptn, UINT wfmode, TMO tmout)
{
	SYS_CAL_5M(TFN_TWAI_FLG, p_flgptn, flgid, waiptn, wfmode, tmout);
}

Inline ER
twai_sem(ID semid, TMO tmout)
{
	SYS_CAL_2(TFN_TWAI_SEM, semid, tmout);
}

Inline ER
trcv_msg(T_MSG* *ppk_msg, ID mbxid, TMO tmout)
{
	SYS_CAL_3M(TFN_TRCV_MSG, ppk_msg, mbxid, tmout);
}

Inline ER
tsnd_mbf(ID mbfid, VP msg, INT msgsz, TMO tmout)
{
	SYS_CAL_4(TFN_TSND_MBF, mbfid, msg, msgsz, tmout);
}

Inline ER
trcv_mbf(VP msg, INT *p_msgsz, ID mbfid, TMO tmout)
{
	SYS_CAL_4M(TFN_TRCV_MBF, msg, p_msgsz, mbfid, tmout);
}

Inline ER
tcal_por(VP msg, INT *p_rmsgsz, ID porid, UINT calptn, INT cmsgsz, TMO tmout)
{
	SYS_CAL_6M(TFN_TCAL_POR, msg, p_rmsgsz, porid, calptn, cmsgsz, tmout);
}

Inline ER
tacp_por(RNO *p_rdvno, VP msg, INT *p_cmsgsz, ID porid, UINT acpptn, TMO tmout)
{
	SYS_CAL_6M(TFN_TACP_POR, p_rdvno, msg, p_cmsgsz, porid, acpptn, tmout);
}

Inline ER
vset_reg(ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs)
{
	SYS_CAL_4(TFN_VSET_REG, tskid, pk_regs, pk_eit, pk_cregs);
}

Inline ER
vget_reg(T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs, ID tskid)
{
	SYS_CAL_4M(TFN_VGET_REG, pk_regs, pk_eit, pk_cregs, tskid);
}

Inline ER
vcre_tsk(T_CTSK *pk_ctsk)
{
	SYS_CAL_1(TFN_VCRE_TSK, pk_ctsk);
}

Inline ER
vcre_sem(T_CSEM *pk_csem)
{
	SYS_CAL_1(TFN_VCRE_SEM, pk_csem);
}

Inline ER
vcre_flg(T_CFLG *pk_cflg)
{
	SYS_CAL_1(TFN_VCRE_FLG, pk_cflg);
}

Inline ER
vcre_mbx(T_CMBX *pk_cmbx)
{
	SYS_CAL_1(TFN_VCRE_MBX, pk_cmbx);
}

Inline ER
vcre_mbf(T_CMBF *pk_cmbf)
{
	SYS_CAL_1(TFN_VCRE_MBF, pk_cmbf);
}

Inline ER
vcre_por(T_CPOR *pk_cpor)
{
	SYS_CAL_1(TFN_VCRE_POR, pk_cpor);
}

Inline ER
vcre_mpl(T_CMPL *pk_cmpl)
{
	SYS_CAL_1(TFN_VCRE_MPL, pk_cmpl);
}

Inline ER
vcre_mpf(T_CMPF *pk_cmpf)
{
	SYS_CAL_1(TFN_VCRE_MPF, pk_cmpf);
}

Inline ER
vdef_cyc(T_DCYC *pk_dcyc)
{
	SYS_CAL_1(TFN_VDEF_CYC, pk_dcyc);
}

Inline ER
vdef_alm(T_DALM *pk_dalm)
{
	SYS_CAL_1(TFN_VDEF_ALM, pk_dalm);
}

Inline ER
vcre_pis(ID pisid, T_CPIS *pk_cpis)
{
	SYS_CAL_2(TFN_VCRE_PIS, pisid, pk_cpis);
}

Inline ER
vdel_pis(ID pisid)
{
	SYS_CAL_1(TFN_VDEL_PIS, pisid);
}

Inline ER
vvcre_pis(T_CPIS *pk_cpis)
{
	SYS_CAL_1(TFN_VVCRE_PIS, pk_cpis);
}

Inline ER
vref_pis(T_RPIS *pk_rpis, ID pisid)
{
	SYS_CAL_2M(TFN_VREF_PIS, pk_rpis, pisid);
}

Inline ER
vwai_pis(ID pisid)
{
	SYS_CAL_1(TFN_VWAI_PIS, pisid);
}

Inline ER
vpreq_pis(ID pisid)
{
	SYS_CAL_1(TFN_VPREQ_PIS, pisid);
}

Inline ER
vtwai_pis(ID pisid, TMO tmout)
{
	SYS_CAL_2(TFN_VTWAI_PIS, pisid, tmout);
}

Inline ER
vsig_pis(ID pisid)
{
	SYS_CAL_1(TFN_VSIG_PIS, pisid);
}

Inline ER
vref_tmb(T_RTMB *pk_rtmb, ID tskid)
{
	SYS_CAL_2M(TFN_VREF_TMB, pk_rtmb, tskid);
}

Inline ER
vrcv_tmb(T_MSG* *ppk_msg)
{
	SYS_CAL_1M(TFN_VRCV_TMB, ppk_msg);
}

Inline ER
vprcv_tmb(T_MSG* *ppk_msg)
{
	SYS_CAL_1M(TFN_VPRCV_TMB, ppk_msg);
}

Inline ER
vtrcv_tmb(T_MSG* *ppk_msg, TMO tmout)
{
	SYS_CAL_2M(TFN_VTRCV_TMB, ppk_msg, tmout);
}

Inline ER
vsnd_tmb(ID tskid, T_MSG *pk_msg)
{
	SYS_CAL_2(TFN_VSND_TMB, tskid, pk_msg);
}

#endif /* LINK_KERNEL */
#endif /* _ITIS_ISYSCALL_ */
