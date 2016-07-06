#ifndef stringworks_h
#define stringworks_h

//#include <string.h>

#include "listworks.h"


/*{ string functions*/
int strcharindex(char* s, char c, int reverse);
char* strmulticat(char* s, ...);
/*}*/


/* A stringbuilder for the masses! */

//typedef struct _stringbuilder stringbuilder;
typedef struct
{
  nodelist* cbufs;
  int xind;
  int xlen;
  int length;
} stringbuilder;

#define stringbuilder_default_xlength 64

stringbuilder* alloc_stringbuilder();
stringbuilder* init_stringbuilder(stringbuilder* sb, int buflen);
stringbuilder* new_stringbuilder(int buflen);

void del_data_stringbuilder(stringbuilder* sb);
void del_stringbuilder(stringbuilder* sb);

stringbuilder* stringbuilder_append_str(stringbuilder* sb, char* s);
stringbuilder* stringbuilder_append_char(stringbuilder* sb, char c);
stringbuilder* stringbuilder_append_int(stringbuilder* sb, int i);
stringbuilder* stringbuilder_append_float(stringbuilder* sb, float f);

char* str_stringbuilder(stringbuilder* sb, char* cbuf);

typedef char*(*strfunc)(char*,void*);



// Boyer-Moore substring search algorithm

int boyerMooreSearch(char* s, char* sub);

// NOTE: The reversed search allocs new memory for a reversed array, making
//   this one at least one more N wasteful in space/time.  ... Not spacetime.
int boyerMooreSearchReverse(char* s, char* sub);


#endif

