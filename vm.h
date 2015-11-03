#ifndef _VM_H_
#define _VM_H_

#include "stack_frame.h"
#include "types.h"

typedef struct vm {
    struct object_t *first_obj;
    int num_objs;
    int max_objs;
} vm_t;

typedef enum {
    ERR_NO_MEM, ERR_INVALID_LOCAL_VAR, ERR_INVALID_CODE_ADDRESS, ERR_INVALID_BUILTIN
} error_t;

typedef enum {
    STACK_PRINT_OBJ, LOCAL_PRINT_OBJ, CONSTANT_PRINT_OBJ
} builtin_method_t;

void vm_error(error_t e);
void invoke_builtin(builtin_method_t method, stack_frame_t *sf);

#endif
