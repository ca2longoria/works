#ifndef sortworks_h
#define sortworks_h

#include <stdio.h>
#include <stdlib.h>

typedef int(*compfunc)(void*,void*);

int compfunc_int(void* pa, void* pb)
{
  int a = *(int*)pa;
  int b = *(int*)pb; 
  return (a < b ? -1 : (a > b ? 1 : 0));
}

int compfunc_float(void* pa, void* pb)
{
  float a = *(float*)pa;
  float b = *(float*)pb; 
  return (a < b ? -1 : (a > b ? 1 : 0));
}

int compfunc_str(void* pa, void* pb)
{
  char* a = (char*)pa;
  char* b = (char*)pb;
  
  while (*a && *b)
  {
    if (*a == *b)
    { 
      ++a; ++b;
      continue;
    }
    return (*a < *b ? -1 : (*a > *b ? 1 : 0));
  }
  if (*a) return 1;
  if (*b) return -1;
  return 0;
}


/* And here lies the famous Merge Sort */

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

int blarg = 0;

void** merge_sort_rec(void** arr, void** buf, int a, int b, compfunc comp)
{
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
  void** buf;
  void** result;
  buf = malloc(sizeof(void*)*(len/2+1));
  result = merge_sort_rec(arr,buf,0,len,comp);
  free(buf);
  return result;
}


#endif

