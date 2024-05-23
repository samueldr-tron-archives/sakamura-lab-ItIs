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
 *  @(#) $Id: sys_conf.h,v 1.4 1996/02/17 09:43:40 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  �X�^�b�N�G���A�C�q�[�v�G���A�̒�`
 *
 *  _end �́C�v���O�����Ŏg�p����f�[�^�G���A�̍Ō�̎��̔Ԓn�D
 */
#define STACKTOP	0x100000	/* �^�X�N�Ɨ����p�X�^�b�N�̏����l */
#define HEAPTOP		_end		/* �q�[�v�G���A�̐擪 */
#define HEAPLIMIT	0x0fe000	/* �q�[�v�G���A�̏�� */

/*
 *  �V�X�e���^�X�N�Ɋւ����`
 */
#define	CONSOLE_PORT	2	/* �R���\�[���p�ɗp����V���A���|�[�g�ԍ� */
#define	LOGTASK_PORT	2	/* �V�X�e�����O���o�͂���V���A���|�[�g�ԍ� */

#endif /* _SYS_CONF_ */
