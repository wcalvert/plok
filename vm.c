#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack_frame.h"
#include "vm.h"

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif

void vm_error(error_t e) {
    switch(e) {
        case ERR_NO_MEM:
            fprintf(stderr, "ERR_NO_MEM: no more memory could be allocated\n");
            break;
        case ERR_INVALID_LOCAL_VAR:
            fprintf(stderr, "ERR_INVALID_LOCAL_VAR: attempted to access invalid local variable\n");
            break;
        case ERR_INVALID_CODE_ADDRESS:
            fprintf(stderr, "ERR_INVALID_CODE_ADDRESS: attempted to jump to invalid address in code\n");
            break;
        case ERR_INVALID_BUILTIN:
            fprintf(stderr, "ERR_INVALID_BUILTIN: attempted to call unknown built-in method\n");
        default:
            fprintf(stderr, "Unknown error!\n");
            break;
    }
}

inline void invoke_builtin(builtin_method_t method, stack_frame *sf) {
    int i;
    node_t *list_val = NULL;
    string_obj *stemp = NULL;

    switch(method) {
        case STACK_PRINT_INT:
            i = stack_pop(sf->s);
            printf("%d\n", i);
            break;
        case LOCAL_PRINT_STRING:
            i = stack_pop(sf->s);
            list_val = list_get(sf->locals, i);
            stemp = (string_obj *) list_val->element;
            printf("%s\n", stemp->str);
            break;
        default:
            vm_error(ERR_INVALID_BUILTIN);
    }
}

int main(void) {

    // Create the stack frame, and populate the local vars with some example values.

    stack_frame *sf = stack_frame_factory();

    int_obj *i = int_factory(123);
    list_append(sf->locals, (object*)i);
    i = int_factory(456);
    list_append(sf->locals, (object*)i);
    string_obj *s = string_factory("Hello World");
    list_append(sf->locals, (object*)s);
    float_obj *f = float_factory(1.21f);
    list_append(sf->locals, (object*)f);

    // Currently, byte code needs to be created by hand. This code just does some very basic operations.

    unsigned char *code = "\x01\x02\x03\x00\x04\x01\x05\x06\x07\x08\x01\x09\x0D\x03\x01\x0A\x00\x02\x0D\x05\x0A\x01\0";

    static void* dispatch_table[] = {
        &&op_ret,           // x00 return
        &&op_iconst0,       // x01 push 0 onto stack
        &&op_iconst1,       // x02 push 1 onto stack
        &&op_iload,         // x03 push locals[index] onto stack 
        &&op_istore,        // x04 pop and store in locals[index] 
        &&op_iadd,          // x05 pop two values, add them, push onto stack
        &&op_imult,         // x06 pop two values, multiply them, push onto stack
        &&op_ineg,          // x07 pop, negate, push
        &&op_iinc,          // x08 increment locals[index]
        &&op_breq,          // x09 pop value, if 0, set pc=index
        &&op_invoke,        // x0A call built-in method[index]
        &&op_pop,           // x0B discard one off the stack
        &&op_swap,          // x0C swap top values on stack
        &&op_dup,           // x0D duplicate top value on stack
        &&op_sload          // x0E push locals[index] onto stack
    };
    #define DISPATCH() goto *dispatch_table[code[sf->pc++]]

    // index will be used to get the next byte in the code.
    // list_val is used for working with locals.
    // value1 and value2 are values popped off the stack.
    // itemp and stemp are for working with intermediate values while casting stuff.
    unsigned int index;
    node_t *list_val = NULL;
    int value1, value2;
    int_obj *itemp = NULL;
    string_obj *stemp = NULL;

    DISPATCH();
    for(;;) {
        op_ret:
            debug("got return\n");
            break;
        op_iconst0:
            debug("got iconst0\n");
            stack_push(sf->s, 0);
            DISPATCH();
        op_iconst1:
            debug("got iconst1\n");
            stack_push(sf->s, 1);
            DISPATCH();
        op_iload:
            debug("got iload\n");
            index = code[sf->pc++];
            if(index > (sf->locals->size - 1)) {
                vm_error(ERR_INVALID_LOCAL_VAR);
                DISPATCH();
            }
            list_val = list_get(sf->locals, index);
            itemp = (int_obj*) list_val->element;
            stack_push(sf->s, i->val);
            DISPATCH();
        op_istore:
            debug("got istore\n");
            index = code[sf->pc++];
            if(index > (sf->locals->size - 1)) {
                vm_error(ERR_INVALID_LOCAL_VAR);
                DISPATCH();
            }
            list_val = list_get(sf->locals, index);
            DISPATCH();
        op_iadd:
            value1 = stack_pop(sf->s);
            value2 = stack_pop(sf->s);
            stack_push(sf->s, value1 + value2);
            debug("got iadd\n");
            DISPATCH();
        op_imult:
            value1 = stack_pop(sf->s);
            value2 = stack_pop(sf->s);
            stack_push(sf->s, value1 * value2);
            debug("got imult\n");
            DISPATCH();
        op_ineg:
            debug("got ineg\n");
            value1 = stack_pop(sf->s);
            stack_push(sf->s, value1 * -1);
            DISPATCH();
        op_iinc:
            debug("got iinc\n");
            index = code[sf->pc++];
            if(index > (sf->locals->size - 1)) {
                vm_error(ERR_INVALID_LOCAL_VAR);
                DISPATCH();
            }
            list_val = list_get(sf->locals, index);
            itemp = (int_obj*)list_val->element;
            itemp->val++;
            DISPATCH();
        op_breq:
            index = code[sf->pc++];
            value1 = stack_pop(sf->s);
            // need to have code length available to check this - strlen will not work.
            /*if(index < 0 || index > (strlen(code) - 1)) {
                vm_error(ERR_INVALID_CODE_ADDRESS);
            }*/
            if(value1 == 0) {
                debug("going to set pc to: %d\n", index);
                sf->pc = index;
            }
            DISPATCH();
        op_invoke:
            debug("got invoke\n");
            index = code[sf->pc++];
            invoke_builtin(index, sf);
            DISPATCH();
        op_pop:
            debug("got pop\n");
            stack_pop(sf->s);
            DISPATCH();
        op_swap:
            debug("got swap\n");
            value1 = stack_pop(sf->s);
            value2 = stack_pop(sf->s);
            stack_push(sf->s, value2);
            stack_push(sf->s, value1);
            DISPATCH();
        op_dup:
            debug("got dup\n");
            value1 = stack_pop(sf->s);
            stack_push(sf->s, value1);
            stack_push(sf->s, value1);
            DISPATCH();
        op_sload:
            debug("got sload\n");
            DISPATCH();
    }

    stack_frame_destructor(sf);

    return 0;
}