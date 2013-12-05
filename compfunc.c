
#include "sortworks.h"

// Comparison Functions {
int comp_int(void* pa, void* pb)
{
  int a = *(int*)pa;
  int b = *(int*)pb; 
  return (a < b ? -1 : (a > b ? 1 : 0));
}

int comp_float(void* pa, void* pb)
{
  float a = *(float*)pa;
  float b = *(float*)pb; 
  return (a < b ? -1 : (a > b ? 1 : 0));
}

int comp_string(void* pa, void* pb)
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
//}

