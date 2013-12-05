
#include <assert.h>
#include <string.h>
#include <sys/types.h>

#include "numdefs.h"
#include "stringworks.h"


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
  
	/* Boo, this guy gives seg faults somewhere... */
	
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

