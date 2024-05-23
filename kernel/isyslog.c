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
 *  @(#) $Id: isyslog.c,v 1.7 1996/02/17 09:33:05 hiro Exp $
 */

#include <stdarg.h>
#include "itis_kernel.h"
#include "task.h"

/*
 *  ���O�^�X�N�̕ϐ�
 *
 *  �^�X�N�̕ϐ����J�[�l�����Q�Ƃ���Ƃ�����O�I�ȏ����ɂȂ��Ă���D
 */
extern int	logtask_alive;		/* ���O�^�X�N�������Ă��邩 */
extern int	log_msg_maxmsz;		/* ���O���b�Z�[�W�̍ő咷 */

/*
 *  �V�X�e�����O�p���b�Z�[�W�o�b�t�@�ւ̑��M (messagebuf.c)
 */
extern ER	log_snd_mbf(VP msg, INT msgsz);

/*
 *  �J�[�l���p�Ȉ� vsprintf�֐� (vsprintf.c)
 */
extern int	itis_vsprintf(char *buf, const char *format, va_list ap);

/*
 *  �J�[�l���p�V�X�e�����O�o�͗p���C�u����
 */

static int	i_logmask = LOG_UPTO(LOG_NOTICE);

#define	FORMAT_BUFSIZ	1024

static char	i_syslog_buf[FORMAT_BUFSIZ];

/*
 *  ���O���b�Z�[�W�̏o��
 *
 *  ���O�^�X�N�������Ă���ꍇ�́C���O���b�Z�[�W�o�b�t�@�֑���D������
 *  ���Ȃ��ꍇ�́C���ڒ჌�x���̏o�̓��[�`�����g���ďo�͂���D
 */
static void
i_syslog_send(const char *string, int len)
{
	if (len > 0) {
#ifdef USE_TMBF_OS
		if (logtask_alive) {
			if (len > log_msg_maxmsz) {
				len = log_msg_maxmsz;
			}
			if (log_snd_mbf((VP) string, len) != E_OK) {
				logtask_alive = 0;
			}
		}
		if (!logtask_alive) {
			sys_write(string, len);
			sys_write("\n", 1);
		}
#else /* USE_TMBF_OS */
		sys_write(string, len);
		sys_write("\n", 1);
#endif /* USE_TMBF_OS */
	}
}

/*
 *  �J�[�l���p syslog �֐��{��
 */
void
i_syslog(int priority, const char *format, ...)
{
	va_list	ap;
	int	len;

	if (i_logmask & LOG_MASK(priority & LOG_PRIMASK)) {
		va_start(ap, format);
		BEGIN_CRITICAL_SECTION;
		if ((priority & LOG_PRIMASK) == LOG_EMERG) {
			logtask_alive = 0;
		}
		len = itis_vsprintf(i_syslog_buf, format, ap);
		i_syslog_send(i_syslog_buf, len);
		END_CRITICAL_SECTION;
		va_end(ap);
	}
}

/*
 *  �J�[�l���p�� assert�}�N���̃��b�Z�[�W�o��
 */

#ifndef NDEBUG

void
i_assert_fail(const char *expr, const char *file, unsigned int line)
{
	i_syslog(LOG_KERN|LOG_EMERG, "%s:%u: Assertion `%s' failed.",
		file, line, expr);
	itis_exit();
}

#endif /* NDEBUG */
