#ifndef timeworks_h
#define timeworks_h

#include <sys/time.h>
#include <numdefs.h>


struct timeval usec_timeval;
struct timezone usec_timezone;

llong usectime()
{
  gettimeofday(&usec_timeval,&usec_timezone); 
  return ((llong)usec_timeval.tv_sec) * 1000000 + usec_timeval.tv_usec;
}

int sectime()
{
  gettimeofday(&usec_timeval,&usec_timezone);
  return usec_timeval.tv_sec;
}


#endif

