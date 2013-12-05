
#include <stdio.h>

#include <stdlib.h>
#include "listworks.h"


// Helper function prototypes {
void _del_nodes_nodelist(nodelist* nl, bit_flag del_flags);
node1* _nodelist_get_node(nodelist* nl, int index);
//}


// nodelist functions {
// object initialization/deletion {
nodelist* alloc_nodelist()
{
	return (nodelist*)malloc(sizeof(nodelist));
}

nodelist* init_nodelist(nodelist* nl)
{
	nl->size = 0;
	nl->first = 0;
	nl->last = 0;
}

nodelist* new_nodelist()
{
	return init_nodelist(alloc_nodelist());
}


void del_nodelist(nodelist* nl, bit_flag del_flags)
{
	int flags = (del_flags ? del_flags : (DEL_STRUCT));
	
	_del_nodes_nodelist(nl,flags);
}
//}


int nodelist_size(nodelist* nl)
{
	return nl->size;
}


// object insertion/removal {
void* nodelist_add(nodelist* nl, void* ob)
{
	node1* n = (node1*)malloc(sizeof(node1));
	n->data = ob;
	
	if (nl->size == 0)
		nl->first = n;
	else
		nl->last->next = n;
	nl->last = n;
	nl->size++;
}

void* nodelist_set(nodelist* nl, int index, void* ob)
{
	node1* n = _nodelist_get_node(nl,index);
	n->data = ob;
	return ob;
}

void* nodelist_insert(nodelist* nl, int index, void* ob)
{
	if (index < 0 || index > nl->size)
		return 0;
		
	node1* n = (node1*)malloc(sizeof(node1));
	n->data = ob;
	
	if (nl->size == index)
	{
		if (nl->last)
			nl->last->next = n;
		nl->last = n;
	}
	
	if (index == 0)
	{
		n->next = nl->first;
		nl->first = n;
	}
	else
	{
		node1* prev = _nodelist_get_node(nl,index-1);
		n->next = prev->next;
		if (prev->next)
			prev->next = n;
	}
	nl->size++;
	return ob;
}

void* nodelist_remove(nodelist* nl, int index)
{
	if (index < 0 || index >= nl->size || nl->size == 0)
		return 0;
	
	// Okay, cases here...
	// - size=0 : makes no sense
	// - size=1 : first and last must be zero-d
	// - index=0 : first must be modified
	// - index=size-1 : last must be modified
	
	node1* n;
	node1* n2;
	void* ret;
	
	if (nl->size == 1)
	{
		ret = nl->first->data;
		free(nl->first);
		nl->first = 0;
		nl->last = 0;
	}	
	else if (index == 0)
	{
		ret = nl->first->data;
		n = nl->first->next;
		free(nl->first);
		nl->first = n;
	}
	else
	{
		n = _nodelist_get_node(nl,index-1);
		ret = ((node1*)n->next)->data;
		n2 = ((node1*)n->next)->next;
		free(n->next);
		n->next = n2;
	}
	nl->size--;
	return ret;
}

void* nodelist_remove_object(nodelist* nl, void* ob)
{
	return nodelist_remove(nl,nodelist_index(nl,ob));
}
//}


// object retreival {
void* nodelist_get(nodelist* nl, int index)
{
	if (nl->size <= 0)
		return 0;	
	return _nodelist_get_node(nl,index)->data;
}

void* nodelist_first(nodelist* nl)
{
	if (!nl->first) return 0;
	return nl->first->data;
}

void* nodelist_last(nodelist* nl)
{
	if (!nl->last) return 0;
	return nl->last->data;
}


int nodelist_index(nodelist* nl, void* ob)
{
	if (nl->size == 0)
		return -1;
	
	int i;
	node1* n = nl->first;
	for (i=0; i < nl->size; ++i)
		if (n->data == ob)
			return i;
		else
			n = n->next;
	return -1;
}

int nodelist_index_comp(nodelist* nl, void* ob, comp_func cmp)
{
	if (nl->size == 0)
		return -1;
	
	int i;
	node1* n = nl->first;
	for (i=0; i < nl->size; ++i)
		if (cmp(n->data,ob) == 0)
			return i;
		else
			n = n->next;
	return -1;
}
//}


ilist* ilist_nodelist(ilist* il)
{
	// (I'm not really sure what will happen with casting
	//  that first nodelist* to void* here...)
	il->size = (int(*)(void*))nodelist_size;
	il->add = (void*(*)(void*,void*))nodelist_add;
	il->insert = (void*(*)(void*,int,void*))nodelist_insert;
	il->remove = (void*(*)(void*,int))nodelist_remove;
	il->remove_object = (void*(*)(void*,void*))nodelist_remove_object;
	il->get = (void*(*)(void*,int))nodelist_get;
	il->first = (void*(*)(void*))nodelist_first;
	il->last = (void*(*)(void*))nodelist_last;
	il->index = (int(*)(void*,void*))nodelist_index;
	il->index_comp = (int(*)(void*,void*,comp_func))nodelist_index_comp;
	return il;
}
//}


// Helper function definitions {
void _del_nodes_nodelist(nodelist* nl, bit_flag del_flags)
{
	if (nl->size <= 0 || !nl->first || !nl->last)
		return;
	
	int i;
	node1* tn;
	node1* n = nl->first;
	for (i=0; i < nl->size; ++i)
	{
		tn = n->next;
		if (del_flags & DEL_DATA)
			free(n->data);
		free(n);
		n = tn;
	}
	
	if (del_flags & DEL_STRUCT)
		free(nl);
}
node1* _nodelist_get_node(nodelist* nl, int index)
{
	node1* n = nl->first;
	int i;
	for (i=0; i < index; ++i)
		n = n->next;
	return n;
}
//}



// Test it out a bit {
#ifdef nodelist_active
#include <stdio.h>

int main(int argv, char** args)
{
	printf("Going in...\n");
	ilist il;
	
	ilist_nodelist(&il);
	
	nodelist* nl = new_nodelist();
	
	nodelist_add(nl,"Doop");
	nodelist_add(nl,"Soup");
	nodelist_add(nl,"Troop");
	il.add(nl,"Zoop");
	
	int i;
	for (i=0; i < il.size(nl); ++i)
		printf("a %s\n",il.get(nl,i));
	
	printf("Inserting at 0,x,n:\n");
	
	il.insert(nl,0,"insert-0");
	il.insert(nl,2,"insert-2");
	il.insert(nl,il.size(nl),"insert-n");
	
	for (i=0; i < il.size(nl); ++i)
		printf("i %s\n",il.get(nl,i));
	
	del_nodelist(nl,DEL_STRUCT);
	
	printf("And out\n");
	return 0;
}
#endif
//}


