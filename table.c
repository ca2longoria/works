
#include <stdlib.h>
#include "tableworks.h"




// Initialization {
table* alloc_table()
{
	return (table*)malloc(sizeof(table));
}

table* init_table(table* t, hash_func f, int size)
{
	t->size = 0;
	t->arrsize = size;
	t->hash = f;
	t->arr = (void**)malloc(sizeof(void*)*size);
	t->delkey = 0;
	t->delval = 0;
	return t;
}

table* new_table(hash_func f, int size)
{
	return init_table(alloc_table(),f,size);
}
//}


// Deletion {
void del_table(table* t, bit_flag del_flags)
{
	int flags = (del_flags ? del_flags : (DEL_STRUCT));
	
	if (flags & DEL_KEYS)
	{
		void* key;
		if (t->delkey)
			// iterate through keys
			t->delkey(key,t->delkeyflags);
	}
	
	if (flags & DEL_VALS)
	{
		void* val;
		if (t->delkey)
			// iterate through vals
			t->delval(val,t->delvalflags);
	}
	
	int i;
	for (i=0; i < t->arrsize; ++i)
		if (t->arr[i])
			del_arrlist(t->arr[i],DEL_STRUCT);
	free(t->arr);
	
	if (flags & DEL_STRUCT)
		free(t);
}

void table_set_del_key(table* t, del_func del, bit_flag del_flags)
{
	t->delkey = del;
	t->delkeyflags = del_flags;
}

void table_set_del_val(table* t, del_func del, bit_flag del_flags)
{
	t->delval = del;
	t->delvalflags = del_flags;
}

void table_get_del_key(table* t, del_func* del, bit_flag* del_flags)
{
	if (del) del[0] = t->delkey;
	if (del_flags) del_flags[0] = t->delkeyflags;
}

void table_get_del_val(table* t, del_func* del, bit_flag* del_flags)
{
	if (del) del[0] = t->delval;
	if (del_flags) del_flags[0] = t->delvalflags;
}
//}


// Content {
int table_size(table* t)
{
	return t->size;
}

// (what should this return, really?  The key, the value, the table?)
void* table_set(table* t, void* k, void* v)
{
	unsigned int hash = t->hash(k);
	int arrindex = hash % t->arrsize;
	
	if (!t->arr[arrindex])
		t->arr[arrindex] = new_arrlist(1);
	arrlist* arrl = t->arr[arrindex];
	
	// (directly casting from uint to void*... Is this safe?)
	int index = arrlist_index(arrl,(void*)(long int)hash);
	if (index < 0)
	{
		arrlist_add(arrl,(void*)(long int)hash); // + 0
		arrlist_add(arrl,k); // + 1
		arrlist_add(arrl,v); // + 2
		t->size++;
	}
	else
	{
		// WARNING: What handles value deletion on value replacement?
		//arrlist_set(arrl,(index*3)+2,v);
		arrlist_set(arrl,index+2,v);
	}
	
	return v;
}


void* table_get(table* t, void* k)
{
	unsigned int hash = t->hash(k);
	int arrindex = hash % t->arrsize;
	
	if (!t->arr[arrindex])
		return 0;
	arrlist* arrl = t->arr[arrindex];
	
	int index = arrlist_index(arrl,(void*)(long int)hash);
	if (index < 0)
		return 0;
	return arrlist_get(arrl,index+2);
}

void** table_keys(table* t, void** key_buffer, int* len)
{
	// If key_buffer == 0, keys = malloc'd
	// If len == 0, a zero is appended to the end of keys
	
	void** keys = (key_buffer ? key_buffer :
		(void**)malloc(sizeof(void*)*(t->size + (len ? 0:1))));
	
	int i,j;
	int length = 0;
	arrlist* arrl;
	for (i=0; i < t->arrsize; ++i)
	{
		if (!t->arr[i])
			continue;
		
		arrl = t->arr[i];
		for (j=1; j < arrlist_size(arrl); j += 3)
			keys[length++] = arrlist_get(arrl,j);
	}
	
	if (!len)
		keys[length] = 0;
	*len = length;
	
	return keys;
}

void** table_values(table* t, void** val_buffer, int* len)
{
	// If val_buffer == 0, vals = malloc'd
	// If len == 0, a zero is appended to the end of vals
	
	void** vals = (val_buffer ? val_buffer :
		(void**)malloc(sizeof(void*)*(t->size + (len ? 0:1))));
	
	int i,j;
	int length = 0;
	arrlist* arrl;
	for (i=0; i < t->arrsize; ++i)
	{
		if (!t->arr[i])
			continue;
		
		arrl = t->arr[i];
		for (j=2; j < arrlist_size(arrl); j += 3)
			vals[length++] = arrlist_get(arrl,j);
	}
	
	if (!len)
		vals[length] = 0;
	*len = length;
	
	return vals;
}
//}


