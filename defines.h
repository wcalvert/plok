#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "stdint.h"

typedef enum bool {
    FALSE = 0, TRUE
} bool;

#if __WORDSIZE == 32
typedef uint32_t DWORD;
#else
typedef uint64_t DWORD;
#endif

#endif