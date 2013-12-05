#ifndef bufferworks_h
#define bufferworks_h

#include <numdefs.h>
#include <listworks.h>

/*{ buffermanager clearing... management (there is code in the define_type for this as well)*/
nodelist* _buffermanager_del_functions;
int _buffermanager_del_functions_initialized = 0;

typedef void(*buf_del_func)(void);

void clear_buffermanagers()
{
  node* n;
  if (!_buffermanager_del_functions_initialized)
    return;
  
  n = _buffermanager_del_functions->first;
  while (n)
  {
    ((buf_del_func)n->data)();
    n = n->next;
  }
  free(_buffermanager_del_functions);
  _buffermanager_del_functions = 0;
}

int buffermanager_default_batch_size = 64;
/*} END: clearing management*/

/*{ DEFINE: buffermanager_define_type*/
#define buffermanager_define_type(type) \
struct _buffermanager_ ## type \
{ \
  nodelist* ready; \
  nodelist* inuse; \
}; \
typedef struct _buffermanager_ ## type buffermanager_ ## type ; \
 \
buffermanager_ ## type  _buf_ ## type; \
type _buf_ ## type ## _initialized = 0; \
 \
void del_buf_ ## type() \
{ \
  if (_buf_ ## type ## _initialized) \
  { \
    del_nodelist(_buf_ ## type.ready,1); \
    del_nodelist(_buf_ ## type.inuse,1); \
  } \
  _buf_ ## type ## _initialized = 0; \
} \
 \
void buf_ ## type ## _add_batch(int len) \
{ \
  int i; \
  void* data; \
  \
  if (!_buf_ ## type ## _initialized) \
  { \
    _buf_ ## type.ready = init_nodelist(alloc_nodelist()); \
    _buf_ ## type.inuse = init_nodelist(alloc_nodelist()); \
    _buf_ ## type ## _initialized = 1; \
    if (!_buffermanager_del_functions_initialized) \
    { \
      _buffermanager_del_functions = init_nodelist(alloc_nodelist()); \
      _buffermanager_del_functions_initialized = 1; \
    } \
    nodelist_add(_buffermanager_del_functions,del_buf_ ## type); \
  } \
  \
  for (i = 0; i < len; ++i) \
  { \
    data = malloc(sizeof(type)); \
    nodelist_push(_buf_ ## type.ready,data); \
  } \
} \
 \
type * buf_ ## type ## _pull() \
{ \
  if (!_buf_ ## type ## _initialized) \
    buf_ ## type ## _add_batch(buffermanager_default_batch_size); \
  node* n = nodelist_pop_node(_buf_ ## type.ready); \
  nodelist_push_node(_buf_ ## type.inuse,n); \
  return (type*)n->data; \
} \
 \
void buf_ ## type ## _push(type* ptr) \
{ \
  node* n = nodelist_pop_node(_buf_ ## type.inuse); \
  n->data = ptr; \
  nodelist_push_node(_buf_ ## type.ready,n); \
}
/*} END buffermanager_define_type*/

/*{ DEFINE: buffermanager_define_struct*/

/*}*/


/*{ DEFINE: basic define_type definitions, (char, short, int, float, double, uint, ulong, llong, ullong)*/
buffermanager_define_type(char);
buffermanager_define_type(short);
buffermanager_define_type(int);
buffermanager_define_type(float);
buffermanager_define_type(double);

buffermanager_define_type(uint);
buffermanager_define_type(ulong);
buffermanager_define_type(llong);
buffermanager_define_type(ullong);
/*} END DEFINE*/

#endif

