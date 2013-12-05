#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "numdefs.h"
#include "listworks.h"
#include "stringworks.h"


/*{ string functions*/
// NOTE: These may be generic enough to merit placement in their
//   own c file, but they can go here for now.
int strcharindex(char* s, char c, int reverse)
{
  int i,len;
  if (reverse)
  {
    len = strlen(s);
    for (i = len-1; i >= 0; --i)
      if (s[i] == c)
        return i;
  }
  else
  {
    for (i = 0; s[i]; ++i)
      if (s[i] == c)
        return i;
  }
  return -1;
}
/*}*/


/* A stringbuilder for the masses! */


/* initialization functions */
stringbuilder* alloc_stringbuilder()
{
  return malloc(sizeof(stringbuilder));
}

stringbuilder* init_stringbuilder(stringbuilder* sb, int buflen)
{
  sb->cbufs = init_nodelist(alloc_nodelist());
  
  buflen = (buflen ? buflen : stringbuilder_default_xlength);
  sb->xind = buflen;
  sb->xlen = buflen;
  sb->length = 0;
  return sb;
}

/* deletion functions: del_data will clear just the data, in case sb isn't dynamically allocated */
void del_data_stringbuilder(stringbuilder* sb)
{
  del_nodelist(sb->cbufs,DEL_DATA);
}
void del_stringbuilder(stringbuilder* sb)
{
  del_data_stringbuilder(sb);
  free(sb);
}

/* append functions: strings, chars, ints, floats */
stringbuilder* stringbuilder_append_str(stringbuilder* sb, char* s)
{
  char* cbuf;
  cbuf = nodelist_last(sb->cbufs);
  
	while (*s)
  {
    if (sb->xind == sb->xlen)
    {
      nodelist_add(sb->cbufs,malloc(sizeof(char)*sb->xlen));
      cbuf = nodelist_last(sb->cbufs);
      sb->xind = 0;
    }
    cbuf[sb->xind++] = *s;
    ++sb->length;
    ++s;
  }
  
  return sb;
}

stringbuilder* stringbuilder_append_char(stringbuilder* sb, char c)
{
  char cs[] = {c,0};
  stringbuilder_append_str(sb,cs);
}

stringbuilder* stringbuilder_append_int(stringbuilder* sb, int i)
{
  char cs[12];
  sprintf(cs,"%i",i);
  return stringbuilder_append_str(sb,cs);
}

stringbuilder* stringbuilder_append_float(stringbuilder* sb, float f)
{
  char cs[24];
  sprintf(cs,"%f",f);
  return stringbuilder_append_str(sb,cs);
}

/* to-string functions */
char* str_stringbuilder(stringbuilder* sb, char* cbuf)
{
	node1 n0;
	n0.next = 0;
	
  int i,ind,xi;
  node1* n;
  char* cs;
  cbuf = (cbuf ? cbuf : malloc(sizeof(char)*(sb->length+1)));
  cbuf[sb->length] = 0;
  if (sb->length == 0)
    return cbuf;
  
  ind = 0;
  n = sb->cbufs->first;
  while (n && n != sb->cbufs->last)
  {
    cs = n->data;
    for (i = 0; i < sb->xlen; ++i)
      cbuf[ind++] = cs[i];
    
    n = n->next;
  }
  if (n == sb->cbufs->last) /* Should be the last node here */
  {
    cs = n->data;
    for (i = 0; ind < sb->length; ++i)
      cbuf[ind++] = cs[i];
  }
  else assert(0);
  
  return cbuf;
}

