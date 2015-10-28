#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"

stack * stack_factory(void) {
    stack *s = malloc(sizeof(stack));
    s->tos = EMPTY_STACK;
    return s;
}

bool stack_full(stack *s) {
    return s->tos < MAX_STACK;
}

bool stack_empty(stack *s) {
    return s->tos == EMPTY_STACK;
}

void stack_push(stack *s, DWORD d) {
    s->tos++;
    s->data[s->tos] = d;
}

DWORD stack_pop(stack *s) {
    DWORD result = s->data[s->tos];
    s->tos--;
    return result;
}

void stack_print(stack *s) {
    if(stack_empty(s)) {
        printf("Stack is empty!\n");
        return;
    }
    for(int i=s->tos; i>=0; i--) {
        printf("%d\n", s->data[i]);
    }
}

void stack_destructor(stack *s) {
    if(s != NULL) {
        free(s);
    }
}
