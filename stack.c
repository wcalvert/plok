#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"
#include "vm.h"

stack * stack_factory(void) {
    stack *s = malloc(sizeof(stack));
    if(s == NULL) {
        vm_error(ERR_NO_MEM);
    }
    s->tos = EMPTY_STACK;
    return s;
}

bool stack_full(stack *s) {
    return s->tos < MAX_STACK;
}

bool stack_empty(stack *s) {
    return s->tos == EMPTY_STACK;
}

void stack_push(stack *s, object *o) {
    s->tos++;
    s->data[s->tos] = o;
}

object * stack_pop(stack *s) {
    object *result = s->data[s->tos];
    s->tos--;
    return result;
}

void stack_print(stack *s) {
    if(stack_empty(s)) {
        printf("Stack is empty!\n");
        return;
    }
    /*for(int i=s->tos; i>=0; i--) {
        printf("%d\n", s->data[i]);
    }*/
}

void stack_destructor(stack *s) {
    if(s != NULL) {
        free(s);
    }
}
