#if defined(WINDOWS)
#ifndef BASE_PLATFORM_WINDOWS_H
#define BASE_PLATFORM_WINDOWS_H

#if defined(GCC)
#include <stddef.h>
#define fallthrough __attribute__ ((fallthrough));
typedef __builtin_va_list VariableArgumentList;
#define no_return _Noreturn
#define variable_argument_begin(v,l) __builtin_va_start(v,l)
#define variable_argument_end(v)     __builtin_va_end(v)
#define variable_argument_get(v,l)   __builtin_va_arg(v,l)
#define variable_argument_copy(d,s)  __builtin_va_copy(d,s)
#endif

#define UNICODE
#define _UNICODE
#include <windows.h>

#undef fallthrough
#define fallthrough
#define debug_break DebugBreak()


typedef struct  {
    ulong low_date_time;
    ulong high_date_time;
} Win32FileTime;

typedef struct {
    ulong dwFileAttributes;
    Win32FileTime ftCreationTime;
    Win32FileTime ftLastAccessTime;
    Win32FileTime ftLastWriteTime;
    ulong nFileSizeHigh;
    ulong nFileSizeLow;
} Win32FileAttributeData;

typedef struct _WIN32_FIND_DATAW Win32FindDataW;

typedef struct {
    union {
        ulong dwOemId;
        struct {
            ushort wProcessorArchitecture;
            ushort wReserved;
        };
    };
    ulong  dwPageSize;
    void*  lpMinimumApplicationAddress;
    void*  lpMaximumApplicationAddress;
    ulong* dwActiveProcessorMask;
    ulong  dwNumberOfProcessors;
    ulong  dwProcessorType;
    ulong  dwAllocationGranularity;
    ushort wProcessorLevel;
    ushort wProcessorRevision;
} Win32SystemInfo;

typedef struct {
    void*  handle_;
    wchar* path_name_;
    Win32FileAttributeData data_;
} Path;

typedef struct {
    Path path;
    Path* dir_path;
    void* handle_;
    Win32FindDataW find_file_data_;
    int first_iteration : 1;
    int last_iteration  : 1;
} DirIter;

#ifdef BASE_GUI
typedef struct {
    HWND handle;
} PlatformWindow;

typedef uint Texture;

typedef struct {
    uint vbo_id, vao_id, ebo_id;
    uint program_id;
    uint vertex_shader_id;
    uint fragment_shader_id;
    int position_location;
    int uv_location;
    int color_location;
    int texture_location;
    int projection_location;

    int max_vertex_buffer_length;
    int max_elements_buffer_length;
} Device;
#endif //GUI
#endif //BASE_PLATFORM_WINDOWS_H
#endif //defined(WINDOWS)