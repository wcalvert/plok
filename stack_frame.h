#ifndef _STACK_FRAME_H
#define _STACK_FRAME_H

#include "stack.h"
#include "list.h"

typedef struct stack_frame {
    list_t *locals;
    stack *s;
    unsigned int pc;
} stack_frame;

stack_frame * stack_frame_factory(void);
void stack_frame_destructor(stack_frame *sf);

#endif