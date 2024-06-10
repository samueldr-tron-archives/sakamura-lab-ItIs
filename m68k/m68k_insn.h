/**
 * 
 * 	ItIs - An ITRON Implementation for Research and Education
 * 
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
 * 3. Neither the name of the university nor the name of the laboratory
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *  @(#) $Id: m68k_insn.h,v 1.2 1998/06/18 07:45:23 hiro Exp $
 */

#ifndef	_M68K_INSN_
#define	_M68K_INSN_

/*
 *  制御レジスタの操作関数
 */

/*
 *  SR の現在値の読み出し
 */
Inline VH
current_sr(void)
{
	VH	sr;

	Asm("move.w sr, %0" : "=g"(sr));
	return(sr);
}

/*
 *  SR の現在値の変更
 *
 *  clobberリストに memory を入れているのは，set_sr の直後にタスクディ
 *  スパッチが起こり，予期しないメモリエリアが書き換えられる可能性があ
 *  ることをコンパイラに知らせるためである．具体的には，エラーコードを
 *  入れる変数が問題になる．
 */
Inline void
set_sr(VH sr)
{
	Asm("move.w %0, sr" : : "g"(sr) : "memory");
}

/*
 *  割込みマスク状態を内部表現で返す．
 */
Inline INT
current_intmask(void)
{
	return(current_sr() & 0x0700);
}

/*
 *  NMI を除くすべての割込みを禁止する．
 */
Inline void
disint(void)
{
	Asm("ori.w #0x0700, sr");
}

/*
 *  current_intmask が返した割込みマスク状態を渡して，割込み禁止前の状
 *  態に戻す．
 */
Inline void
enaint(INT intmask)
{
	set_sr((current_sr() & ~0x0700) | intmask);
}

/*
 *  intmask から，CPUロック中かどうかを判別する．ロック中である場合に
 *  真となる．
 */
#define intmask_lock(intmask)	((intmask) > 0)

/*
 *  IPM の現在値の読み出し
 *
 *  IPM の値として，0 以上 7 以下の値が返る．
 */
Inline INT
current_ipm(void)
{
	return(current_intmask() >> 16);
}

/*
 *  IPM の現在値の変更
 *
 *  ipm は，0 以上 7 以下であること．
 */
Inline void
set_ipm(INT ipm)
{
	set_sr((current_sr() & ~0x0700) | (ipm << 16));
}

/*
 *  例外関連の定義
 */

#define EXCVEC_AFAULT	0x02		/* Access Fault のベクタ番号 */

#define EXCVEC_TRAP1	0x21		/* TRAP #1 のベクタ番号 */
#define EXCVEC_TRAP2	0x22		/* TRAP #2 のベクタ番号 */
#define EXCVEC_TRAP6	0x26		/* TRAP #6 のベクタ番号 */
#define EXCVEC_TRAP7	0x27		/* TRAP #7 のベクタ番号 */

/*
 *  例外ベクタテーブルの構造の定義
 */
typedef struct exc_vector_entry {
	FP	exchdr;			/* 例外ハンドラのアドレス */
} EXCVE;

/*
 *  ベクタベースレジスタ (VBR) の設定
 *
 *  初期化処理の中で使うための関数．
 */
Inline void
set_vbr(EXCVE *vbr)
{
	Asm("movec.l %0, vbr" : : "r"(vbr));
}

/*
 *  ベクタベースレジスタ (VBR) の現在値の読み出し
 *
 *  EXCVT_ITIS が定義されている時は，初期化処理の中で VBR を EXCVT_ITIS
 *  に設定するので，EXCVT_ITIS を返せばよい．そうでない場合は，CPU の 
 *  VBR レジスタを読み出す．
 */
#ifdef EXCVT_ITIS
#define current_vbr()	((EXCVE *) EXCVT_ITIS)
#else /* EXCVT_ITIS */

Inline EXCVE
*current_vbr(void)
{
	EXCVE	*vbr;

	Asm("movec.l vbr, %0" : "=r"(vbr));
	return(vbr);
}

#endif /* EXCVT_ITIS */

/*
 *  例外ハンドラの設定
 *
 *  ベクトル番号 excvec の例外ハンドラ開始番地を exchdr に設定する．
 */
Inline void
define_exc(INT excvec, FP exchdr)
{
	EXCVE	*excvt = current_vbr();

	excvt[excvec].exchdr = exchdr;
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
	VB	*d = dest;
	VB	*s = src;

	while (len-- > 0) {
		*d++ = *s++;
	}
	return(dest);
}

Inline VP
memset(VP str, INT ch, UINT len)
{
	VB	*s = str;

	while (len-- > 0) {
		*s++ = ch;
	}
	return(str);
}

#endif /* _M68K_INSN_ */
