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
 *  @(#) $Id: tron_defs.h,v 1.4 1997/01/10 12:34:58 hiro Exp $
 */

#ifndef _TRON_DEFS_
#define _TRON_DEFS_

/*
 *  TRON仕様チップの制御レジスタの番地の定義
 */
#define PSW		0x0004		/* Gmicro/200, Gmicro/300, TX-2 */
#define SMRNG		0x000c		/* Gmicro/200, Gmicro/300, TX-2 */
#define IMASK		0x0014		/* Gmicro/200, Gmicro/300, TX-2 */
#define EITVB		0x0024		/* Gmicro/200, Gmicro/300, TX-2 */
#define JRNGVB		0x002c		/* Gmicro/200, Gmicro/300, TX-2 */
#define CTXBB		0x0034		/* Gmicro/200, Gmicro/300, TX-2 */
#define SATB		0x0044		/* Gmicro/200, Gmicro/300 */
#define UATB		0x004c		/* Gmicro/200, Gmicro/300 */
#define LSID		0x0054		/* Gmicro/300 */
#define IOADDR		0x0064
#define IOMASK		0x006c
#define MJRNGV		0x0074		/* TX-2 */
#define CSW		0x0084		/* Gmicro/200, Gmicro/300, TX-2 */
#define DIR		0x008c		/* Gmicro/200, Gmicro/300, TX-2 */
#define DCT		0x0094		/* TX-2 */
#define CTXBFM		0x009c		/* TX-2 */
#define SPI		0x0104		/* Gmicro/200, Gmicro/300, TX-2 */
#define SP0		0x0124		/* Gmicro/200, Gmicro/300, TX-2 */
#define SP1		0x012c		/* Gmicro/200, Gmicro/300, TX-2 */
#define SP2		0x0134		/* Gmicro/200, Gmicro/300, TX-2 */
#define SP3		0x013c		/* Gmicro/200, Gmicro/300, TX-2 */

/*
 *  Gmicro/200
 */
#define G200_DCW	0x0404
#define G200_DCCW	0x040c
#define G200_IBA0	0x0484
#define G200_IBA1	0x048c
#define G200_OBA	0x0504

/*
 *  Gmicro/300
 */
#define G300_DBIW	0x0444
#define G300_RBCW	0x044c
#define G300_IBA0	0x0484
#define G300_IBA1	0x048c
#define G300_OBA0	0x0504
#define G300_OBA1	0x050c

/*
 *  TX-2
 */
#define TX2_DBIW	0x0444
#define TX2_IBA0	0x0484
#define TX2_IBA1	0x048c
#define TX2_OBA0	0x0504
#define TX2_OBA1	0x050c

/*
 *  制御レジスタアクセス用マクロ
 */
#define CREG(ADDR)	(*((VW *)(ADDR)))

/*
 *  ディスパッチャ関連の定数の定義
 */
#ifndef TRON_NO_DI
#define DI14_EITINF	0x0003005e	/* DI=14 に対する EITINF */
#else /* TRON_NO_DI */
#define TRAPA4_EITINF	0x02010024	/* TRAPA #4 に対する EITINF */
#define DISPATCHER_PSW	0x800f6000	/* ディスパッチャ分岐後の PSW */
#endif /* TRON_NO_DI */

#endif /* _TRON_DEFS_ */
