#ifndef listworks_h
#define listworks_h

#include "sortworks.h"

/* (not sure where to put this...) */
typedef unsigned int bit_flag;
#define DEL_NONE   0x1
#define DEL_STRUCT 0x2
#define DEL_DATA   0x4
#define DEL_KEYS   0x8
#define DEL_VALS   0x16
typedef int(*comp_func)(void*,void*);


// ilist (interface) {
typedef struct
{
	int(*size)(void*);
	
	void*(*add)(void*,void*);
	void*(*set)(void*,int,void*);
	void*(*insert)(void*,int,void*);
	void*(*remove)(void*,int);
	void*(*remove_object)(void*,void*);
	
	void*(*get)(void*,int);
	void*(*first)(void*);
	void*(*last)(void*);
	
	int(*index)(void*,void*);
	int(*index_comp)(void*,void*,comp_func);
}
ilist;
//}


// arrlist api (or interface?) {

//typedef struct _arrlist arrlist;
typedef struct
{
	int size;
	int truesize;
	float multiplier;
	
	void** data;
} arrlist;

arrlist* alloc_arrlist();
arrlist* init_arrlist(arrlist* arr, int size);
arrlist* new_arrlist(int size);

void del_arrlist(arrlist* arr, bit_flag del_flags);

int sizeof_arrlist();

int arrlist_size(arrlist* arr);

void* arrlist_add(arrlist* arr, void* ob);
void* arrlist_set(arrlist* arr, int index, void* ob);
void* arrlist_insert(arrlist* arr, int index, void* ob);
void* arrlist_remove(arrlist* arr, int index);
void* arrlist_remove_object(arrlist* arr, void* ob);

void* arrlist_get(arrlist* arr, int index);
void* arrlist_first(arrlist* arr);
void* arrlist_last(arrlist* arr);

int arrlist_index(arrlist* arr, void* ob);
int arrlist_index_comp(arrlist* arr, void* ob, comp_func cmp);

ilist* ilist_arrlist(ilist* il);
//}


// nodelist api {

typedef struct
{
	void* next;
	void* data;
} node1;
typedef struct
{
	int size;
	node1* first;
	node1* last;
} nodelist;

nodelist* alloc_nodelist();
nodelist* init_nodelist(nodelist* nl);
nodelist* new_nodelist();

void del_nodelist(nodelist* nl, bit_flag del_flags);

int nodelist_size(nodelist* nl);

void* nodelist_add(nodelist* nl, void* ob);
void* nodelist_set(nodelist* nl, int index, void* ob);
void* nodelist_insert(nodelist* nl, int index, void* ob);
void* nodelist_remove(nodelist* nl, int index);
void* nodelist_remove_object(nodelist* nl, void* ob);

void* nodelist_get(nodelist* nl, int index);
void* nodelist_first(nodelist* nl);
void* nodelist_last(nodelist* nl);

int nodelist_index(nodelist* nl, void* ob);
int nodelist_index_comp(nodelist* arr, void* ob, comp_func cmp);

ilist* ilist_nodelist(ilist* il);
//}


// arrqueue (not sure about this one being here, either...) {

//typedef struct _arrqueue arrqueue;
typedef struct
{
	void** arr;
	int arrsize;
	int size;
	int xn;
	int xd;
} arrqueue;

arrqueue* alloc_arrqueue();
arrqueue* init_arrqueue(arrqueue* q, void** arr, int size);
arrqueue* new_arrqueue(void*,int);

void del_arrqueue(arrqueue*,bit_flag);

int sizeof_arrqueue();

int arrqueue_size(arrqueue*);

void* arrqueue_nq(arrqueue*,void*);
void* arrqueue_dq(arrqueue*);

void* iter_arrqueue(arrqueue*,int*);
void* iter_arrqueue_next(arrqueue*,int*);
int iter_arrqueue_has_next(arrqueue*,int*);

// }


// binary search tree api (should this really be in this file?) {
//typedef struct _bintree bintree;

typedef struct
{
	int weight;
	void* parent;
	void* left;
	void* right;
	void* data;
} bintree_node;

typedef struct
{
	int size;
	bintree_node* root;
	compfunc comp;
} bintree;

bintree* alloc_bintree();
bintree* init_bintree(bintree*,comp_func);
bintree* new_bintree(comp_func);

void del_bintree(bintree*,bit_flag);

void* bintree_add(bintree*,void*);
void* bintree_contains(bintree*,void*);

void* iter_bintree(bintree*);
void* iter_bintree_next(void**);
int iter_bintree_has_next(void**);

//}


#endif

