#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "types.h"
#include "vm.h"

list_t * list_factory(void) {
    list_t *list = malloc(sizeof(list_t));
    if(list == NULL) {
        vm_error(ERR_NO_MEM);
    }
    list->head = NULL;
    list->curr = NULL;
    list->size = 0;
    return list;
}

void list_append(list_t *list, object_t *o) {
    list->curr = malloc(sizeof(node_t));
    if(list->curr == NULL) {
        vm_error(ERR_NO_MEM);
    }
    list->curr->id = list->size;

    list->curr->next = list->head;
    list->head = list->curr;
    list->size++;

    list->curr->element = o;
}

static node_t * list_delete_node(list_t *list, node_t *node, unsigned int n) {
    node_t *temp = NULL;
    if(node == NULL) {
        return NULL;
    }
    if(node->id == n) {
        temp = node->next;
        object_destructor(node->element);
        free(node);
        return temp;
    } else {
        node->next = list_delete_node(list, node->next, n);
    }
    return node;
}

void list_delete(list_t *list, unsigned int n) {
    if(list == NULL) {
        return;
    }
    list->curr = list->head;
    list->head = list_delete_node(list, list->curr, n);
    list->size--;
}

node_t * list_get(list_t *list, unsigned int n) {
    list->curr = list->head;
    while(list->curr) {
        if(list->curr->id == n) {
            return list->curr;
        } else {
            list->curr = list->curr->next;
        }
    }
    return NULL;
}

void list_destructor(list_t **list) {
    (*list)->curr = (*list)->head;
    node_t *temp = NULL;
    while((*list)->curr)
    {
        temp = (*list)->curr->next;
        object_destructor((*list)->curr->element);
        free((*list)->curr);
        (*list)->curr = temp;
        (*list)->size--;

    }
    free(*list);
    list = NULL;
}

void list_print(list_t *list) {
    if(list == NULL) {
        printf("List is empty!\n");
        return;
    }
    list->curr = list->head;
    char *temp = NULL;
    while(list->curr) {
        temp = object_to_string(list->curr->element);
        printf("id: %i, %s\n", list->curr->id, temp);
        list->curr = list->curr->next;
        free(temp);
    }  
}
