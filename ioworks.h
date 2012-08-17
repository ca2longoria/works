#ifndef ioworks_h
#define ioworks_h

#include <stringworks.h>

char* readline(FILE* file, char* cbuf)
{
  char c = 1;
  stringbuilder* sb = init_stringbuilder(alloc_stringbuilder(),256);
  while (fgets(&c,2,file))
  {
    if (c == '\n')
      break;
    stringbuilder_append_char(sb,c);
    /*printf("%c %i\n",c,(int)c);*/
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

#endif

