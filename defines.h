#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "stdint.h"

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif

#if __WORDSIZE == 32
typedef uint32_t DWORD;
#else
typedef uint64_t DWORD;
#endif

#endif