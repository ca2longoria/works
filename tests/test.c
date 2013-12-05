
#include <stdlib.h>
#include <stdio.h>

#include "../stringworks.h"
#include "../fileworks.h"
#include "../listworks.h"

int main(int argv, char** args)
{
	char* fname;
	fname = "test.c";
	if (argv > 1)
		fname = args[1];
	
	FILE* f;
	f = fopen(fname,"r");

	printf("%s filesize: %i\n",fname,filesize(f));
	
	arrlist* a = new_arrlist(1);
	del_arrlist(a,DEL_STRUCT);
	
	return 0;
}