unsigned int hash_address(void* ob)
{
	return (unsigned int)(long)ob;
}

unsigned int hash_string(void* ob)
{
	char* s = (char*)ob;
	if (!s || !s[0])
		return 0;
	
	// (Just quickly throwing this together...  Will it work...?)
	int i;
	char c = 0;
	unsigned long int a = 1;
	unsigned long int max = (unsigned long int)~(int)0;
	for (i=1; s[i]; ++i)
	{
		a += s[i] * s[i-1] * a - i;
		if (a >= max)
			a = (a & max);
	}
	
	return (unsigned int)a;
}



//#define table_active
//#define mtrace_active
#define table_mode 2

// Testing {
#ifdef table_active
#include <stdio.h>
#include <mcheck.h>

char* stringit(table* t, char* cbuf, int brief)
{
	int i,k;
	int size;
	int j = 0;
	for (i=0; i < t->arrsize; ++i)
	{
		j += sprintf(&cbuf[j],"%2i-(%x;%2i):  ",i,t->arr[i],
			(t->arr[i] ? arrlist_size(t->arr[i])/3 : 0));
		
		if (t->arr[i])
			for (k=0; k < arrlist_size(t->arr[i])/3; ++k)
			{
				if (brief)
					j += sprintf(&cbuf[j],"*");
				else
				{
					j += sprintf(&cbuf[j],"\n    <%s : ",arrlist_get(t->arr[i],k*3+1));
					j += sprintf(&cbuf[j],"%s>",arrlist_get(t->arr[i],k*3+2));
				}
			}
		
		j += sprintf(&cbuf[j],(brief ? "\n" : "\n\n"));
	}
	printf("j: %i\n",j);
	return cbuf;
}

int main(int argc, char** argv)
{
#ifdef mtrace_active
	
	mtrace();
	
	printf("sizeof:\ntable: %i\narrlist: %i\n",sizeof(table),sizeof_arrlist());
	
	table* t = new_table(hash_string,13);	
	int i;
	for (i=0; i < argc; ++i)
		table_set(t,argv[i],argv[i]);
	del_table(t,0);
	
	arrlist* a = new_arrlist(10);
	arrlist_add(a,"poof");
	arrlist_add(a,"loop");
	del_arrlist(a,DEL_STRUCT);
	
	muntrace();
	return 0;
	
#elif table_mode == 1
	
	mtrace();
	
	table* t = new_table(hash_string,143);
	
	int i;
	for (i=0; i < argc; ++i)
		table_set(t,argv[i],argv[i]);
	
	printf("table size: %i\n",table_size(t));
	
	char s[12000];
	stringit(t,s,1);
	printf(s);
	
	del_table(t,0);
	
	muntrace();
	return 0;
	
#elif table_mode == 2
	
	mtrace();
	
	char* kv[] = {"Tom","Bombadil", "Lork","Spork", "particular","particles",
		"Nordic","soothayin", "Pepto","Dismal", "Sprakdak","MaDoodle",
		"Distopian","Nomenclature", "Apologetic","Smelter", "Nynamic","Obligation",
	0};
	
	int size;
	for (size=0; kv[size]; ++size);
	
	// verify alloc+init
	table* t = new_table(hash_string,7);
	
	// verify set
	int i;
	for (i=0; i < size; i+=2)
		table_set(t,kv[i],kv[i+1]);
	
	// display it prettily (assumes string:string key value pairing)
	char s[12000];
	stringit(t,s,0);
	printf(s);
	
	// verify get
	for(i=0; i < size; i+=2)
		printf("item %i: %s, %s\n",(i>>1),kv[i],table_get(t,kv[i]));
	
	// verify keys
	int len;
	void** keys = table_keys(t,0,&len);
	printf("keys: %i:",len);
	for (i=0; i < len; ++i)
		printf(" %s",keys[i]);
	printf("\n");
	free(keys);
	
	// verify values
	void** vals = table_values(t,0,&len);
	printf("vals: %i:",len);
	for (i=0; i < len; ++i)
		printf(" %s",vals[i]);
	printf("\n");
	free(vals);
	
	
	del_table(t,0);
	
	muntrace();
	return 0;
	
#endif
}
#endif
//}

