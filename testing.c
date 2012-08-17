
#include <stdio.h>
#include <bufferworks.h>
#include <sortworks.h>
#include <stringworks.h>
#include <timeworks.h>


struct _bunch
{
  char arr[1024];
};
typedef struct _bunch bunch;
buffermanager_define_type(bunch);

int main(int argc, char** argv)
{
  int i;
  int sum = 0;
  int count = 10000000;
  int* ival;
  
  llong start, end;
  
  /*
  start = usectime();
  for (i = 0; i < count; ++i)
  {
    ival = buf_int_pull();
    *ival = sum++;
    buf_int_push(ival);
  }
  end = usectime();
  
  printf("using buffer: %lld\n", end-start);
  
  start = usectime();
  for (i = 0; i < count; ++i)
  {
    ival = malloc(sizeof(int));
    *ival = sum++;
    free(ival);
  }
  end = usectime();
  
  printf("using malloc: %lld\n", end-start);
  */
  
  clear_buffermanagers();
  
  return 0;
}

