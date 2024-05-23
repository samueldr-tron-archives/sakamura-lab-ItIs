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
 *  @(#) $Id: ready_queue.h,v 1.9 1997/01/10 11:50:14 hiro Exp $
 */

#ifndef _READY_QUEUE_
#define _READY_QUEUE_

/*
 *  ���f�B�L���[���샋�[�`��
 */

#include "queue.h"

/*
 *  ���f�B�L���[�̍\���̒�`
 *
 *  ���f�B�L���[���ɂ́C�D��x���Ƃ̃^�X�N�L���[ tskque ���p�ӂ���Ă�
 *  ��C�^�X�N�� TCB �́C�Y������D��x�̃L���[�ɓo�^�����D
 *  ���f�B�L���[�̃T�[�`�������悭�s�����߂ɁC�D��x���Ƃ̃^�X�N�L���[
 *  �Ƀ^�X�N�������Ă��邩�ǂ����������r�b�g�}�b�v�̈� bitmap ��p�ӂ�
 *  �Ă���D���̗̈���g�����ƂŁC���[�h�P�ʂ̃T�[�`���\�ɂȂ�C����
 *  ���A�N�Z�X�̉񐔂����炷���Ƃ��ł���D�������C�D��x�̃��x��������
 *  �Ȃ��C�r�b�g���얽�߂��[�����Ă��Ȃ� CPU �̏ꍇ�ɂ́C�r�b�g�}�b�v��
 *  ��̃I�[�o�[�w�b�h�̂��߂ɁC�t�Ɍ�����������\��������DTRON�d�l
 *  �`�b�v�p�̃R�[�h�Ɣėp�R�[�h�ŁC���[�h���̃r�b�g�̔ԍ��t�����t�ɂ�
 *  ���Ă���̂Œ��ӂ���D
 *  �܂��C���f�B�L���[���̍ō��D��x�̃^�X�N���Q�Ƃ��鑀��������悭�s
 *  �����߂ɁC���f�B�L���[���̍ō��D��x�̃^�X�N�̗D��x�� top_priority
 *  �t�B�[���h�ɓ���Ă����D���f�B�L���[����̏ꍇ�ɂ́C���̃t�B�[���h
 *  �̒l�� NUM_PRI �ɂ���D���̎��� tskque[top_priority] ���Q�Ƃ���� 
 *  NULL ��Ԃ��������߁C��� NULL �������Ă��� null�t�B�[���h��p�ӂ�
 *  �Ă���D
 */

#define BITMAPSZ	(sizeof(UINT) * 8)
#define NUM_BITMAP	((NUM_PRI + BITMAPSZ - 1) / BITMAPSZ)

typedef	struct ready_queue {
	INT	top_priority;		/* ���f�B�L���[���̍ō��D��x */
	QUEUE	tskque[NUM_PRI];	/* �D��x���Ƃ̃^�X�N�L���[ */
	TCB	*null;			/* ���f�B�L���[����ɂȂ������̂��� */
	UINT	bitmap[NUM_BITMAP];	/* �D��x���Ƃ̃r�b�g�}�b�v�̈� */
} RDYQUE;

/*
 *  �r�b�g�}�b�v�̈摀�색�C�u����
 *
 *  �ėp�R�[�h�� (1 << xxx) �Ƃ��Ă��镔���́C�e�[�u����p�ӂ���������
 *  �����悢�Ǝv���邪�C�e�[�u���̍����� CPU �̃��[�h���Ɉˑ����Ă�
 *  �܂��̂ŁC(1 << xxx) �̌`�ɂ��Ă���D
 */

/*
 *  ���f�B�L���[ rq �̃r�b�g�}�b�v�̈撆�́C�D��x priority �ɑΉ�����
 *  �r�b�g���Z�b�g����D
 */
Inline void
bitmap_set(RDYQUE *rq, INT priority)
{
#ifdef tron
	Asm("bset %0, %a1" : : "g"(priority), "g"(rq->bitmap) : "memory");
#else /* tron */
	rq->bitmap[priority / BITMAPSZ] |= 1 << (priority % BITMAPSZ);
#endif /* tron */
}

/*
 *  ���f�B�L���[ rq �̃r�b�g�}�b�v�̈撆�́C�D��x priority �ɑΉ�����
 *  �r�b�g���N���A����D
 */
Inline void
bitmap_clear(RDYQUE *rq, INT priority)
{
#ifdef tron
	Asm("bclr %0, %a1" : : "g"(priority), "g"(rq->bitmap) : "memory");
#else /* tron */
	rq->bitmap[priority / BITMAPSZ] &= ~(1 << (priority % BITMAPSZ));
#endif /* tron */
}

/*
 *  �ȉ��� _ffs�֐��́C�W���� ffs�֐��ƈႢ�Ci �� 0 �̏ꍇ�͍l�����Ă�
 *  �炸�C�Ԃ��l�� 0 ���x�[�X�ɂ��Ă���D�܂��CTRON�d�l�`�b�v�p�̃R�[�h
 *  �ł́C�T�[�`����������t�ɂȂ��Ă���D
 *
 *  �W�����C�u������ ffs ������Ȃ�C���̂悤�ɒ�`���āC�W�����C�u����
 *  ���g���������������ǂ��\���������D
 *	#define _ffs(i) (ffs(i) - 1)
 *  TRON�d�l�`�b�v�̕W���ݒ�ł́C�C�Ӓ��r�b�g�t�B�[���h���얽�߂��g��
 *  �̂� _ffs�֐��͎g���Ȃ����Cbvsch ���߂��Ȃ��ꍇ�ɂ͂��̊֐����g��
 *  �K�v������D
 */
Inline INT
_ffs(INT i)
{
#ifdef tron
	Asm("bsch/1/f %0, %2" : "=g"(i) : "0"(i), "rm"(0));
	return(i);
#else /* tron */
	INT	j = 0;

	assert(i != 0);
	while ((i & 1) == 0) {
		i >>= 1;
		j++;
	}
	return(j);
#endif /* tron */
}

/*
 *  ���f�B�L���[�̏�����
 */
Inline void
ready_queue_initialize(RDYQUE *rq)
{
	INT	i;

	rq->top_priority = NUM_PRI;
	for (i = 0; i < NUM_PRI; i++) {
		queue_initialize(&rq->tskque[i]);
	}
	rq->null = (TCB *) 0;
	memset(rq->bitmap, 0, sizeof(rq->bitmap));
}

/*
 *  ���f�B�L���[���̍ō��D��x�̃^�X�N���Q�Ƃ���D
 *
 *  ���f�B�L���[����̎��� NULL ��Ԃ��D
 */
Inline TCB *
ready_queue_top(RDYQUE *rq)
{
	return((TCB *)(rq->tskque[rq->top_priority].next));
}

/*
 *  ���f�B�L���[���̍ō��D��x�̃^�X�N�̗D��x���Q�Ƃ���D
 *
 *  ���f�B�L���[����̎��ɂ͌Ă΂�Ȃ��D
 */
Inline INT
ready_queue_top_priority(RDYQUE *rq)
{
	return(rq->top_priority);
}

/*
 *  �^�X�N�����f�B�L���[�ɑ}������D
 *
 *  tcb �Ŏw���^�X�N�̗D��x�Ɠ����D��x�����^�X�N�̒��ł̍Ō�ɓ���
 *  ��D
 *  �r�b�g�}�b�v�̈�̊Y������r�b�g���Z�b�g���C�K�v�Ȃ� top_priority 
 *  ���X�V����Dtop_priority ���X�V�������� 1�C�����łȂ��ꍇ�� 0 ���
 *  �� (�C�����C���֐��Ȃ̂ŁC�Ԃ�l���s�v�ȏꍇ�ł����ʂȃR�[�h�͏o��
 *  ��)�D
 */
Inline BOOL
ready_queue_insert(RDYQUE *rq, TCB *tcb)
{
	INT	priority = tcb->priority;

	queue_insert(&(tcb->tskque), &(rq->tskque[priority]));
	bitmap_set(rq, priority);
	if (priority < rq->top_priority) {
		rq->top_priority = priority;
		return(1);
	}
	return(0);
}

/*
 *  �^�X�N�����f�B�L���[�ɑ}������D
 *
 *  tcb �Ŏw���^�X�N�̗D��x�Ɠ����D��x�����^�X�N�̒��ł̐擪�ɓ���
 *  ��DRUN��Ԃ̃^�X�N�����f�B�L���[����O���Ă��������ŁC�^�X�N���v
 *  ���G���v�g���ꂽ�ꍇ�Ɏg���D
 *  �r�b�g�}�b�v�̈�̊Y������r�b�g���Z�b�g���C�K�v�Ȃ� top_priority 
 *  ���X�V����D
 */
Inline void
ready_queue_insert_top(RDYQUE *rq, TCB *tcb)
{
	INT	priority = tcb->priority;

	queue_insert(&(tcb->tskque), rq->tskque[priority].next);
	bitmap_set(rq, priority);
	if (priority < rq->top_priority) {
		rq->top_priority = priority;
	}
}

/*
 *  �^�X�N�����f�B�L���[����폜����D
 *
 *  TCB ���Y������D��x�̃^�X�N�L���[����͂����C����ɂ���ă^�X�N�L���[
 *  ����ɂȂ����ꍇ�ɂ́C�r�b�g�}�b�v�̈�̊Y������r�b�g���N���A����D
 *  ����ɁC�폜�����^�X�N���ō��D��x�ł��������́Ctop_priority ���X�V
 *  ����D���̍ۂɁC���ɗD��x�̍����^�X�N���T�[�`���邽�߂ɁC�r�b�g�}�b
 *  �v�̈���g���D
 */
Inline void
ready_queue_delete(RDYQUE *rq, TCB *tcb)
{
#if defined(tron) && !defined(TRON_LEVEL1)
   Asm("qdel %a1, r0	\n"	/* �^�X�N�L���[����̍폜 */
"	bne 1f		\n"	/* �L���[����ɂȂ�Ȃ����͂���ŏI�� */
"	mov %2 ,r0	\n"
"	mov %3, r1	\n"
"	bclr r1, @r0	\n"	/* �Y���r�b�g�̃N���A */
"	cmp %4, r1	\n"
"	bne 1f		\n"	/* ��荂�D��x�̃^�X�N�����鎞�͏I�� */
"	mov %5, r2	\n"
"	sub r1, r2	\n"	/* �r�b�g�T�[�`����r�b�g�����v�Z */
"	bvsch/1/f	\n"
"	mov r1, %0	\n"	/* top_priority ���X�V */
"1:			"
:	"=g"(rq->top_priority)
:	"g"(tcb->tskque.prev), "g"(rq->bitmap), "g"(tcb->priority),
	"g"(rq->top_priority), "g"(NUM_PRI)
:	"r0", "r1", "r2", "memory");
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	INT	priority = tcb->priority;
	INT	i;

	queue_delete(&(tcb->tskque));
	if (!queue_empty_p(&(rq->tskque[priority]))) {
		return;
	}
	bitmap_clear(rq, priority);
	if (priority != rq->top_priority) {
		return;
	}
	for (i = priority / BITMAPSZ; i < NUM_BITMAP; i++) {
		if (rq->bitmap[i]) {
			rq->top_priority = i * BITMAPSZ + _ffs(rq->bitmap[i]);
			return;
		}
	}
	rq->top_priority = NUM_PRI;
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
}

/*
 *  ���f�B�L���[�̗D��x���x�� priority �̃L���[�̐擪�̃^�X�N���C�L���[
 *  �Ō�ֈړ�����D�L���[����̏ꍇ�́C�������Ȃ��D
 *
 *  ready_queue_delete �� ready_queue_insert �𑱂��ČĂ�ł��������Ƃ�
 *  �����ł��邪�Cready_queue_rotate �̏ꍇ�̓r�b�g�}�b�v�̈�̑���͕K
 *  �v�Ȃ����߁C�������悭���邽�߂ɕʂ̊֐��ɂ��Ă���D
 */
Inline void
ready_queue_rotate(RDYQUE *rq, INT priority)
{
#ifdef tron
   Asm("qdel %a0, r0	\n"
"	bvs 1f		\n"
"	qins @r0, %a0	\n"
"1:			"
:	/* no output */
:	"g"(&(rq->tskque[priority]))
:	"r0", "memory");
#else /* tron */
	QUEUE	*entry, *queue;

	if (!queue_empty_p(queue = &(rq->tskque[priority]))) {
		entry = queue_delete_next(queue);
		queue_insert(entry, queue);
	}
#endif /* tron */
}

#endif /* _READY_QUEUE_ */
