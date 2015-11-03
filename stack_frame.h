#ifndef _STACK_FRAME_H
#define _STACK_FRAME_H

#include "stack.h"
#include "list.h"

typedef struct stack_frame_t {
    list_t *locals;
    list_t *constants;
    stack *s;
    unsigned int pc;
} stack_frame_t;

stack_frame_t * stack_frame_factory(void);
void stack_frame_destructor(stack_frame_t *sf);

#endif