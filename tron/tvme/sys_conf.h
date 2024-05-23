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
 *  @(#) $Id: sys_conf.h,v 1.5 1996/07/28 00:29:14 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  ItIs ���쎞�̃������}�b�v
 *
 *	00000000 -		�J�[�l���R�[�h�̈�
 *		 - 00001000	ROM���j�^�̏���SPI (���o�C�g�󂳂��)
 *	00040000 - 0007ffff	���[�U�v���O�����̈� (256KB)
 *	00080000 - 000fffff	�J�[�l���f�[�^�̈� (512KB)
 *	fff20000 - fff27fff	���j�^���[�N�̈� (32KB)
 *	fff28000 - fff37fff	���g�p (32KB, �ꕔ�_�C���N�g�N���p�̃X�^�b�N)
 *		 - fff3ffff	�^�X�N�Ɨ����p�X�^�b�N�̈�
 *	fffb0000 - fffbffff	���[�J��I/O�̈�
 *	fffc0000 - fffc007f	���C���{�b�N�X�̈�
 */

/*
 *  �X�^�b�N�G���A�C�q�[�v�G���A�̒�`
 *
 *  _end �́C�v���O�����Ŏg�p����f�[�^�G���A�̍Ō�̎��̔Ԓn�D
 */
#define STACKTOP	0xfff40000	/* �^�X�N�Ɨ����p�X�^�b�N�̏����l */
#define HEAPTOP		_end		/* �q�[�v�G���A�̐擪 */
#define HEAPLIMIT	0x00100000	/* �q�[�v�G���A�̏�� */

/*
 *  �V�X�e���^�X�N�Ɋւ����`
 */
#define	CONSOLE_PORT	1	/* �R���\�[���p�ɗp����V���A���|�[�g�ԍ� */
#define	LOGTASK_PORT	1	/* �V�X�e�����O���o�͂���V���A���|�[�g�ԍ� */

#endif /* _SYS_CONF_ */
