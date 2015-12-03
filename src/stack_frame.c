#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack_frame.h"
#include "vm.h"

stack_frame_t * stack_frame_factory(void) {
    stack_frame_t *sf = malloc(sizeof(stack_frame_t));
    sf->pc = 0;
    sf->tos = EMPTY_STACK;
    sf->locals = list_factory();
    sf->constants = list_factory();
}

void stack_frame_destructor(stack_frame_t *sf) {
    list_destructor(&sf->locals);
    list_destructor(&sf->constants);
    free(sf);
}

inline bool stack_full(stack_frame_t *sf) {
    return sf->tos >= MAX_STACK;
}

inline bool stack_empty(stack_frame_t *sf) {
    return sf->tos == EMPTY_STACK;
}

void stack_push(stack_frame_t *sf, object_t *o) {
    if(stack_full(sf)) {
        vm_error(ERR_STACK_OVERFLOW);
    }
    sf->tos++;
    sf->stack[sf->tos] = o;
}

object_t * stack_pop(stack_frame_t *sf) {
    if(stack_empty(sf)) {
        vm_error(ERR_STACK_UNDERFLOW);
    }
    object_t *result = sf->stack[sf->tos];
    sf->tos--;
    return result;
}

void stack_print(stack_frame_t *sf) {
    if(stack_empty(sf)) {
        printf("Stack is empty!\n");
        return;
    }
    char *temp = NULL;
    for(int i=sf->tos; i>=0; i--) {
        temp = object_to_string(sf->stack[i]);
        printf("%s\n", temp);
        free(temp);
    }

}
