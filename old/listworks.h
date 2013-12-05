#ifndef listworks_h
#define listworks_h

#include <stdio.h>
#include <stdlib.h>


/*{ TYPEDEF: voidarray*/
/* voidarray is just a void**, but used here for naming convenience between its functions */
typedef void** voidarray;

voidarray alloc_voidarray(int len)
{
  return malloc(sizeof(void*)*len);
}

void del_data_voidarray(voidarray varr, int len)
{
  int i;
  for (i = 0; i < len; ++i)
    free(varr[i]);
}

/* del_data_len doubles as both a del_data flag and the length of the array to delete data from */
void del_voidarray(voidarray varr, int del_data_len) 
{
  if (del_data_len)
    del_data_voidarray(varr,del_data_len);
  free(varr);
}

/*{ DEFINE: voidarray_define_type*/

#define voidarray_define_type(type) \
voidarray voidarray_ ## type ## s(voidarray varr, type * arr, int len) \
{ \
  int i; \
  varr = (varr ? varr : alloc_voidarray(len)); \
  for (i = 0; i < len; ++i) \
    varr[i] = (void*)&arr[i]; \
  return varr; \
} \
voidarray voidarray_new_ ## type ## s(voidarray varr, type * arr, int len) \
{ \
  int i; \
  varr = (varr ? varr : alloc_voidarray(len)); \
  for (i = 0; i < len; ++i) \
  { \
    varr[i] = malloc(sizeof(type*)); \
    type** ptr = (type**)&varr[i]; \
    **ptr = arr[i]; \
  } \
  return varr; \
} \
voidarray voidarray_ ## type ## ptrs(voidarray varr, type ** arr, int len) \
{ \
  int i; \
  varr = (varr ? varr : alloc_voidarray(len)); \
  for (i = 0; i < len; ++i) \
    varr[i] = (void*)arr[i]; \
  return varr; \
} \
type * voidarray_to_ ## type ## s(voidarray varr, type * arr, int len) \
{ \
  int i; \
  arr = (arr ? arr : malloc(sizeof(type)*len)); \
  for (i = 0; i < len; ++i) \
    arr[i] = *(type*)varr[i]; \
  return arr; \
} \
type ** voidarray_to_ ## type ## ptrs(voidarray varr, type ** arr, int len) \
{ \
  int i; \
  arr = (arr ? arr : malloc(sizeof(type*)*len)); \
  for (i = 0; i < len; ++i) \
    arr[i] = (type*)varr[i]; \
  return arr; \
}

/*}*/

voidarray_define_type(char);
voidarray_define_type(int);
voidarray_define_type(float);
/*} END voidarray*/


/*{ STRUCT: node*/
/* Simplest node EVAH */
struct _node
{
  void* data;
  void* next;
};
typedef struct _node node;

node* alloc_node()
{
  return (node*)malloc(sizeof(node));
}

node* init_node(node* n, void* data, void* next)
{
  n->data = data;
  n->next = next;
  return n;
}

void del_node(node* n, int del_data)
{
  if (del_data)
    free(n->data);
  free(n);
}
/*} END STRUCT*/


/*{ STRUCT: nodelist*/
/* Here's a list made of the above node struct */
struct _nodelist
{
  node* first;
  node* last;
  int size;
};
typedef struct _nodelist nodelist;

nodelist* alloc_nodelist()
{
  return (nodelist*)malloc(sizeof(nodelist));
}

nodelist* init_nodelist(nodelist* nlist)
{
  nlist->first = 0;
  nlist->last = 0;
  nlist->size = 0;
  return nlist;
}

del_nodelist(nodelist* nlist, int del_data)
{
  int i;
  
  node* n = nlist->first;
  node* n2 = 0;
  
  for (i=0; i<nlist->size; ++i)
  {
    n2 = n->next;
    del_node(n,del_data);
    n = n2;
  }
  free(nlist);
}

void* nodelist_pop(nodelist* nlist)
{
  node* nfirst;
  void* data;
  
  if (nlist->size == 0)
    return 0;
  
  --nlist->size;
    
  if (nlist->size == 0)
  {
    data = nlist->first->data;
    del_node(nlist->first,0);
    nlist->first = 0;
    nlist->last = 0;
    return data;
  }
  
  nfirst = nlist->first->next;
  data = nlist->first->data;
  del_node(nlist->first,0);
  nlist->first = nfirst;

  return data;
}

nodelist* nodelist_push_node(nodelist* nlist, node* n)
{
  if (!nlist->first)
  {
    nlist->first = n;
    nlist->last = n;
    nlist->size += 1;
    return nlist;
  }
  nlist->last->next = n;
  nlist->last = n;
  nlist->size += 1;
  return nlist;
}

node* nodelist_pop_node(nodelist* nlist)
{
  node* nfirst;
  
  if (nlist->size == 0)
    return 0;
  
  --nlist->size;
  
  if (nlist->size == 0)
  {
    nfirst = nlist->first;
    nlist->first = 0;
    nlist->last = 0;
    return nfirst;
  }
  
  nfirst = nlist->first;
  nlist->first = nlist->first->next;
  
  return nfirst;
}

nodelist* nodelist_add(nodelist* nlist, void* data)
{
  node* n = init_node(alloc_node(),data,0);
  return nodelist_push_node(nlist,n);
}

#define nodelist_push(a,b) nodelist_add(a,b)

void* nodelist_first(nodelist* nlist)
{
  return (nlist->first ? nlist->first->data : 0);
}

void* nodelist_last(nodelist* nlist)
{
  return (nlist->last ? nlist->last->data : 0);
}

/*} END STRUCT*/


#endif

