#ifndef tableworks_h
#define tableworks_h

#include "listworks.h"


typedef unsigned int(*hash_func)(void*);
typedef unsigned int(*hash_func2)(void*,void*);

unsigned int hash_address(void*);
unsigned int hash_string(void*);


// (I suppose this might be declared elsewhere, too...)
typedef void(*del_func)(void*,bit_flag);


// (putting these here...)

typedef struct
{
	int size;
	int arrsize;
	
	hash_func hash;
	void** arr;
	
	void(*delkey)(void*,bit_flag);
	void(*delval)(void*,bit_flag);
	
	bit_flag delkeyflags;
	bit_flag delvalflags;
} table;

typedef struct _table_iter table_iter;


// (not yet implemented)
// table iterator api {
table_iter* alloc_table_iter();
table_iter* init_table_iter();
table_iter* new_table_iter();

void del_table_iter(table_iter* it, bit_flag del_flags);

table* table_iter_target(table_iter* it);
int table_iter_has_next(table_iter* it);
int table_iter_next(table_iter* it, void* key, void* val);
//}


// table api {
table* alloc_table();
table* init_table(table* t, hash_func f, int size);
table* new_table(hash_func f, int size);

void del_table(table* t, bit_flag del_flags);
void table_set_del_key(table* t, del_func del, bit_flag del_flags);
void table_set_del_val(table* t, del_func del, bit_flag del_flags);
void table_get_del_key(table* t, del_func* del, bit_flag* del_flags);
void table_get_del_val(table* t, del_func* del, bit_flag* del_flags);

int table_size(table* t);

void* table_set(table* t, void* k, void* v);
void* table_remove(table* t, void* k);

void* table_get(table* t, void* k);
void** table_keys(table* t, void** keys, int* len);
void** table_values(table* t, void** values, int* len);

// This could use a key:val iterator
void* table_iter_key_val(table* t);
//}

#endif

