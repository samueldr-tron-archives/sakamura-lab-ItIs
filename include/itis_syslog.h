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
 *  @(#) $Id: itis_syslog.h,v 1.4 1996/02/17 09:26:47 hiro Exp $
 */

#ifndef	_ITIS_SYSLOG_
#define	_ITIS_SYSLOG_

/* 
 *  �V�X�e�����O�o�̓��C�u�����̒�`
 * 
 *  �V�X�e�����O�փ��b�Z�[�W���o�͂��邽�߂̊֐��̒�`�DUNIX �̃C���^�t�F�[
 *  �X�Ƌ߂��C���^�t�F�[�X�ɂ��Ă��邪�C���̂Ƃ��� facility �͎g���Ă��Ȃ��D
 *
 *  �J�[�l���p�� setlogmask �͗p�ӂ��Ă��Ȃ� (���̂͊ȒP)�D
 */

extern void	syslog(int priority, const char *format, ...);
extern int	setlogmask(int maskpri);

/*
 *  facility �̒�`
 */
#define LOG_KERN	(0 << 3)	/* �J�[�l�� */
#define LOG_NKERN	(1 << 3)	/* �O�j */
#define LOG_INDP	(2 << 3)	/* �^�X�N�Ɨ��� */
#define LOG_EXTSVC	(3 << 3)	/* �g��SVC�n���h�� */
#define LOG_STASK	(4 << 3)	/* �V�X�e���^�X�N */
#define LOG_UTASK	(5 << 3)	/* ���[�U�^�X�N */

#define LOG_NFACILITIES	16		/* facility �̐� */
#define LOG_FACMASK	0x0078		/* facility �����o�����̃}�X�N�l */

/*
 *  priority �̒�`
 */
#define LOG_EMERG	0		/* �V�X�e���_�E���ɒl����G���[ */
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3		/* �V�X�e���G���[ */
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7		/* �f�o�b�O�p���b�Z�[�W */

#define LOG_PRIMASK	0x0007		/* priority �����o���}�X�N�l */

/*
 *  setlogmask �̈�������邽�߂̃}�N��
 */
#define LOG_MASK(pri)	(1 << (pri))
#define LOG_UPTO(pri)	((1 << ((pri) + 1)) - 1)

#endif /* _ITIS_SYSLOG_ */
