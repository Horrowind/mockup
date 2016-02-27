#ifndef BASE_BASE_H
#define BASE_BASE_H

#ifdef BASE_IMPLEMENTATION
#define BASE_BOGUS2_IMPLEMENTATION
#define BASE_FIXED_STRING_IMPLEMENTATION
#define BASE_HASH_IMPLEMENTATION
#define BASE_PLATFORM_IMPLEMENTATION
#define BASE_POOL_IMPLEMENTATION
#define BASE_MISC_IMPLEMENTATION
#define BASE_STACK_IMPLEMENTATION
#define BASE_DATASIZES_IMPLEMENTATION
#define BASE_VARIABLE_STRING_IMPLEMENTATION
#endif

#include "bogus2.h"
#include "fixed_string.h"
#include "hash.h"
#include "platform.h"
#include "pool.h"
#include "misc.h"
#include "stack.h"
#include "datasizes.h"
#include "variable_string.h"
#endif //BASE_BASE_H
