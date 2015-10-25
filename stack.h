#ifndef _STACK_H_
#define _STACK_H_

#include "defines.h"

#define MAX_STACK 512
#define EMPTY_STACK -1

typedef struct stack {
    DWORD data[MAX_STACK];
    int tos;
} stack;

stack * stack_factory(void);
bool stack_full(stack *s);
bool stack_empty(stack *s);
void stack_push(stack *s, DWORD d);
DWORD stack_pop(stack *s);
void stack_print(stack *s);
void stack_destructor(stack *s);

#endif