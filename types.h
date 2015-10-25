#ifndef _TYPES_H_
#define _TYPES_H_

typedef enum {
    INTEGER, FLOAT, STRING
} object_type;

typedef struct object {
    object_type o_type;
    void (*destructor)(struct object **o);
} object;

typedef struct int_obj {
    object_type o_type;
    void (*destructor)(struct int_obj **i);
    int val;
} int_obj;

typedef struct float_obj {
    object_type o_type;
    void (*destructor)(struct float_obj **f);
    float val;
} float_obj;

typedef struct string_obj {
    object_type o_type;
    void (*destructor)(struct string_obj **s);
    char * str;
    size_t len;
} string_obj;


int_obj * int_factory(int initial);
void int_destructor(int_obj **n);
float_obj * float_factory(float initial);
void float_destructor(float_obj **n);
string_obj * string_factory(const char * initial);
void string_destructor(string_obj **s);

#endif