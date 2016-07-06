
#ifndef intworks_h
#define intworks_h

#include <stdint.h>

typedef struct {
	int32_t a;
	int32_t b;
} range32;

typedef struct {
	uint32_t a;
	uint32_t b;
} range32u;

typedef struct {
	int32_t a;
	int32_t b;
} range64;

typedef struct {
	uint32_t a;
	uint32_t b;
} range64u;

int comp_range32(const void*,const void*);
int comp_range64(const void*,const void*);

int comp_range32u(const void*,const void*);
int comp_range64u(const void*,const void*);

int sorted_range32_inplace_merge(range32*,int);
int sorted_range64_inplace_merge(range64*,int);

int sorted_range32u_inplace_merge(range32u*,int);
int sorted_range64u_inplace_merge(range64u*,int);


#endif

