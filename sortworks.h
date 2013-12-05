#ifndef sortworks_h
#define sortworks_h

#include <stdio.h>
#include <stdlib.h>


typedef int(*compfunc)(void*,void*);

int comp_int(void*,void*);
int comp_float(void*,void*);
int comp_string(void*,void*);


int linear_search(void**,int,void*,compfunc);
int linear_search_reverse(void**,int,void*,compfunc);
int binary_search(void**,int,void*,compfunc);


void** merge_sort(void**,int,compfunc);
void** merge_sort_rec(void**,void**,int,int,compfunc);


#endif

