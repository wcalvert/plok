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
    const char * (*class_name)(object_t *);
    void (*destructor)(object_t *);
    char * (*to_string)(object_t *);
    int (*compare)(object_t *, object_t *);
    bool (*equal)(object_t *, object_t *);
} object_vtable;

const char * class_name(object_t *object) {
    object->vtable->class_name(object);
}

void object_destructor(object_t *object) {
    object->vtable->destructor(object);
}

char * object_to_string(object_t *object) {
    return object->vtable->to_string(object);
}

int object_compare(object_t *o1, object_t *o2) {
    return o1->vtable->compare(o1, o2);
}

bool object_equal(object_t *o1, object_t *o2) {
    return o1->vtable->equal(o1, o2);
}

// end of object API ////////////////////////////////////////////////////////

static const char * private_class_name_int(object_t *o) {
    return "int";
}

static char * private_to_string_int(object_t *o) {
    int_obj_t *i = container_of(o, int_obj_t, object);
    size_t size = snprintf(NULL, 0, "%d", i->val);
    char *s = malloc(size + 1);
    if(s == NULL) {
        vm_error(ERR_NO_MEM);
    }
    sprintf(s, "%d", i->val);
    return s;
}

static void private_free_int(object_t *o) {
    free(o);
}

static int private_compare_int(object_t *o1, object_t *o2) {
    int_obj_t *i1 = container_of(o1, int_obj_t, object);
    int_obj_t *i2 = container_of(o2, int_obj_t, object);
    return i1->val - i2->val;
}

static bool private_equal_int(object_t *o1, object_t *o2) {
    int_obj_t *i1 = container_of(o1, int_obj_t, object);
    int_obj_t *i2 = container_of(o2, int_obj_t, object);
    return i1->val == i2->val;
}

object_vtable int_obj_vtable = {
    private_class_name_int, private_free_int, private_to_string_int, private_compare_int, private_equal_int
};

object_t * int_factory(int initial) {
    int_obj_t *i = malloc(sizeof(int_obj_t));
    if(i == NULL) {
        vm_error(ERR_NO_MEM);
    }
    i->val = initial;
    i->object.vtable = &int_obj_vtable;
    return &i->object;
}

////////////////////////////////////////////////////////

static const char * private_class_name_float(object_t *o) {
    return "float";
}

static char * private_to_string_float(object_t *o) {
    float_obj_t *f = container_of(o, float_obj_t, object);
    size_t size = snprintf(NULL, 0, "%f", f->val);
    char *s = malloc(size + 1);
    if(s == NULL) {
        vm_error(ERR_NO_MEM);
    }
    sprintf(s, "%f", f->val);
    return s;
}

static void private_free_float(object_t *o) {
    free(o);
}

static int private_compare_float(object_t *o1, object_t *o2) {
    float_obj_t *f1 = container_of(o1, float_obj_t, object);
    float_obj_t *f2 = container_of(o2, float_obj_t, object);
    return (int)f1->val - f2->val;
}

static bool private_equal_float(object_t *o1, object_t *o2) {
    float_obj_t *f1 = container_of(o1, float_obj_t, object);
    float_obj_t *f2 = container_of(o2, float_obj_t, object);
    return (int)f1->val - f2->val;
}

object_vtable float_obj_vtable = {
    private_class_name_float, private_free_float, private_to_string_float, private_compare_float, private_equal_float
};

object_t * float_factory(float initial) {
    float_obj_t *f = malloc(sizeof(float_obj_t));
    if(f == NULL) {
        vm_error(ERR_NO_MEM);
    }
    f->val = initial;
    f->object.vtable = &float_obj_vtable;
    return &f->object;
}

////////////////////////////////////////////////////////

static const char * private_class_name_string(object_t *o) {
    return "string";
}

static char * private_to_string_string(object_t *o) {
    string_obj_t *s = container_of(o, string_obj_t, object);
    return strdup(s->str);
}

static void private_free_string(object_t *o) {
    string_obj_t *s = container_of(o, string_obj_t, object);
    free(s->str);
    free(s);
}

static int private_compare_string(object_t *o1, object_t *o2) {
    string_obj_t *s1 = container_of(o1, string_obj_t, object);
    string_obj_t *s2 = container_of(o2, string_obj_t, object);
    return strcasecmp(s1->str, s2->str);
}

static bool private_equal_string(object_t *o1, object_t *o2) {
    string_obj_t *s1 = container_of(o1, string_obj_t, object);
    string_obj_t *s2 = container_of(o2, string_obj_t, object);
    return strcmp(s1->str, s2->str) == 0;
}

object_vtable string_obj_vtable = {
    private_class_name_string, private_free_string, private_to_string_string, private_compare_string, private_equal_string
};

object_t * string_factory(const char *initial) {
    string_obj_t *s = malloc(sizeof(string_obj_t));
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
