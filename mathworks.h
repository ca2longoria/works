#ifndef mathworks_h
#define mathworks_h

#include <numdefs.h>

#define MODPOW_SHIFT_LIMIT 32

uint modpow(int a, int p, int m)
{
  int pshift = p;
  int val = 1;
  
  uint x = a % m;
  uint pcheck = 1;
  
  int i;
  for (i = 0; i < MODPOW_SHIFT_LIMIT && pcheck <= p; ++i)
  {
    /*printf("pcheck: %i, x: %i\n",pcheck,x);*/
    if (pshift&1)
    {
      /*printf("p of %i is good\n",pcheck);*/
      val *= x;
      val %= m;
      /*printf("val is now: %u\n",val);*/
    }
    x *= x;
    /*printf("then x is: %i\n",x);*/
    x %= m;
    /*printf("x ends as: %i\n\n",x);*/
    pshift >>= 1;
    pcheck <<= 1;
  }
  
  return val;
}

#endif

