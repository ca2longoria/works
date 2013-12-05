
#include <stdio.h>

#include "fileworks.h"
#include "stringworks.h"

int filesize(FILE* fp)
{
  int prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp,prev,SEEK_SET);
  return sz;
}


//*
// * These depend on stringworks, so I'm not sure how to integrate them here, yet.

char* readline(FILE* file, char* cbuf)
{
	char cp[] = {'x',0};
  stringbuilder* sb = init_stringbuilder(alloc_stringbuilder(),256);
  
	while (fgets(cp,2,file))
  {
    if (*cp == '\n')
      break;
    stringbuilder_append_char(sb,*cp);
    //printf("%c %i\n",c,(int)c);
  }
  if (!sb->length && feof(file))
    cbuf = 0;
  else
    cbuf = str_stringbuilder(sb,cbuf);
  del_stringbuilder(sb);
  return cbuf;
}

char* readline_new(FILE* file, char** cbufptr)
{
  char* cbuf;
  cbuf = readline(file,0);
  if (cbufptr)
		*cbufptr = cbuf;
  return cbuf;
}
//*/

