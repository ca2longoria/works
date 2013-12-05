
#include <stdlib.h>
#include <stdio.h>

#include "numdefs.h"
#include "listworks.h"
#include "sortworks.h"


bintree_node* alloc_node()
{
	return (bintree_node*)malloc(sizeof(bintree_node));
}
bintree_node* init_node(bintree_node* n, void* data)
{
	n->weight = 0;
	n->parent = 0;
	n->left = 0;
	n->right = 0;
	n->data = data;
	return n;
}
bintree_node* new_node(void* data)
{
	return init_node(alloc_node(),data);
}

// WARNING: DEL_DATA runs free() on n->data, no passed delfunc
void del_node(bintree_node* n, bit_flag del_flags)
{
	bit_flag flags = (del_flags ? del_flags : DEL_STRUCT);
	
	if (flags & DEL_DATA)
		free(n->data);
	
	if (flags & DEL_STRUCT)
		free(n);
}
void del_node_rec(bintree_node* n, bit_flag del_flags)
{
	if (!n) return;
	
	del_node_rec(n->left,del_flags);
	del_node_rec(n->right,del_flags);
	
	del_node(n,del_flags);
}

bintree_node* node_add_child(bintree_node* n, bintree_node* c, compfunc comp)
{
	int r = comp(c->data,n->data);
	
	// (assumes every entry is unique... should I do that?)
	if (!r) return 0;
	
	// (why did I do it like this?  Obfuscation for fun?  Obfuscate...  Obfuscate...)
	bintree_node** cp = (bintree_node**)(r < 0 ? &n->left : &n->right);
	if (*cp)
		node_add_child(*cp,c,comp);
	else
	{
		*cp = c;
		c->parent = n;
		// (we're going to assume a weight of 1, for now)
		node_propagate_weight(c,1);
	}
	
	return c;
}

bintree_node* node_get_child(bintree_node* n, void* data, compfunc comp)
{
	printf("node_get_child: %s,%s,%x\n",
		(char*)n->data,(char*)data,(uint)(llong)comp);
	
	int r = comp(data,n->data);
	if (r < 0)
	{
		if (!n->left) return 0;
		return node_get_child(n->left,data,comp);
	}
	else if (r > 0)
	{
		if (!n->right) return 0;
		return node_get_child(n->right,data,comp);
	}
	return n;
}

bintree_node* node_prev(bintree_node* n)
{
	if (n->left)
	{
		n = n->left;
		while (n->right)
			n = n->right;
		return n;
	}
	
	if (n->parent)
	{
		while (n->parent)
		{
			if (n == ((bintree_node*)n->parent)->right)
				return n->parent;
			n = n->parent;
		}
	}
	
	return 0;
}

bintree_node* node_next(bintree_node* n)
{
	if (n->right)
	{
		n = n->right;
		while (n->left)
			n = n->left;
		return n;
	}
	
	if (n->parent)
	{
		while (n->parent)
		{
			if (n == ((bintree_node*)n->parent)->left)
				return n->parent;
			n = n->parent;
		}
	}
	
	return 0;
}

int node_propagate_weight(bintree_node* n, int w)
{
	printf("node_propagate_weight: %s, %i\n",(char*)(n->data ? n->data : 0),w);
	int ret = n->weight;
	n->weight += w;
	printf("  new weight: %i\n",n->weight);
	if (n->parent)
		node_propagate_weight(n->parent,w);
		//ret += node_propagate_weight(n->parent,w);
	//return n->weight;
	return 0;
}




bintree* alloc_bintree()
{
	return (bintree*)malloc(sizeof(bintree));
}
bintree* init_bintree(bintree* b, compfunc comp)
{
	b->size = 0;
	b->root = 0;
	b->comp = comp;
}
bintree* new_bintree(compfunc comp)
{
	return init_bintree(alloc_bintree(),comp);
}

