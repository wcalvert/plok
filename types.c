#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <strings.h>
#include <stdbool.h>
#include "types.h"
#include "defines.h"
#include "vm.h"

typedef struct object_vtable {
    void (*destructor)(object *);
    char * (*to_string)(object *);
    int (*compare)(object *, object *);
    bool (*equal)(object *, object *);
} object_vtable;

void object_destructor(object *object) {
    object->vtable->destructor(object);
}

char * object_to_string(object *object) {
    return object->vtable->to_string(object);
}

int object_compare(object *o1, object *o2) {
    return o1->vtable->compare(o1, o2);
}

bool object_equal(object *o1, object *o2) {
    return o1->vtable->equal(o1, o2);
}

// end of object API ////////////////////////////////////////////////////////

static char * private_to_string_int(object *o) {
    int_obj *i = container_of(o, int_obj, object);
    size_t size = snprintf(NULL, 0, "%d", i->val);
    char *s = malloc(size + 1);
    if(s == NULL) {
        vm_error(ERR_NO_MEM);
    }
    sprintf(s, "%d", i->val);
    return s;
}

static void private_free_int(object *o) {
    free(o);
}

static int private_compare_int(object *o1, object *o2) {
    int_obj *i1 = container_of(o1, int_obj, object);
    int_obj *i2 = container_of(o2, int_obj, object);
    return i1->val - i2->val;
}

static bool private_equal_int(object *o1, object *o2) {
    int_obj *i1 = container_of(o1, int_obj, object);
    int_obj *i2 = container_of(o2, int_obj, object);
    return i1->val == i2->val;
}

object_vtable int_obj_vtable = {
    private_free_int, private_to_string_int, private_compare_int, private_equal_int
};

object * int_factory(int initial) {
    int_obj *i = malloc(sizeof(int_obj));
    if(i == NULL) {
        vm_error(ERR_NO_MEM);
    }
    i->val = initial;
    i->object.vtable = &int_obj_vtable;
    return &i->object;
}

////////////////////////////////////////////////////////

static char * private_to_string_float(object *o) {
    float_obj *f = container_of(o, float_obj, object);
    size_t size = snprintf(NULL, 0, "%f", f->val);
    char *s = malloc(size + 1);
    if(s == NULL) {
        vm_error(ERR_NO_MEM);
    }
    sprintf(s, "%f", f->val);
    return s;
}

static void private_free_float(object *o) {
    free(o);
}

static int private_compare_float(object *o1, object *o2) {
    float_obj *f1 = container_of(o1, float_obj, object);
    float_obj *f2 = container_of(o2, float_obj, object);
    return (int)f1->val - f2->val;
}

static bool private_equal_float(object *o1, object *o2) {
    float_obj *f1 = container_of(o1, float_obj, object);
    float_obj *f2 = container_of(o2, float_obj, object);
    return (int)f1->val - f2->val;
}

object_vtable float_obj_vtable = {
    private_free_float, private_to_string_float, private_compare_float, private_equal_float
};

object * float_factory(float initial) {
    float_obj *f = malloc(sizeof(float_obj));
    if(f == NULL) {
        vm_error(ERR_NO_MEM);
    }
    f->val = initial;
    f->object.vtable = &float_obj_vtable;
    return &f->object;
}

////////////////////////////////////////////////////////

static char * private_to_string_string(object *o) {
    string_obj *s = container_of(o, string_obj, object);
    return strdup(s->str);
}

static void private_free_string(object *o) {
    string_obj *s = container_of(o, string_obj, object);
    free(s->str);
    free(s);
}

static int private_compare_string(object *o1, object *o2) {
    string_obj *s1 = container_of(o1, string_obj, object);
    string_obj *s2 = container_of(o2, string_obj, object);
    return strcasecmp(s1->str, s2->str);
}

static bool private_equal_string(object *o1, object *o2) {
    string_obj *s1 = container_of(o1, string_obj, object);
    string_obj *s2 = container_of(o2, string_obj, object);
    return strcmp(s1->str, s2->str) == 0;
}

object_vtable string_obj_vtable = {
    private_free_string, private_to_string_string, private_compare_string, private_equal_string
};

object * string_factory(const char *initial) {
    string_obj *s = malloc(sizeof(string_obj));
    if(s == NULL) {
        vm_error(ERR_NO_MEM);
    }
    s->str = malloc(strlen(initial));
    if(s->str == NULL) {
        vm_error(ERR_NO_MEM);
    }
    strcpy(s->str, initial);
    s->object.vtable = &string_obj_vtable;
    return &s->object;
}
