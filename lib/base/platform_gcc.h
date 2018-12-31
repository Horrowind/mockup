#if defined(GCC)
#ifndef BASE_PLATFORM_GCC_H
#define BASE_PLATFORM_GCC_H
#define fallthrough __attribute__ ((fallthrough));
typedef __builtin_va_list VariableArgumentList;
#define variable_argument_begin(v,l) __builtin_va_start(v,l)
#define variable_argument_end(v)     __builtin_va_end(v)
#define variable_argument_get(v,l)   __builtin_va_arg(v,l)
#define variable_argument_copy(d,s)  __builtin_va_copy(d,s)


#endif //BASE_PLATFORM_GCC_H
#endif //defined(GCC)