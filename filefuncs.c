
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


char* readlines_callback2(FILE* f, char* cbuf, int len, readline_callback2 call, void* param) {
	int i,x,count;
	int line=0;
	int offset=0;
	int remainder=0;
	char* fred=0;
	readlines_param p1;
	cbuf = (cbuf ? cbuf : (fred=(char*)malloc(sizeof(char)*len+1)));
	while (!feof(f)) {
		count = fread(&cbuf[remainder],1,len-remainder,f);
		x = 0;
		for (i=0; i < len; ++i) {
			if (cbuf[i] == '\n') {
				cbuf[i] = 0;
				p1.s = &cbuf[x];
				p1.length = i-x;
				p1.line = line;
				p1.offset = offset;
				call(&p1,param);
				line++;
				offset += (i-x) + 1;
				x = i+1;
			}
		}
		if (count < len-remainder)
			break;
		remainder = len-x;
		for (i=0; i < remainder; ++i)
			cbuf[i] = cbuf[x+i];
		//x = 0;
	}
	if (fred)
		free(cbuf);
}
