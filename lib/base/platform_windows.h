#if defined(WINDOWS)
#ifndef BASE_PLATFORM_WINDOWS_H
#define BASE_PLATFORM_WINDOWS_H

#define UNICODE
#define _UNICODE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

typedef struct {
    ulong dwFileAttributes;
    Win32FileTime ftCreationTime;
    Win32FileTime ftLastAccessTime;
    Win32FileTime ftLastWriteTime;
    ulong nFileSizeHigh;
    ulong nFileSizeLow;
    ulong dwReserved0;
    ulong dwReserved1;
    wchar cFileName[260];
    wchar cAlternateFileName[14];
} Win32FindDataW;

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

#define WIN32API __declspec(dllimport) __stdcall
WIN32API
void DebugBreak();

#endif //BASE_PLATFORM_WINDOWS_H
#endif //defined(WINDOWS)