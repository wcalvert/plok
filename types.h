#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct object {
    struct object_vtable *vtable;
} object;

typedef struct int_obj {
    object object;
    int val;
} int_obj;

typedef struct float_obj {
    object object;
    float val;
} float_obj;

typedef struct string_obj {
    object object;
    char *str;
    size_t len;
} string_obj;

void object_destructor(object *o);
char * object_to_string(object *o);
int object_compare(object *o1, object *o2);

object * int_factory(int initial);
object * float_factory(float initial);
object * string_factory(const char *initial);

#endif