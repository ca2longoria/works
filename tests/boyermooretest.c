
#include <stdio.h>
#include <stdlib.h>

#include <cworks/stringworks.h>

int main(int argc, char** argv)
{
	char* s = "This is a string which's searching can be done.";
	char* sub = "is a";
	
	int pos1 = boyerMooreSearch(s,sub);
	int pos2 = boyerMooreSearchReverse(s,"is a");
	
	printf("From string: %s\nFinding: %s\nPositions: %i, %i\n",s,sub,pos1,pos2);
	
	return 0;
}