void del_bintree(bintree* b, bit_flag del_flags)
{
	bit_flag flags = (del_flags ? del_flags : DEL_STRUCT);
	
	del_node_rec(b->root,(flags | DEL_STRUCT));
	
	if (flags & DEL_STRUCT)
		free(b);
}

void* bintree_add(bintree* b, void* data)
{
	bintree_node* n = new_node(data);
	if (b->root)
		node_add_child(b->root,n,b->comp);
	else
		b->root = n;
	b->size++;
	return data;
}
void* bintree_contains(bintree* b, void* data)
{
	if (!b->root)
		return 0;
	bintree_node* n = node_get_child(b->root,data,b->comp);
	return (n ? n->data : 0);
}


// (if I do it like this, then iterators can be handled as, well, handles,
//  which would be then passed to an iter_NAME_next function.  But should
//  it take (T* ob, void* handle) as its arguments, or just (void* handle)?)
void* iter_bintree(bintree* b)
{
	bintree_node* n = b->root;
	while (n->left)
		n = n->left;
	return n;
}

void* iter_bintree_next(void** handle)
{
	// (n is the handle, and, incidentally, a node)
	if (!*handle)
		return 0;
	
	bintree_node* n = (bintree_node*)*handle;	
	void* data = n->data;
	*handle = node_next(n);
	return data;
}

int iter_bintree_has_next(void** handle)
{
	if (!*handle)
		return 0;
	return 1;
}




//#define bintree_active

#ifdef bintree_active
#include <stdio.h>
#include <string.h>

#ifdef mtrace_active
#include <mcheck.h>
#endif

int main(int argc, char** argv)
{
	printf("testing bintree\n");
	
	node na,nb,nc,nd;
	init_node(&na,"Anchor");
	init_node(&nb,"Bunk");
	init_node(&nc,"Corgi");
	init_node(&nd,"Dapper");
	
	node_add_child(&nb,&nd,comp_string);
	node_add_child(&nb,&nc,comp_string);
	node_add_child(&nb,&na,comp_string);
	
	printf("heythere\n");
	printf("%x: %x, %x\n",nb.data,nb.left,nb.right);
	printf("%s: %s, %s\n",nb.data,((bintree_node*)nb.left)->data,((bintree_node*)nb.right)->data);
	printf("%s: %s, %s\n",nd.data,((bintree_node*)nd.left)->data,(nd.right ? ((bintree_node*)nd.right)->data : 0));
	
	bintree_node* ns[] = {&na,&nb,&nc,&nd};
	int i;
	for (i=0; i<4; ++i)
	{
#define ndata(a,b) ((bintree_node*)a->b)->data
		printf("<%s: (%s) %s, %s, %i>\n",
			(ns[i]->data ? ns[i]->data : 0),
			(ns[i]->parent ? ndata(ns[i],parent) : 0),
			(ns[i]->left ? ndata(ns[i],left) : 0),
			(ns[i]->right ? ndata(ns[i],right) : 0),
			ns[i]->weight);
	}
	
	bintree_node* n = &na;
	printf("testing node_next\n");
	do {
		printf("- %s\n",n->data);
	} while ((n = node_next(n)));
	
	n = &nd;
	printf("testing node_prev\n");
	do {
		printf("- %s\n",n->data);
	} while ((n = node_prev(n)));
	
#undef ndata
	
#ifdef mtrace_active
	mtrace();	
#endif

	bintree* b = new_bintree(comp_string);	
	del_bintree(b,DEL_STRUCT);
	
	b = new_bintree(comp_string);
	bintree_add(b,"Brawn");
	bintree_add(b,"Dour");
	bintree_add(b,"Court");
	bintree_add(b,"Acclamate");
	
	char* s;
	void* h = iter_bintree(b);
	while (s = iter_bintree_next(&h))
		printf("%s\n",s);
	
	h = iter_bintree(b);
	while (s = iter_bintree_next(&h))
		printf("%s\n",bintree_contains(b,s));
	
	del_bintree(b,DEL_STRUCT);
	
#ifdef mtrace_active
	muntrace();
#endif
	
	return 0;
}

#endif

