#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack_frame.h"

stack_frame * stack_frame_factory(void) {
    stack_frame *sf = malloc(sizeof(stack_frame));
    sf->pc = 0;
    sf->s = stack_factory();
    sf->locals = list_factory();
    sf->constants = list_factory();
}

void stack_frame_destructor(stack_frame *sf) {
    stack_destructor(sf->s);
    list_destructor(&sf->locals);
    list_destructor(&sf->constants);
    free(sf);
}
