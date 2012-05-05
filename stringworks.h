#ifndef stringworks_h
#define stringworks_h

#include <numdefs.h>
#include <mathworks.h>
#include <listworks.h>
#include <string.h>
#include <assert.h>


/*{ string functions*/
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

struct _stringbuilder
{
  nodelist* cbufs; /* Okay, going to make this a nodelist of char* */
  int xind;
  int xlen;
  int length;
};
typedef struct _stringbuilder stringbuilder;

#define stringbuilder_default_xlength 64

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
  del_nodelist(sb->cbufs,1);
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
  int i,ind,xi;
  node* n;
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



typedef char*(*strfunc)(char*,void*);



/* A Boyer-Moore attempt */

/*
  Returns the index of given substring within s, assuming that the first character of the
  substring is in fact a not-char, as in the first matching character simply must *not* be sub[0]
*/
int checkNotSubstring(char* s, char* sub)
{
  int slen,sublen;
  char nc;
  int ind,i,j;
  
  slen = strlen(s);
  sublen = strlen(sub);
  nc = sub[0];
  
  /* Assertions:
     - Obviously, nc mustn't be the null character, for whatever reason
     - Substring must not be the empty string
     - Substring must not be longer than the string itself
  */
  assert(nc);
  assert(sublen > 0);
  assert(sublen <= slen);
  
  /* This will increment up the returning table array */
  ind = 0;
  
  /* The loop will continue into negative indexes, so that the substring may match at least an ending character or two */
  for(i = slen-sublen-1; i >= -sublen; --i)
  {
    /* As long as i >= 0, the not-character can be checked against the curring s[i] */
    if (i >= 0 && s[i] == nc)
      continue;
    
    /* This loop continues until a character between s[i+j] and sub[j] does not match */
    for (j=1; j<sublen; ++j)
    {
      /* Can't check strings in s below the 0-index, so just continue until i+j hits a valid index */
      if (i+j < 0)
        continue;
      
      /* printf("checking s[%i] against sub[%i]: %c vs %c\n",i+j,j,s[i+j],sub[j]); */
      /* The character comparison */
      if (s[i+j] != sub[j])
        break;
    }
    
    /* If the previoius loop reached its end, then we are go!  A match! */
    if (j == sublen)
    {
      /* printf("match! %i\n", i); */
      return i;
    }
  }
  
  return -sublen;
}

/*
  Builds the first table, which works off backward-checking substrings in substrings
*/
int* boyerMooreTableOne(char* s)
{
  int* arr;
  int slen,sublen;
  int i,ind,sind;
  
  /* Create an array the length of s */
  arr = malloc(sizeof(int)*strlen(s));
  
  /* Fill the array */
  arr[0] = 1;
  
  slen = strlen(s);
  
  ind = 1;
  
  for (i = slen-2; i >= 0; --i)
  {
    /* printf("checking notstring: %s\n", &s[i]); */
    sind = checkNotSubstring(s, &s[i]);
    sublen = slen-i;
    if (sind <= -sublen)
    {
      arr[ind] = slen;
      /* printf("notstring not found\n"); */
    }
    else
    {
      arr[ind] = slen - (sind+sublen);
      /* printf("notstring found at index %i!\n", arr[ind]); */
    }
    ++ind;
  }
  
  /*
  for (i = 0; i < slen; ++i)
    printf("arr[%i]: %i\n", i, arr[i]);
  */
  
  return arr;
}

/*
  Builds the second table, which is a simple min-character shift based off given character
*/
int* boyerMooreTableTwo(char* s)
{
  int* arr;
  int* mp;
  int* endp;
  int slen,i;
  char c;
  
  slen = strlen(s);
  
  assert(slen > 0);
  
  /* This block of code inits the table array, filling it with 0's */ 
  arr = malloc(sizeof(int)*255);
  mp = arr;
  endp = &arr[256];
  for (mp = arr; mp != endp; ++mp)
    *mp = 0;
  
  /* Run backwards through s, setting the appropriate jump value only on the first encounter of each new character */
  for (i = slen-1; i >= 0; --i)
  {
    c = s[i];
    if (arr[c])
      continue;
    arr[(uint)c] = slen-i-1;
  }
  
  return arr;
}


/*
  The search itself, utilizing both tables, and jumping ahead by the max value (for the given character) from each
*/
int boyerMooreSearch(char* s, char* sub)
{
  int* tone;
  int* ttwo;
  int slen,sublen;
  int i,j,jump1,jump2;
  int index;
  char c;
  
  slen = strlen(s);
  sublen = strlen(sub);
  
  assert(sublen > 0);
  assert(sublen <= slen);
  
  tone = boyerMooreTableOne(sub);
  ttwo = boyerMooreTableTwo(sub);
  
  index = -1;
  i = sublen-1;
  while (i < slen)
  {
    /*printf("i: %i\n", i);*/
    c = s[i];
    
    jump1 = 0;
    
    /* Utilization of table two */
    jump2 = ttwo[(uint)c];
    /*printf("jump2 for c(%c): %i\n", c,jump2);*/
    
    if (jump2 || c == sub[sublen-1])
    {
      /* Utilization of table one */
      for (j = 0; j < sublen; ++j)
      {
        /*printf("comparing (%c) to (%c)\n", s[i-j], sub[sublen-j-1]);*/
        if (s[i-j] != sub[sublen-j-1])
        {
          /* A non-matching character, so pull from table one */
          jump1 = tone[j];
          /*printf("jump1 gets value: %i\n", jump1);*/
          break;
        }
      }
    }
    else
      jump2 = sublen;
 
    if (j == sublen)
    {
      /* Wait, what!  A match!  We've got a match! */
      index = i-sublen+1;
      break;
    }
    
    /*printf("jump1 vs jump2: %i vs %i\n", jump1, jump2);*/
    i += (jump1 > jump2 ? jump1 : jump2);
  }
  
  free(tone);
  free(ttwo);
  
  return index;
}

/*
  Searches for the last instance of the given substring in string
*/
int boyerMooreSearchReverse(char* s, char* sub)
{
  char* s2;
  char* sub2;
  char c;
  int i,j;
  int slen = strlen(s);
  int sublen = strlen(sub);
  
  s2 = malloc(sizeof(char) * (slen + 1));
  sub2 = malloc(sizeof(char) * (sublen + 1));
  
  for (i = 0; i < slen; ++i)
    s2[i] = s[slen-i-1];
  
  for (i = 0; i < sublen; ++i)
    sub2[i] = sub[sublen-i-1];
  
  i = boyerMooreSearch(s2, sub2);
  
  free(s2);
  free(sub2);
  
  return (i >= 0 ? (slen-i-(sublen-1)-1) : i);
}


#endif

