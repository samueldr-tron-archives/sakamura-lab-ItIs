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
 *  @(#) $Id: queue.h,v 1.6 1996/02/17 09:33:28 hiro Exp $
 */

#ifndef	_QUEUE_
#define	_QUEUE_

/*
 *  �_�u�������N�L���[
 * 
 *  �_�u�������N�L���[�̍\���́CTRON�d�l�`�b�v�̃L���[���얽�߂������`
 *  ���ɂ��킹�Ă���D
 */
typedef struct queue {
	struct queue *next;
	struct queue *prev;
} QUEUE;

/*
 *  �L���[���색�C�u����
 */

/*
 *  �L���[�̏�����
 */
Inline void
queue_initialize(QUEUE *queue)
{
	queue->prev = queue->next = queue;
}

/*
 *  �L���[�փG���g����}��
 *
 *  queue �̒��O�� entry ��}������Dqueue ���L���[�w�b�_���w���ꍇ�ɂ́C
 *  �L���[�̍Ō���� entry ��}�����邱�ƂɂȂ�D
 */
Inline void
queue_insert(QUEUE *entry, QUEUE *queue)
{
#ifdef tron
	Asm("qins %a0, %a1" : : "g"(entry), "g"(queue) : "memory");
#else /* tron */
	entry->prev = queue->prev;
	entry->next = queue;
	queue->prev->next = entry;
	queue->prev = entry;
#endif /* tron */
}

/*
 *  �L���[����G���g�����폜
 *
 *  entry ���L���[����폜����D
 */
Inline void
queue_delete(QUEUE *entry)
{
#ifdef tron
	Asm("qdel %a0, r0" : : "g"(entry->prev) : "r0", "memory");
#else /* tron */
	if (entry->next != entry) {
		entry->prev->next = entry->next;
		entry->next->prev = entry->prev;
	}
#endif /* tron */
}

/*
 *  �L���[�̐擪�̃G���g���̎��o��
 *
 *  queue �̒���̃G���g�����L���[����폜���C�폜�����G���g����Ԃ��D
 *  queue ���L���[�w�b�_���w���ꍇ�ɂ́C�L���[�̐擪�̃G���g�������o
 *  �����ƂɂȂ�D
 */
Inline QUEUE *
queue_delete_next(QUEUE *queue)
{
	QUEUE	*entry;

	assert(queue->next != queue);
#ifdef tron
	Asm("qdel %a1, %0" : "r="(entry) : "g"(queue) : "memory");
#else /* tron */
	entry = queue->next;
	queue->next = entry->next;
	entry->next->prev = queue;
#endif /* tron */
	return(entry);
}

/*
 *  �L���[���̃G���g���̃T�[�`
 *
 *  queue �Ŏ����L���[����Coffset �Ŏ����t�B�[���h�� val ���傫����
 *  �G���g�����T�[�`���C���̃G���g����Ԃ��D�Y������G���g�����Ȃ��ꍇ
 *  �́Cqueue ��Ԃ��D
 */
Inline QUEUE *
queue_search_gt(QUEUE *queue, INT val, INT offset)
{
#if defined(tron) && !defined(TRON_LEVEL1)
	register QUEUE	*r0 asm("r0") = queue;
	register QUEUE	*r2 asm("r2") = queue;
	register INT	r3 asm("r3") = val + 1;
	register INT	r5 asm("r5") = offset;

	Asm("qsch/ge/f"
	  : "g="(r0)
	  : "0"(r0), "g"(r2), "g"(r3), "g"(r5)
	  : "r1");
	return(r0);
#else /* defined(tron) && !defined(TRON_LEVEL1) */
	QUEUE	*entry;

	for (entry = queue->next; entry != queue; entry = entry->next) {
		if (*((INT *)(((VB *) entry) + offset)) > val) {
			break;
		}
	}
	return(entry);
#endif /* defined(tron) && !defined(TRON_LEVEL1) */
}

/*
 *  �L���[���󂩂ǂ����̃`�F�b�N
 */
Inline BOOL
queue_empty_p(QUEUE *queue)
{
	if (queue->next == queue) {
		assert(queue->prev == queue);
		return(1);
	}
	return(0);
}

#endif /* _QUEUE_ */
