#ifndef _DEFINES_H_
#define _DEFINES_H_

#define DISPATCH() goto *dispatch_table[code[sf->pc++]]

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))

#define OBJ_CREATE(factory, initial, vm, sf, gc)    ({ if(vm->num_objs == vm->max_objs) gc(vm, sf); \
                                                    object_t *o = factory(initial); \
                                                    o->marked = 0; \
                                                    o->next = vm->first_obj; \
                                                    vm->first_obj = o; \
                                                    vm->num_objs++; \
                                                    o; })

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif

#endif
