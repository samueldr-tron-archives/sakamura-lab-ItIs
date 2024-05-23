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
 *  @(#) $Id: tron_insn.h,v 1.4 1997/01/10 12:34:59 hiro Exp $
 */

#ifndef	_TRON_INSN_
#define	_TRON_INSN_

/*
 *  制御レジスタの操作関数
 */

/*
 *  PSW の現在値の読み出し
 */
Inline VW
current_psw(void)
{
	VW	psw;

	Asm("stc %1, %0" : "=g"(psw) : "m"(CREG(PSW)));
	return(psw);
}

/*
 *  PSW の現在値の変更
 */
Inline void
set_psw(VW psw)
{
	Asm("ldc %0, %1" : : "g"(psw), "m"(CREG(PSW)));
}

/*
 *  IMASK の現在値の読み出し
 *
 *  IMASK の値として，0 以上 15 以下の値が返る．
 */
#define imask_psw(psw)	((psw >> 16) & 15)
#define current_imask() (imask_psw(current_psw()))

/*
 *  IMASK の現在値の変更
 *
 *  imask は，0 以上 15 以下であること．
 */
Inline void
set_imask(INT imask)
{
	Asm("ldc %0, %1" : : "g"(imask << 16), "m"(CREG(IMASK)));
}

/*
 *  割込みマスク状態を内部表現で返す．
 */
Inline INT
current_intmask(void)
{
	INT	intmask;

	Asm("stc %1, %0" : "=g"(intmask) : "m"(CREG(IMASK)));
	return(intmask);
}

/*
 *  NMI を除くすべての割込みを禁止する．
 */
Inline void
disint(void)
{
	Asm("ldc #0, %0" : : "m"(CREG(IMASK)));
}

/*
 *  current_intmask が返した割込みマスク状態を渡して，割込み禁止前の状
 *  態に戻す．
 *
 *  clobberリストに memory を入れているのは，enaint の直後にタスクディ
 *  スパッチが起こり，予期しないメモリエリアが書き換えられる可能性があ
 *  ることをコンパイラに知らせるためである．具体的には，エラーコードを
 *  入れる変数が問題になる．
 */
Inline void
enaint(INT intmask)
{
	Asm("ldc %0, %1" : : "g"(intmask), "m"(CREG(IMASK)) : "memory");
}

/*
 *  DIR (遅延割込み要求レジスタ) の現在値の読み出し
 */
Inline VW
current_dir(void)
{
	VW	dir;

	Asm("stc %1, %0" : "=g"(dir) : "m"(CREG(DIR)));
	return(dir);
}

/*
 *  DIR (遅延割込み要求レジスタ) の現在値の変更
 *
 *  dir は，0 以上 15 以下であること．
 */
Inline void
set_dir(INT dir)
{
	Asm("ldc %0, %1" : : "g"(dir), "m"(CREG(DIR)));
}

/*
 *  EIT関連の定義
 */

#define EITVEC_BUSERR	0x11		/* バスエラーのベクタ番号 */
#define EITVEC_TRAPA1	0x21		/* TRAPA #1 のベクタ番号 */
#define EITVEC_TRAPA2	0x22		/* TRAPA #2 のベクタ番号 */
#define EITVEC_TRAPA3	0x23		/* TRAPA #3 のベクタ番号 */
#define EITVEC_TRAPA4	0x24		/* TRAPA #4 のベクタ番号 */
#define EITVEC_DI14	0x5e		/* DI=14 のベクタ番号 */

#define EITATR(smode, imask)	(((smode) << 31) + ((imask) << 16))

/*
 *  EITベクタテーブルの構造の定義
 */
typedef struct eit_vector_entry {
	UINT	eitatr;			/* EIT属性 */
	FP	eithdr;			/* EITハンドラのアドレス */
} EITVE;

/*
 *  EITベクタテーブルベース (EITVB) の設定
 *
 *  初期化処理の中で使うための関数．
 */
Inline void
set_eitvb(EITVE *eitvb)
{
	Asm("ldc %0, %1" : : "r"(eitvb), "m"(CREG(EITVB)));
}

/*
 *  EITベクタテーブルベース (EITVB) の現在値の読み出し
 *
 *  EITVB_ITIS が定義されている時は，初期化処理の中で EITVBレジスタを
 *  EITVB_ITIS に設定するので，EITVB_ITIS を返せばよい．そうでない場合
 *  は，CPU の EITVBレジスタを読み出す．
 */
#ifdef EITVB_ITIS
#define current_eitvb()	((EITVE *) EITVB_ITIS)
#else /* EITVB_ITIS */

Inline EITVE
*current_eitvb(void)
{
	EITVE	*eitvb;

	Asm("stc %1, %0" : "=r"(eitvb) : "m"(CREG(EITVB)));
	return(eitvb);
}

#endif /* EITVB_ITIS */

/*
 *  EITハンドラの設定
 *
 *  ベクトル番号 eitvec の EITベクタテーブルエントリの，EIT属性 (EITハ
 *  ンドラ実行時の，スタックモード，アドレス変換モード，デバッグモード，
 *  割込みマスクを決める) を eitatr，EITハンドラ開始番地を eithdr に設
 *  定する．
 */
Inline void
define_eit(INT eitvec, UINT eitatr, FP eithdr)
{
	EITVE	*eitvb = current_eitvb();

	eitvb[eitvec].eitatr = eitatr;
	eitvb[eitvec].eithdr = eithdr;
}

/*
 *  メモリブロック操作ライブラリ
 *
 *  関数の仕様は，ANSI C ライブラリの仕様と同じ．標準ライブラリのものを
 *  使った方が効率が良い可能性がある．
 */

Inline VP
memcpy(VP dest, VP src, UINT len)
{
#ifdef tron
	if (len > 0) {
		register VP	src_r0 asm("r0") = src;
		register VP	dest_r1 asm("r1") = dest;
		register UINT	len_r2 asm("r2") = len;

		asm("smov/f.b"
		  : /* no output */
		  : "g"(src_r0), "g"(dest_r1), "g"(len_r2)
		  : "r0", "r1", "r2", "memory");
	}
#else /* tron */
	VB	*d = dest;
	VB	*s = src;

	while (len-- > 0) {
		*d++ = *s++;
	}
#endif /* tron */
	return(dest);
}

Inline VP
memset(VP str, INT ch, UINT len)
{
#ifdef tron
	if (len > 0) {
		register VP	str_r1 asm("r1") = str;
		register UINT	len_r2 asm("r2") = len;
		register INT	ch_r3 asm("r3") = ch;

		asm("sstr.b"
		  : /* no output */
		  : "g"(str_r1), "g"(len_r2), "g"(ch_r3)
		  : "r1", "r2", "memory");
	}
#else /* tron */
	VB	*s = str;

	while (len-- > 0) {
		*s++ = ch;
	}
#endif /* tron */
	return(str);
}

#endif /* _TRON_INSN_ */
