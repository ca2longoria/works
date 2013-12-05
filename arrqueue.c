
#include <stdlib.h>
#include "listworks.h"




// initialization {
arrqueue* alloc_arrqueue()
{
	return (arrqueue*)malloc(sizeof(arrqueue));
}
arrqueue* init_arrqueue(arrqueue* q, void** arr, int size)
{
	q->arr = (arr ? arr : (void**)malloc(sizeof(void*)*size));
	q->arrsize = size;
	q->size = 0;
	q->xn = 0;
	q->xd = 0;
	
	return q;
}
arrqueue* new_arrqueue(void* target, int size)
{
	return init_arrqueue(alloc_arrqueue(),target,size);
}
//}

// deletion {
void del_arrqueue(arrqueue* q, bit_flag del_flags)
{
	bit_flag flags = (del_flags ? del_flags : DEL_STRUCT);
	// DEL_DATA will free the target array
	
	if (flags & DEL_DATA)
	{
		// Dequeue items and DESTROY THEM ALL
		void* a;
		while (a = arrqueue_dq(q))
			free(a); 
		
		// Then free
		free(q->arr);
	}
	
	if (flags & DEL_STRUCT)
		free(q);
}
//}

// arrqueue functions {
int sizeof_arrqueue()
{
	return sizeof(arrqueue);
}

int arrqueue_size(arrqueue* q)
{
	return q->size;
}

void* arrqueue_nq(arrqueue* q, void* p)
{
	if (q->size == q->arrsize)
		return 0;
	
	q->size++;
	q->arr[q->xn % q->arrsize] = p;
	q->xn++;
}

void* arrqueue_dq(arrqueue* q)
{
	if (q->xd == q->xn)
		return 0;
	
	if (q->xd >= q->size)
	{
		q->xd -= q->size;
		q->xn -= q->size;
	}
	
	q->size--;
	return q->arr[q->xd++];
}

void* iter_arrqueue(arrqueue* q, int* handle)
{
	// Assumed to be an... int**?
	int* ip = (int*)handle;
	*ip = q->xd;
	return q->arr[q->xd];
}

void* iter_arrqueue_next(arrqueue* q, int* handle)
{
	int* ip = (int*)handle;
	if (*ip == q->xn) return 0;
	void* ret = q->arr[*ip % q->arrsize];
	(*ip)++;
	return ret;
}

int iter_arrqueue_has_next(arrqueue* q, int* handle)
{
	int* ip = (int*)handle;
	return (*ip < q->arrsize);
}
//}

// test code {
#ifdef arrqueue_active

#include <stdio.h>

#ifdef mtrace_active
#include <mcheck.h>
#endif

int main(int argc, char** argv)
{
#ifdef mtrace_active
	mtrace();
#endif
	
	printf("testing arrqueue\n");
	
	// Test bare alloc and del
	arrqueue* q = new_arrqueue(0,10);
	del_arrqueue(q,(DEL_STRUCT|DEL_DATA));
	
	//*
	int i;
	char* arr[] = {"Articulate","Breathe","Circulate","Diaphragm","Esophagus"};
	q = new_arrqueue(arr,5);
	
	// Straight-up insertion
	printf("size before: %i\n", arrqueue_size(q));
	for (i=0; i < q->arrsize; ++i)
		arrqueue_nq(q,arr[i]);
	for (i=0; i < q->arrsize; ++i)
		printf("%s ",q->arr[i]); printf("\n");
	printf("size after: %i\n", arrqueue_size(q));
	
	// 
	void* a;
	void* b;
	int it;
	char* s;
	a = arrqueue_dq(q);
	b = arrqueue_dq(q);
	
	iter_arrqueue(q,&it);
	while (s = iter_arrqueue_next(q,&it))
		printf("%s ",s); printf("\n");
	
	arrqueue_nq(q,b);
	arrqueue_nq(q,a);
	
	iter_arrqueue(q,&it);
	while (s = iter_arrqueue_next(q,&it))
		printf("%s ",s); printf("\n");
	
	del_arrqueue(q,0);
	//*/
	
#ifdef mtrace_active
	printf("\nMTRACE OUTPUT:\n");
	muntrace();
#endif
	
	return 0;
}

#endif
//}

