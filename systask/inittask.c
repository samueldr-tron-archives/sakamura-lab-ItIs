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
 *  @(#) $Id: inittask.c,v 1.11 1998/01/30 07:14:05 hiro Exp $
 */

/*
 *  �������^�X�N
 */

#include "systask.h"

/*
 *  �K�v�ȃV�X�e���^�X�N����я����N���^�X�N���N������D��̓I�ɂ́C�g
 *  �ݍ��ނׂ��V�X�e���^�X�N����уh���C�o�ƁC�����N���^�X�N���N������D
 *  �ȏ�̏������I����ƁCslp_tsk �ɂ��҂���Ԃɓ���D�V�X�e���S�̂�
 *  ���S�ɒ�~������ꍇ�ɂ́C�������^�X�N�� wup_tsk �ŋN��������D
 */
void
init_task(INT stacd)
{
	syslog(LOG_NOTICE, "init: I'm now starting up some tasks.....");

	/*
	 *  BSD UNIX�p �m���u���b�L���OI/O �T�|�[�g���W���[���̋N��
	 *
	 *  �V���A���C���^�t�F�[�X����ɋN������ׂ��D
	 */
#ifdef FLG_BSD_SIGIO
	bsd_sigio_startup();
#endif

	/*
	 *  �V���A���C���^�t�F�[�X�h���C�o�̋N��
	 */
#ifdef CONSOLE_PORT
	serial_startup(CONSOLE_PORT);
#endif

	/*
	 *  �V�X�e�����O�^�X�N�̋N��
	 */
#ifdef LOGTASK_PORT
	logtask_startup(LOGTASK_PORT);
#endif

	/*
	 *  �����N���^�X�N�̋N��
	 */
#ifdef TSK_FIRST
	syscall(cre_tsk(TSK_FIRST, &TSK_FIRST_CTSK));
	syscall(sta_tsk(TSK_FIRST, stacd));
#endif

	/*
	 *  ��������������
	 */
	syslog(LOG_NOTICE, "init: initialize OK.");
	syscall(slp_tsk());

	/*
	 *  �V�X�e����~����
	 */
#ifdef CONSOLE_PORT
	serial_shutdown(CONSOLE_PORT, 1);
#endif
	itis_exit();
}
