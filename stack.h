#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>
#include "defines.h"
#include "types.h"

#define MAX_STACK 512
#define EMPTY_STACK -1

typedef struct stack {
    object_t * data[MAX_STACK];
    int tos;
} stack;

stack * stack_factory(void);
bool stack_full(stack *s);
bool stack_empty(stack *s);
void stack_push(stack *s, object_t *o);
object_t * stack_pop(stack *s);
void stack_print(stack *s);
void stack_destructor(stack *s);

#endif