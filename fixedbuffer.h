#ifndef fixedbuffer_h
#define fixedbuffer_h

#include <stdio.h>
#include <stdint.h>
#include "listworks.h"


#define define_fixed_buffer_queue(name,T) \
typedef struct {\
	uint a;\
	uint b;\
	uint size;\
	uint bufsize;\
	T** qarr;\
} fixed_ ## name ## _buffer_queue;\
\
fixed_ ## name ## _buffer_queue* alloc_fixed_ ## name ## _buffer_queue() {\
	return (fixed_ ## name ## _buffer_queue*)malloc(sizeof(fixed_ ## name ## _buffer_queue));\
}\
\
fixed_ ## name ## _buffer_queue* init_fixed_ ## name ## _buffer_queue(\
		fixed_ ## name ## _buffer_queue* q,\
		uint bufsize,\
		uint size) {\
	q->a = 0;\
	q->b = 0;\
	q->bufsize = bufsize;\
	q->size = size;\
	q->qarr = (T**)malloc(sizeof(T*)*size);\
	int i;\
	T** p = q->qarr;\
	T** end = &(q->qarr[size-1]);\
	while (p <= end) {\
		*p = 0;\
		p++;\
	}\
	return q;\
}\
\
fixed_ ## name ## _buffer_queue* new_fixed_ ## name ## _buffer_queue(\
		uint bufsize,\
		uint size) {\
	return init_fixed_ ## name ## _buffer_queue(alloc_fixed_ ## name ## _buffer_queue(),bufsize,size);\
}\
\
void del_fixed_ ## name ## _buffer_queue(fixed_ ## name ## _buffer_queue* q, uint flags) {\
	T** p = q->qarr;\
	T** end = &(q->qarr[q->size-1]);\
	while (p <= end) {\
		if (*p)\
			free(*p);\
		p++;\
	}\
	free(q->qarr);\
	if (flags & DEL_STRUCT)\
		free(q);\
}\
\
uint sizeof_fixed_ ## name ## _buffer_queue(fixed_ ## name ## _buffer_queue* q) {\
	int size = sizeof(fixed_ ## name ## _buffer_queue);\
	size += sizeof(T*) * q->size;\
	T** p = q->qarr;\
	T** end = &(q->qarr[q->size-1]);\
	while (p <= end) {\
		if (*p)\
			size += sizeof(T) * q->bufsize;\
		p++;\
	}\
	return size;\
}\
\
T* fixed_ ## name ## _buffer_queue_dq(fixed_ ## name ## _buffer_queue* q) {\
	uint i = q->a % q->size;\
	if (!q->qarr[i]) {\
		q->qarr[i] = (T*)malloc(sizeof(T)*q->bufsize);\
	}\
	T* ret = q->qarr[i];\
	if (q->a == q->b)\
		q->b += 1;\
	q->a += 1;\
	return ret;\
}\
\
T* fixed_ ## name ## _buffer_queue_nq(fixed_ ## name ## _buffer_queue* q) {\
	uint i = q->b % q->size;\
	if (!q->qarr[i])\
		q->qarr[i] = (T*)malloc(sizeof(T)*q->bufsize);\
	q->b += 1;\
	return q->qarr[i];\
}



#endif

