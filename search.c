
#include <stdlib.h>
#include <stdio.h>

#include "sortworks.h"


int linear_search(void** arr, int len, void* ob, compfunc comp)
{
	int i;
	for (i=0; i < len; ++i)
		if (comp(ob,arr[i]) == 0)
			return i;
	return -1;
}

int linear_search_reverse(void** arr, int len, void* ob, compfunc comp)
{
	int i;
	for (i=len-1; i >= 0; --i)
		if (comp(ob,arr[i]) == 0)
			return i;
	return -1;
}


int binary_search(void** arr, int len, void* ob, compfunc comp)
{
	int a,b,c,r;
	a = 0;
	b = len>>1;
	c = len;
	
	while (a < c)
	{
		//printf("a,b,c, s-s: %i,%i,%i : %s vs %s\n",a,b,c,ob,arr[b]);
		r = comp(ob,arr[b]);
		if (r < 0)
			c = b;
		else if (r > 0)
			a = b+1;
		else
			return b;
		b = (a + c)>>1;
	}
	
	return ~b;
}



//#define search_active

#ifdef search_active
#include <stdio.h>
#include <string.h>

#include "listworks.h"


int comp_string(void* a, void* b)
{
	return strcmp(a,b);
}

int comp_string_reverse(void* a, void* b)
{
	return strcmp(b,a);
}

int main(int argv, char** args)
{
	char* arr[] = {"Apple","Banana","Corgy","Deer","Effigy","Figure","Goop"};
	int len = 7;
	
	int i;
	printf("linear_search\n");
	for (i=0; i < len; ++i)
		printf("arr[%i]: %i\n",i,linear_search((void**)arr,len,arr[i],comp_string));
	
	printf("binary_search\n");
	for (i=0; i < len; ++i)
		printf("arr[%i]: %i\n",i,binary_search((void**)arr,len,arr[i],comp_string));
	
	printf("finding %s: %i\n","Aardvark",binary_search((void**)arr,len,"Aardvark",comp_string));
	printf("finding %s: %i\n","Art",binary_search((void**)arr,len,"Art",comp_string));
	printf("finding %s: %i\n","Cat",binary_search((void**)arr,len,"Cat",comp_string));
	
	return 0;
}

#endif

