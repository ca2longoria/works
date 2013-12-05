#ifndef fileworks_h
#define fileworks_h

int filesize(FILE* fp)
{
  int prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp,prev,SEEK_SET);
  return sz;
}

#endif

