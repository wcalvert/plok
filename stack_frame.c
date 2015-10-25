#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack_frame.h"

stack_frame * stack_frame_factory(void) {
    stack_frame *sf = malloc(sizeof(stack_frame));
    sf->pc = 0;
    sf->s = stack_factory();
    sf->locals = list_factory();
}

void stack_frame_destructor(stack_frame *sf) {
    stack_destructor(sf->s);
    list_destructor(&sf->locals);
    free(sf);
}

/*int main() {
    stack_frame *sf = stack_frame_factory();

    int_obj *b = int_factory(123);
    list_append(sf->local_ints, b);
    b = int_factory(456);
    list_append(sf->local_ints, b);


    node_t *temp = list_get(sf->local_ints,1);
    stack_push(sf->s, temp->element->val);
    stack_print(sf->s);
    DWORD temp2 = stack_pop(sf->s);
    temp2++;
    stack_push(sf->s, temp2);
    stack_print(sf->s);

    stack_frame_destructor(sf);

}*/