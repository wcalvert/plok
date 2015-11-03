#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct object_t {
    struct object_vtable *vtable;
    unsigned char marked;
    struct object_t *next;
} object_t;

// Class method calling not implemented yet.
typedef struct class_method_t {
    int (*method)(object_t *[]);
    unsigned int num_args;
} class_method_t;

typedef struct int_obj {
    object_t object;
    int val;
} int_obj_t;

typedef struct float_obj {
    object_t object;
    float val;
} float_obj_t;

typedef struct string_obj {
    object_t object;
    char *str;
    size_t len;
} string_obj_t;

const char * class_name(object_t *o);
void object_destructor(object_t *o);
char * object_to_string(object_t *o);
int object_compare(object_t *o1, object_t *o2);

object_t * int_factory(int initial);
object_t * float_factory(float initial);
object_t * string_factory(const char *initial);

#endif