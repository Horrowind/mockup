#if defined(LINUX)
#ifndef BASE_LINUX_H
#define BASE_LINUX_H

#if defined(GCC)

#include <stddef.h>
#define fallthrough __attribute__ ((fallthrough));
typedef __builtin_va_list VariableArgumentList;
#define no_return _Noreturn
#define warn_unused_result __attribute__((warn_unused_result))
#define variable_argument_begin(v,l) __builtin_va_start(v,l)
#define variable_argument_end(v)     __builtin_va_end(v)
#define variable_argument_get(v,l)   __builtin_va_arg(v,l)
#define variable_argument_copy(d,s)  __builtin_va_copy(d,s)
void* memcpy (void*, const void*, uptr) __asm__ ("c_memcpy");
void* memset (void*, int c, uptr) __asm__ ("c_memset");

#elif defined(TCC)
#include <stdarg.h>
#include <stddef.h>
#define fallthrough
typedef va_list VariableArgumentList;
#define no_return
#define variable_argument_begin(a,l) va_start(a,l)
#define variable_argument_end(v) va_end(v)
#define variable_argument_get(a,t) va_arg(a,t)
#define variable_argument_copy(d,s) va_copy(d,s)
void* memmove (void*, const void*, uptr) __asm__ ("c_memcpy");
void* memset (void*, int c, uptr) __asm__ ("c_memset");

#elif defined(CLANG)
#define fallthrough //[[fallthrough]];
#define no_return _Noreturn
typedef __builtin_va_list VariableArgumentList;
#define variable_argument_begin(v,l) __builtin_va_start(v,l)
#define variable_argument_end(v)     __builtin_va_end(v)
#define variable_argument_get(v,l)   __builtin_va_arg(v,l)
#define variable_argument_copy(d,s)  __builtin_va_copy(d,s)

void* memcpy (void*, const void*, uptr) __asm__ ("c_memcpy");
void* memset (void*, int c, uptr) __asm__ ("c_memset");
#else
#error "On linux only gcc, clang and tcc are supported"
#endif

#define export
#define debug_break raise_signal(2 /*SIGINT*/)

typedef struct
{
    int fd;
    i64 tell;
    int buf_pos;
    int buf_end;
    volatile int lock[1];
    char buf[2048];
} DirStream_;

typedef struct {
    long seconds;
    long nano_seconds;
} TimeSpec_;

typedef struct {
    ulong st_dev;
    u64   st_ino;
    long  st_nlink;

    uint  st_mode;
    uint  st_uid;
    uint  st_gid;
    uint  __pad0;
    u64   st_rdev;
    long  st_size;
    long  st_blksize;
    u64   st_blocks;

    TimeSpec_ st_atim;
    TimeSpec_ st_mtim;
    TimeSpec_ st_ctim;
    long __unused[3];
} Stat_;

typedef struct {
    int fd_;
    int fd_read_;
    int fd_write_;
    Stat_ stat_;
} Path;

typedef struct {
    Path path;
    Path* dir_path;
    DirStream_ dp_;
} DirIter;

#ifdef BASE_GUI
#include <X11/Xlib.h>
#include <GL/glx.h>

#define OPENGL_DEF(name, ogl_name, return_type, ...)                            \
    typedef return_type(*type_##name##_)( __VA_ARGS__ );                        \
    static type_##name##_ name = NULL;

#define OPENGL_CORE_DEF(name, ogl_name, return_type, ...)                       \
    typedef return_type(*type_##name##_)( __VA_ARGS__ );                        \
    static type_##name##_ name = NULL;
        
typedef struct {
    Display *dpy;
    Window win;
    XVisualInfo *vis;
    Colormap cmap;
    XSetWindowAttributes swa;
    XWindowAttributes attr;
    GLXFBConfig fbc;
    GLXContext gl_context;
    Atom wm_delete_window;
} PlatformWindow;

#endif //GUI
#endif //BASE_LINUX
#endif //defined(LINUX)
