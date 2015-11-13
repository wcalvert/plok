#ifndef _STACK_FRAME_H
#define _STACK_FRAME_H

#include <stdbool.h>
#include "list.h"

#define MAX_STACK 512
#define EMPTY_STACK -1

typedef struct stack_frame_t {
    list_t *locals;
    list_t *constants;
    object_t * stack[MAX_STACK];
    int tos;
    unsigned int pc;
} stack_frame_t;

stack_frame_t * stack_frame_factory(void);
void stack_frame_destructor(stack_frame_t *sf);

bool stack_full(stack_frame_t *sf);
bool stack_empty(stack_frame_t *sf);
void stack_push(stack_frame_t *sf, object_t *o);
object_t * stack_pop(stack_frame_t *sf);
void stack_print(stack_frame_t *sf);

#endif