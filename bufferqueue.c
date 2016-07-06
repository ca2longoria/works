
// listworks.h should be included in bufferworks.h.
#include <stdint.h>
#include "listworks.h"
#include "bufferworks.h"


// Generic Buffer Constructor

int leftmost_bit_uint32(uint32_t val) {
	char* p = (char*)&val;
	char c;
	int a,b;
	//printf("val: %u\n",val);
	//printf("p %x,%x,%x,%x\n",p[3],p[2],p[1],p[0]);
	if (val&0xffff0000) {
		if (val&0xff000000)
			c = p[a=3];
		else if (val&0x00ff0000)
			c = p[a=2];
	}
	else if (val&0x0000ffff) {
		if (val&0x0000ff00)
			c = p[a=1];
		else if (val&0x000000ff)
			c = p[a=0];
	}
	else
		return 0;
	//printf("a be: %i,%u\n",a,c);
	for (b=8; b>0 && !(c&0x80); --b)
		c <<= 1;
	return (int)b+(a*8);
}

//*
#define define_buffer_factory(name,T) \
\
typedef struct name ## _buffer_factory {\
	nodelist* qarr[33];\
} name ## _buffer_factory;\
\
name ## _buffer_factory* init_ ## name ## _buffer_factory(name ## _buffer_factory* f) {\
	int i;\
	for (i=0; i <= 32; ++i)\
		f->qarr[i] = new_nodelist();\
}\
\
void del_ ## name ## _buffer_factory(name ## _buffer_factory* f, bit_flag del_flags) {\
	int i;\
	for (i=0; i <= 32; ++i)\
		del_nodelist(f->qarr[i],(DEL_STRUCT|DEL_DATA));\
	if (del_flags & DEL_STRUCT)\
		free(f);\
}\
\
int name ## _buffer_factory_fill(name ## _buffer_factory* f, int count, uint32_t flags) {\
	void* p;\
	uint32_t a = flags;\
	uint32_t m = 1;\
	int j,i=0;\
	int sum = 0;\
	printf("f,count,flags: %x, %u, 0x%x\n",f,count,flags);\
	printf("type " #T ", size: %u\n",sizeof(T));\
	for (a=flags; a; a >>= 1) {\
		printf("a,i,m: 0x%02x, %2u, %2u\n",a,i,m);\
		if (a&1) {\
			/* Add the new buffers, here.*/\
			printf("Adding %u buffers at %u, size %u.\n",count,i,sizeof(T)*m);\
			for (j=0; j < count; ++j) {\
				p = (void*)malloc(sizeof(T)*m);\
				nodelist_add(f->qarr[i],p);\
			}\
			sum += sizeof(T)*m*count;\
		}\
		m <<= 1;\
		i++;\
	}\
	return sum;\
}\
\
T* name ## _buffer_factory_dq(name ## _buffer_factory* f, uint32_t length, int* size) {\
	/* Determine leftmost bit.\
	// WARNING: architecture-dependent*/\
	T* ret;\
	uint32_t true_size;\
	int bit = leftmost_bit_uint32((length > 1 ? length-1 : length));\
	if (!bit)\
		return 0;\
	bit = (length > (true_size=(1<<bit)) ? bit+1 : bit);\
	\
	nodelist* n;\
	if (size)\
		*size = true_size;\
	if ((n=f->qarr[bit])->size == 0) {\
		/* Create a new one, if none are left to dequeue.*/\
		return (T*)malloc(sizeof(T)*true_size);\
	}\
	ret = (T*)nodelist_remove(n,0);\
	return ret;\
}\
\
void name ## _buffer_factory_nq(name ## _buffer_factory* f, T* p, uint32_t length) {\
	int bit = leftmost_bit_uint32((length > 1 ? length-1 : length));\
	if (!bit)\
		return;\
	bit = (length > (1<<bit) ? bit+1 : bit);\
	nodelist_add(f->qarr[bit],(void*)p);\
}\

//*/


#ifdef bufferqueue_active
#include <time.h>
#include <stdio.h>
#include <stdint.h>

uint64_t utime() {
	struct timeval t;
	gettimeofday(&t,0);
	// Apparently, -ull appended can represent unsigned long long.
	return t.tv_sec*1000000ull+t.tv_usec;
}

define_buffer_factory(ip,uint32_t)
define_buffer_factory(char,char)

int main(int argc, char** argv) {
	
	printf("Oh for the goodness of sake.\n");
	
	
	int i;
	uint32_t these[] = {0x0,0x1,0x2,0x4,0x8,0x16,0x32,0x64,0x228};
	for (i=0; i < 16; ++i)
		printf("yeah... %3x: %i\n",i,leftmost_bit_uint32(i));
	
	ip_buffer_factory a;
	char_buffer_factory b;
	
	init_ip_buffer_factory(&a);
	
	ip_buffer_factory_fill(&a,2,0x10);
	printf("nlist size: %u\n",a.qarr[4]->size);
	
	int what;
	uint32_t* ok = ip_buffer_factory_dq(&a,8,&what);
	printf("what... %u\n",what);
	printf("now, nlist3 size: %u\n",a.qarr[3]->size);
	printf("now, nlist4 size: %u\n",a.qarr[4]->size);
	ip_buffer_factory_nq(&a,ok,8);
	printf("finally, nlist1 size: %u\n",a.qarr[1]->size);
	printf("finally, nlist3 size: %u\n",a.qarr[3]->size);
	printf("finally, nlist4 size: %u\n",a.qarr[4]->size);
	del_ip_buffer_factory(&a,0);
	
	printf("Just because, you, like all and... what even...!\n");
	
	// Now, to benchmark this bad boi.
	
	// List of random numbers, seeded?
	
	// nq(number)/dq(number) vs malloc(number)/free(number)
	
	#define MANY 1000000
	const int seed = 0xbeef;
	uint16_t rands[MANY];
	srand(seed);
	
	uint64_t start,end;
	uint32_t hilo[2];
	
	printf("Build random length array... ");
	start = utime();
	for (i=0; i < MANY; ++i)
		rands[i] = (uint16_t)rand();
	end = utime();
	printf("%u us\n",end-start);
	
	ip_buffer_factory q;
	uint32_t* deese;
	int truesize;
	
	init_ip_buffer_factory(&q);
	
	printf("Time dq/nq: ");
	start = utime();
	for (i=0; i < MANY; ++i) {
		deese = ip_buffer_factory_dq(&q,rands[i],&truesize);
		ip_buffer_factory_nq(&q,deese,rands[i]);
	}
	end = utime();
	printf("%u us\n",end-start);
	
	del_ip_buffer_factory(&q,0);
	
	printf("Time malloc/free: ");
	start = utime();
	for (i=0; i < MANY; ++i) {
		deese = (uint32_t*)malloc(sizeof(uint32_t)*rands[i]);
		free(deese);
	}
	end = utime();
	printf("%u us\n",end-start);
	
	
	return 0;
}
#endif

