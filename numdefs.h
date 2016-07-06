#ifndef numdefs_h
#define numdefs_h

// It appears a few of these are common under /usr/include/sys/types.h
// as old campatibility names for C types.

// Not for Windows machines, though.  And since I've finally begun using this
// under a MinGW build environment, as well, it's time to include them.

#if defined(WIN32) || defined(WIN64) || defined(__MINGW32__) || defined(__MINGW64__)
#ifndef uint
#define uint unsigned int
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#endif

#ifndef llong
#define llong long long
#endif

#ifndef ullong
#define ullong unsigned long long
#endif

#endif

