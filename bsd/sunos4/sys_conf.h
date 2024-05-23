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
 *  @(#) $Id: sys_conf.h,v 1.5 1996/02/17 09:24:12 hiro Exp $
 */

#ifndef _SYS_CONF_
#define _SYS_CONF_

/*
 *  OS �̎�ނ̒�`
 */
#define	SUNOS4

/*
 *  CPU �� _setjmp/_longjmp ���C�u�����̎����Ɉˑ�����萔�̐ݒ�
 *
 *  JMPBUF_PC: jmp_buf �\���̒��ŁCPC �̒l������ꏊ�̃I�t�Z�b�g
 *  JMPBUF_SP: jmp_buf �\���̒��ŁCSP �̒l������ꏊ�̃I�t�Z�b�g
 *
 *  LONGJMP_MERGIN: longjmp ������ɁC�X�^�b�N�|�C���^����̔Ԓn�� 
 *  �A�N�Z�X����\��������ꍇ�́C�}�[�W���̈�̃T�C�Y�D
 *
 *  SIGSTACK_MERGIN: �V�O�i���X�^�b�N���v���Z�X�X�^�b�N��Ɏ�鎞�̃}�[
 *  �W���D�X�^�[�g�A�b�v���[�`�����g�����߂̃X�^�b�N�̈�D
 */

#ifdef sparc

#define JMPBUF_PC		3
#define JMPBUF_SP		2
#define LONGJMP_MERGIN		256

#define SIGSTACK_MERGIN		1024

#else /* sparc */

#error not supported.

#endif /* sparc */

/*
 *  �V�O�i���X�^�b�N�̕ύX�� sigstack ���g��
 */
#define USE_SIGSTACK

/*
 *  �X�^�b�N�G���A�̎擾/�ԋp�֐��� mprotect ���g�����̂ɍ���������
 */
#define	USE_MPROTECT_STACK

/*
 *  �^���V���A���h���C�o�̂��߂̒�`
 */

#ifdef _BSD_SERIAL_

#include <sys/ioctl.h>

Inline void
set_rawmode(struct sgttyb *ttyb)
{
	ttyb->sg_flags |= (RAW | CBREAK);
	ttyb->sg_flags &= ~(ECHO);
}

#undef RAW			/* �Փ˂���V���{���̒�`�𖕏� */
#undef ECHO

#endif /* _BSD_SERIAL_ */
#endif /* _SYS_CONF_ */
