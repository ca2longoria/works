
#include <stdlib.h>

#include "listworks.h"


// Helper function pre-definitions {
void _del_data_arrlist(arrlist* arr);
void _arrlist_expand_data(arrlist* arr);
//}


// arrlist functions {
arrlist* alloc_arrlist()
{
	return (arrlist*)malloc(sizeof(arrlist));
}

arrlist* init_arrlist(arrlist* arr, int size)
{
	arr->size = 0;
	arr->truesize = size;
	arr->multiplier = 1.5;
	arr->data = (void**)malloc(sizeof(void*)*size);
	
	return arr;
}

arrlist* new_arrlist(int size)
{
	return init_arrlist(alloc_arrlist(), size);
}

void del_arrlist(arrlist* arr, bit_flag del_flags)
{
	int flags;
	flags = (del_flags ? del_flags : DEL_STRUCT);
	
	if (flags & DEL_DATA)
		_del_data_arrlist(arr);
	
	// Obligatory deletion
	free(arr->data);
	
	if (flags & DEL_STRUCT)
		free(arr);
}

int sizeof_arrlist()
{
	return sizeof(arrlist);
}


int arrlist_size(arrlist* arr)
{
	return arr->size;
}


void* arrlist_add(arrlist* arr, void* ob)
{
	if (arr->size >= arr->truesize)
		_arrlist_expand_data(arr);
	
	arr->data[arr->size++] = ob;
	return ob;
}

void* arrlist_set(arrlist* arr, int index, void* ob)
{
	arr->data[index] = ob;
	return ob;
}

void* arrlist_insert(arrlist* arr, int index, void* ob)
{
	if (arr->size >= arr->truesize)
		_arrlist_expand_data(arr);
	
	int i;
	for (i=arr->size; i>index; --i)
		arr->data[i] = arr->data[i-1];
	arr->data[index] = ob;
	arr->size += 1;
	return ob;
}

void* arrlist_remove(arrlist* arr, int index)
{
	int i;
	void* ret = arr->data[index];
	for (i = index; i < arr->size; ++i)
		arr->data[i] = arr->data[i+1];
	arr->size -= 1;
	return ret;
}

void* arrlist_remove_object(arrlist* arr, void* ob)
{
	return arrlist_remove(arr, arrlist_index(arr,ob));
}


void* arrlist_get(arrlist* arr, int index)
{
	return arr->data[index];
}

void* arrlist_first(arrlist* arr)
{
	return (arr->size > 0 ? arr->data[0] : 0);
}

void* arrlist_last(arrlist* arr)
{
	return (arr->size > 0 ? arr->data[arr->size-1] : 0);
}


int arrlist_index(arrlist* arr, void* ob)
{
	int i;
	for (i=0; i < arr->size; ++i)
		if (arr->data[i] == ob)
			return i;
	return -1;
}

int arrlist_index_comp(arrlist* arr, void* ob, comp_func cmp)
{
	int i;
	for (i=0; i < arr->size; ++i)
		if (cmp(arr->data[i],ob) == 0)
			return i;
	return -1;
}


ilist* ilist_arrlist(ilist* il)
{
	// (I'm not really sure what will happen with casting
	//  that first arrlist* to void* here...)
	il->size = (int(*)(void*))arrlist_size;
	il->add = (void*(*)(void*,void*))arrlist_add;
	il->insert = (void*(*)(void*,int,void*))arrlist_insert;
	il->remove = (void*(*)(void*,int))arrlist_remove;
	il->remove_object = (void*(*)(void*,void*))arrlist_remove_object;
	il->get = (void*(*)(void*,int))arrlist_get;
	il->first = (void*(*)(void*))arrlist_first;
	il->last = (void*(*)(void*))arrlist_last;
	il->index = (int(*)(void*,void*))arrlist_index;
	il->index_comp = (int(*)(void*,void*,comp_func))arrlist_index_comp;
	return il;
}
//}


// Helper function definitions {
void _del_data_arrlist(arrlist* arr)
{
	int i;
	for (i=0; i<arr->size; ++i)
		if (arr->data[i])
			free(arr->data[i]);
}
void _arrlist_expand_data(arrlist* arr)
{
	int newsize;
	void** newdata;
	
	newsize = arr->truesize * arr->multiplier;
	if (newsize < 4)
		newsize = 2*arr->truesize;
	
	newdata = (void**)malloc(sizeof(void*)*newsize);
	
	int i;
	for (i=0; i < arr->size; ++i)
		newdata[i] = arr->data[i];
	
	free(arr->data);
	arr->data = newdata;
	
	arr->truesize = newsize;
}
//}


// Testing arrlist {
#ifdef arrlist_active
#include <string.h>
#include <stdio.h>

int comp_string(void* a, void* b){ return strcmp(a,b);}

int main(int argv, char** args)
{
	char* ss[] = {"Boop","Brap","Zpow","Nglap",0};
	
	arrlist* a = new_arrlist(1);
	
	int i;
	for (i=0; ss[i]; ++i)
		arrlist_add(a,ss[i]);
	
	for (i=0; i < arrlist_size(a); ++i)
		printf("%s\n", arrlist_get(a,i));
	
	printf("%s vs %s\n",ss[0],arrlist_first(a));
	printf("%s vs %s\n",ss[3],arrlist_last(a));
	
	printf("size,truesize: %i,%i\n",arrlist_size(a),a->truesize);
	
	printf("index of %s is %i\n",ss[2],arrlist_index(a,ss[2]));
	arrlist_insert(a,2,ss[3]);
	printf("index of %s is %i\n",ss[2],arrlist_index_comp(a,ss[2],comp_string));
	printf("index of %s is %i\n",ss[3],arrlist_index_comp(a,ss[3],comp_string));
	arrlist_remove(a,4);
	
	printf("and now...\n");
	for (i=0; i < arrlist_size(a); ++i)
		printf("%s\n", arrlist_get(a,i));
	printf("size,truesize: %i,%i\n",arrlist_size(a),a->truesize);
	
	del_arrlist(a,DEL_STRUCT);
	
	return 0;
}
#endif
//}

