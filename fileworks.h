#ifndef fileworks_h
#define fileworks_h

int filesize(FILE* fp);

char* readline(FILE* file, char* cbufptr);

char* readline_new(FILE* file, char** cbufptr);


typedef struct {
	char* s;
	int length;
	int line;
	int offset;
} readlines_param;

typedef void(*readline_callback2)(void*,void*);
char* readlines_callback2(FILE*,char*,int,readline_callback2,void*);

#endif

