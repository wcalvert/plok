#ifndef _VM_H_
#define _VM_H_

#include "stack_frame.h"

typedef enum {
    ERR_NO_MEM, ERR_INVALID_LOCAL_VAR, ERR_INVALID_CODE_ADDRESS, ERR_INVALID_BUILTIN
} error_t;

typedef enum {
    STACK_PRINT_INT, LOCAL_PRINT_STRING
} builtin_method_t;

void vm_error(error_t e);
void invoke_builtin(builtin_method_t method, stack_frame *sf);

#endif