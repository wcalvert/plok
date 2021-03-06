#ifndef _LIST_H_
#define _LIST_H_

#include "types.h"

typedef struct node {
    struct node *next;
    object_t *element;
    unsigned int id;
} node_t;

typedef struct list {
    node_t *head;
    node_t *curr;
    unsigned int size;
} list_t;

list_t * list_factory(void);
void list_append(list_t *list, object_t *o);
void list_delete(list_t *list, unsigned int n);
node_t * list_get(list_t *list, unsigned int n);
void list_destructor(list_t **list);
void list_print(list_t *list);

#endif