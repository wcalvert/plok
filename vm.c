#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack_frame.h"
#include "vm.h"
#include "defines.h"

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
            break;
        default:
            fprintf(stderr, "Unknown error!\n");
            break;
    }
    exit(-1);
}

inline void invoke_builtin(builtin_method_t method, stack_frame_t *sf) {
    node_t *list_val = NULL;
    int_obj_t *i1 = NULL;
    object_t *o = NULL;
    char *temp = NULL;
    
    switch(method) {
        case STACK_PRINT_OBJ:
            o = stack_pop(sf->s);
            temp = object_to_string(o);
            printf("%s\n", temp);
            free(temp);
            break;
        case LOCAL_PRINT_OBJ:
            i1 = (int_obj_t*)stack_pop(sf->s);
            list_val = list_get(sf->locals, i1->val);
            temp = object_to_string(list_val->element);
            printf("%s\n", temp);
            free(temp);
            break;
        case CONSTANT_PRINT_OBJ:
            i1 = (int_obj_t*)stack_pop(sf->s);
            list_val = list_get(sf->constants, i1->val);
            temp = object_to_string(list_val->element);
            printf("%s\n", temp);
            free(temp);
            break;
        default:
            vm_error(ERR_INVALID_BUILTIN);
    }
}

vm_t * vm_factory() {
    vm_t *vm = malloc(sizeof(vm_t));
    if(vm == NULL) {
        vm_error(ERR_NO_MEM);
    }
    vm->first_obj = NULL;
    vm->num_objs = 0;
    vm->max_objs = 8;
    return vm;
}

void vm_destructor(vm_t *vm) {
    if(vm == NULL) {
        return;
    }
    free(vm);
}

void mark(object_t *object) {
    if (object->marked) return;

    object->marked = 1;
}

void mark_all(stack_frame_t *sf) {
    for (int i = 0; i <= sf->s->tos; i++) {
        mark(sf->s->data[i]);
    }
}

void sweep(vm_t *vm) {
    object_t **obj = &vm->first_obj;
    while (*obj) {
        if (!(*obj)->marked) {
            object_t *unreached = *obj;

            *obj = unreached->next;
            free(unreached);

            vm->num_objs--;
        } else {
            (*obj)->marked = 0;
            obj = &(*obj)->next;
        }
    }
}

void gc(vm_t *vm, stack_frame_t *sf) {
    int num_objs = vm->num_objs;

    mark_all(sf);
    sweep(vm);

    vm->max_objs = vm->num_objs * 2;

    debug("Collected %d objects, %d remaining.\n", num_objs - vm->num_objs, vm->num_objs);
}

int run(vm_t *vm, stack_frame_t *sf, unsigned char *code) {

    // index will be used to get the next byte in the code.
    // list_val is used for working with locals.
    // i1, i1, o1, and o2 are for working with stack.

    unsigned int index;
    node_t *list_val = NULL;
    int_obj_t *i1 = NULL;
    int_obj_t *i2 = NULL;
    object_t *o1 = NULL;
    object_t *o2 = NULL;

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
    
    DISPATCH();
    for(;;) {
        op_ret:
            debug("got return\n");
            break;
        op_iconst0:
            debug("got iconst0\n");
            stack_push(sf->s, OBJ_CREATE(int_factory, 0, vm, sf, gc));
            DISPATCH();
        op_iconst1:
            debug("got iconst1\n");
            stack_push(sf->s, OBJ_CREATE(int_factory, 1, vm, sf, gc));
            DISPATCH();
        op_iload:
            debug("got iload\n");
            index = code[sf->pc++];
            if(index > (sf->locals->size - 1)) {
                vm_error(ERR_INVALID_LOCAL_VAR);
                DISPATCH();
            }
            list_val = list_get(sf->locals, index);
            o1 = list_val->element;
            stack_push(sf->s, o1);
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
            i1 = (int_obj_t*)stack_pop(sf->s);
            i2 = (int_obj_t*)stack_pop(sf->s);
            i1->val = i1->val + i2->val;
            stack_push(sf->s, (object_t*)i1);
            debug("got iadd\n");
            DISPATCH();
        op_imult:
            i1 = (int_obj_t*)stack_pop(sf->s);
            i2 = (int_obj_t*)stack_pop(sf->s);
            i1->val = i1->val * i2->val;
            stack_push(sf->s, (object_t*)i1);
            debug("got imult\n");
            DISPATCH();
        op_ineg:
            debug("got ineg\n");
            i1 = (int_obj_t*)stack_pop(sf->s);
            i1->val = i1->val * -1;
            stack_push(sf->s, (object_t*)i1);
            DISPATCH();
        op_iinc:
            debug("got iinc\n");
            index = code[sf->pc++];
            if(index > (sf->locals->size - 1)) {
                vm_error(ERR_INVALID_LOCAL_VAR);
                DISPATCH();
            }
            list_val = list_get(sf->locals, index);
            i1 = (int_obj_t*)list_val->element;
            i1->val++;
            DISPATCH();
        op_breq:
            index = code[sf->pc++];
            i1 = (int_obj_t*)stack_pop(sf->s);
            // need to have code length available to check this - strlen will not work.
            /*if(index < 0 || index > (strlen(code) - 1)) {
                vm_error(ERR_INVALID_CODE_ADDRESS);
            }*/
            if(i1->val == 0) {
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
            o1 = stack_pop(sf->s);
            o2 = stack_pop(sf->s);
            stack_push(sf->s, o2);
            stack_push(sf->s, o1);
            DISPATCH();
        op_dup:
            debug("got dup\n");
            o1 = stack_pop(sf->s);
            stack_push(sf->s, o1);
            stack_push(sf->s, o1);
            DISPATCH();
        op_sload:
            debug("got sload\n");
            DISPATCH();
    }

    return 0;
}

void code_test() {
    vm_t *vm = vm_factory();
    stack_frame_t *sf = stack_frame_factory();
    unsigned char *code = "\x01\x0A\x02\x01\x02\x03\x00\x04\x01\x05\x06\x07\x08\x01\x09\x10\x03\x01\x0A\x00\x02\x0D\x05\x0A\x01\0";

    list_append(sf->locals, int_factory(123));
    list_append(sf->locals, int_factory(456));
    list_append(sf->locals, float_factory(1.21f));
    list_append(sf->constants, string_factory("Simple Code Test"));

    run(vm, sf, code);

    stack_frame_destructor(sf);
    vm_destructor(vm);
}

void gc_test() {
    vm_t *vm = vm_factory();
    stack_frame_t *sf = stack_frame_factory();
    unsigned char *code = "\x01\x0A\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x00";

    list_append(sf->constants, string_factory("Garbage Collector Test"));

    run(vm, sf, code);

    stack_frame_destructor(sf);
    vm_destructor(vm);
}

int main() {
    gc_test();
    code_test();
}