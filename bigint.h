#ifndef bigint_h
#define bigint_h

#include <numdefs.h>

#define BIGINT_MAX_VALUE_LENGTH 256
#define BIGINT_DEFAULT_VALUE_LENGTH 8
#define BIGINT_UINT_MAX 4294967295

uint _BigInt_buffer[BIGINT_MAX_VALUE_LENGTH];

struct _BigInt
{
  uint* val;
  uint negative;
  uint val_length;
};

typedef struct _BigInt BigInt;

BigInt* init_BigInt(BigInt* bint)
{
  int i;
  bint->val = malloc(sizeof(uint)*BIGINT_DEFAULT_VALUE_LENGTH);
  for (i = 0; i < BIGINT_DEFAULT_VALUE_LENGTH; ++i)
    bint->val[i] = 0;
  
  bint->val[0] = 0;
  bint->negative = 0;
  bint->val_length = 1;
  return bint;
};

void del_BigInt(BigInt* bint)
{
  free(bint->val);
  free(bint);
};

char* str_BigInt(BigInt* bint, char* cbuf)
{
  if (!cbuf)
    cbuf = malloc(sizeof(char)*(bint->val_length+1));
  sprintf(cbuf, "<BigInt:%u>", bint->val[0]);
  return cbuf;
};


BigInt* BigIntGetInt(BigInt* bint, int val)
{
  bint->val_length = 1;
  bint->val[0] = val;
  return bint;
}

BigInt* BigIntGet(BigInt* a)
{
  
}

BigInt* BigIntAdd(BigInt* a, BigInt* b, BigInt* c)
{
  ullong carry;
  ullong bval;
  ullong cval;
  ullong sum;
  
  int i = 0;
  int len = (b->val_length < c->val_length ? b->val_length : c-> val_length);
  
  carry = 0;
  for (; i <= len; ++i) /* equals in <= for that last possible 1+0+0 carry */
  {
    bval = b->val[i];
    cval = c->val[i];
    sum = bval + cval + carry;
    if (sum >= BIGINT_UINT_MAX)
    {
      carry = 1;
      sum -= BIGINT_UINT_MAX;
    }
    else
      carry = 0;
    a->val[i] = sum;
  }
  
  return a;
}

BigInt* BigIntSub(BigInt* a, BigInt* b, BigInt* c)
{

}

BigInt* BigIntMul(BigInt* a, BigInt* b, BigInt* c)
{

}

BigInt* BigIntDiv(BigInt* a, BigInt* b, BigInt* c)
{

}


#endif

