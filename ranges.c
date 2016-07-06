
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "intworks.h"


//typedef uint32_t I;
#define I uint32_t

#define range range32u
//typedef rangeI range;

int comp_range32u(const void* a, const void* b) {
	I x = ((range*)a)->a - ((range*)b)->a;
	return (x ? x : ((range*)a)->b - ((range*)b)->b);
}

int sorted_range32u_inplace_merge(range* r, int len) {
	int i,x = 0;
	range current = r[0];
	for (i=0; i < len; ++i) {
		//printf("%i,%i vs %i,%i\n",current.a,current.b,r[i].a,r[i].b);
		if (current.b < r[i].a) {
			// push current
			r[x++] = current;
			current = r[i];
		}
		else if (current.b < r[i].b) {
			//printf("b < r.b: %i < %i\n", current.b,r[i].b);
			current.b = r[i].b;
		}
	}
	if (len)
		r[x++] = current;
	return x;
}

char* sprint_range32u_array(char* cbuf, range* r, int len) {
	int i;
	int x = 0;
	char* s;
	for (i=0; i < len; ++i) {
		s = &cbuf[x];
		sprintf(s,"(%i,%i) ",r[i].a,r[i].b);
		x += strlen(s);
	}
	cbuf[x-1] = 0;
	return cbuf;
}

void print_range32u_array(range* r, int len) {
	int i;
	for (i=0; i < len; ++i) {
		printf("(%i,%i) ",r[i].a,r[i].b);
	}
	printf("\n");
}

#ifdef ranges_active

int main() {
	
	int i;
	int len = 8;
	printf("What even?\n");
	
	range yall[len];
	I these[] = {5,7, 2,4, 4,6, 1,4, 8,11, 2,3, 8,8, 4,5};
	for (i=0; i < len; ++i) {
		yall[i].a = these[i<<1];
		yall[i].b = these[(i<<1)+1];
	}
	
	print_range32u_array(yall,len);
	
	qsort((void*)yall,len,sizeof(range),comp_range32u);
	
	print_range32u_array(yall,len);
	
	int newlen = sorted_range32u_inplace_merge(yall,len);
	
	print_range32u_array(yall,newlen);
	
	return 0;
}

#undef range
#undef I

#endif

