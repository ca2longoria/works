
#include <stdio.h>
#include <stdlib.h>

#include "sortworks.h"


void** merge_sort_merge(void** arr, void** buf, int a, int b, int c, compfunc comp)
{
  int bind = 0;
  int r = 0;
  int ia = a;
  int ib = b;
  
  while (ia < b && ib < c)
  {
    r = comp(arr[ia],arr[ib]);
    if (r < 0)
      buf[bind++] = arr[ia++];
    else if (r > 0)
      buf[bind++] = arr[ib++];
    else
    {
      buf[bind++] = arr[ia++];
      buf[bind++] = arr[ib++];
    }
  }
  
  while (ia < b)
    buf[bind++] = arr[ia++];
  
  while (ib < c)
    buf[bind++] = arr[ib++];
  
  bind = 0;
  for (r = a; r < c; ++r)
    arr[r] = buf[bind++];
  
  return arr;
}

void** merge_sort_rec(void** arr, void** buf, int a, int b, compfunc comp)
{
	//printf("merge_sort_rec: %x, %x, %i, %i, %x\n",arr,buf,a,b,comp);
	
  int m,span;
  void** left;
  void** right;
  
  span = b-a;
  if (span < 2)
    return arr;
  
  m = a + span/2;
  
  left = merge_sort_rec(arr,buf,a,m,comp);
  right = merge_sort_rec(arr,buf,m,b,comp);
  
  return merge_sort_merge(arr,buf,a,m,b,comp);
}


void** merge_sort(void** arr, int len, compfunc comp)
{
  /* arr is presumed an array of void pointers */
  void** buf = 0;
  void** result;
  buf = malloc(sizeof(void*)*len);
  result = merge_sort_rec(arr,buf,0,len,comp);
  free(buf);
  return result;
}



//#define mergesort_active

#ifdef mergesort_active

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int i;
	
	int len = 8;
	int arr[] = {5,3,6,4,7,2,8,1};
	int** brr = malloc(sizeof(int*)*len);
	
	for (i=0; i<len; ++i)
		brr[i] = &arr[i];
	
	printf("unsorted\n");
	for (i=0; i<len; ++i)
		printf("brr[%i]: %i\n",i,*brr[i]);
	
	merge_sort(brr,len,comp_int);
	printf("sorted\n");
	for (i=0; i<len; ++i)
		printf("brr[%i]: %i\n",i,*brr[i]);
	
	free(brr);
	
	return 0;	
}

#endif

