#if defined(WASM)
#ifndef BASE_PLATFORM_WASM_H
#define BASE_PLATFORM_WASM_H

#define fallthrough //[[fallthrough]];
#define no_return _Noreturn
typedef __builtin_va_list VariableArgumentList;
#define variable_argument_begin(v,l) __builtin_va_start(v,l)
#define variable_argument_end(v)     __builtin_va_end(v)
#define variable_argument_get(v,l)   __builtin_va_arg(v,l)
#define variable_argument_copy(d,s)  __builtin_va_copy(d,s)


void debug_break_js();
#define debug_break { debug_break_js(); }
void* memcpy (void*, const void*, ulong);
void* memset (void*, int c, ulong);

typedef struct {
    int fd;
} Path;

typedef struct {
    Path path;
    int index;
} DirIter;

typedef struct {

} PlatformWindow;

#define NULL ((void*)0)

#define OPENGL_DEF(name, ogl_name, return_type, ...)                            \
    extern return_type name( __VA_ARGS__ ) __asm__( #ogl_name "_" );

#define OPENGL_CORE_DEF(name, ogl_name, return_type, ...)                       \
    extern return_type name( __VA_ARGS__ ) __asm__( #ogl_name "_" );

#endif
#endif //BASE_PLATFORM_WASM_H