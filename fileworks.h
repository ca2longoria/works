#ifndef fileworks_h
#define fileworks_h

int filesize(FILE* fp);

char* readline(FILE* file, char* cbufptr);

char* readline_new(FILE* file, char** cbufptr);

#endif

