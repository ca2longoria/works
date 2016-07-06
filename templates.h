#ifndef templates_h
#define templates_h


#ifndef DEL_NONE
#define DEL_NONE   0x1
#define DEL_STRUCT 0x2
#define DEL_DATA   0x4
#define DEL_KEYS   0x8
#define DEL_VALS   0x16
#endif


#define define_arrlist(T,name)\
typedef struct {\
	int size;\
	int truesize;\
	T* arr;\
} name ## _arrlist;\
\
name ## _arrlist* alloc_ ## name ## _arrlist() {\
	return (name ## _arrlist*)malloc(sizeof(name ## _arrlist));\
}\
\
name ## _arrlist* init_ ## name ## _arrlist(name ## _arrlist* r, unsigned int truesize) {\
	truesize = (truesize ? truesize : 10);\
	r->size = 0;\
	r->truesize = truesize;\
	r->arr = (T*)malloc(sizeof(T)*truesize);\
	return r;\
}\
\
name ## _arrlist* new_ ## name ## _arrlist(unsigned int truesize) {\
	return init_ ## name ## _arrlist(alloc_ ## name ## _arrlist(),truesize);\
}\
\
void del_ ## name ## _arrlist(name ## _arrlist* r, unsigned int flags) {\
	free(r->arr);\
	if (flags & DEL_STRUCT)\
		free(r);\
}\
\
void name ## _arrlist_zero(name ## _arrlist* r) {\
	T* p = &r->arr[0];\
	T* stop = &r->arr[r->truesize];\
	while (p != stop)\
		*p++ = 0;\
}\
\
unsigned int sizeof_ ## name ## _arrlist(name ## _arrlist* r) {\
	int size = sizeof(name ## _arrlist);\
	size += sizeof(T) * r->truesize;\
	return size;\
}\
\
T name ## _arrlist_add(name ## _arrlist* r, T v) {\
	int i;\
	T* p;\
	if (r->size == r->truesize) {\
		p = (T*)malloc(sizeof(T) * (r->truesize<<1));\
		for (i=0; i < r->truesize; ++i)\
			p[i] = r->arr[i];\
		free(r->arr);\
		r->arr = p;\
		r->truesize = (r->truesize<<1);\
	}\
	r->arr[r->size] = v;\
	r->size += 1;\
	return v;\
}\
\
T name ## _arrlist_insert(name ## _arrlist* r, unsigned int index, T v) {\
	int i;\
	T noid;\
	name ## _arrlist_add(r,noid);\
	for (i=index+1; i < r->size; ++i)\
		r->arr[i] = r->arr[i-1];\
	r->arr[index] = v;\
	return v;\
}\
\
T name ## _arrlist_remove(name ## _arrlist* r, unsigned int index) {\
	int i;\
	T ret = r->arr[index];\
	for (i=index; i < r->size-1; ++i)\
		r->arr[i] = r->arr[i+1];\
	r->size -= 1;\
	return ret;\
}

#endif

