#if defined(WINDOWS)


typedef enum {
    WIN32_GET_FILEEX_INFO_STANDARD,
    WIN32_GET_FILEEX_MAX_INFO_LEVEL
} Win32GetFileExInfoLevels;

typedef enum {
    WIN32_LMEM_FIXED          = 0x0000,
    WIN32_LMEM_MOVEABLE       = 0x0002,
    WIN32_LMEM_NOCOMPACT      = 0x0010,
    WIN32_LMEM_NODISCARD      = 0x0020,
    WIN32_LMEM_ZEROINIT       = 0x0040,
    WIN32_LMEM_MODIFY         = 0x0080,
    WIN32_LMEM_DISCARDABLE    = 0x0f00,
    WIN32_LMEM_VALID_FLAGS    = 0x0f72,
    WIN32_LMEM_INVALID_HANDLE = 0x8000
} Win32LocalAllocFlags;

WIN32API
b32 GetFileAttributesExW(const wchar* lpFileName,
                         Win32GetFileExInfoLevels fInfoLevelId,
                         void* lpFileInformation);
WIN32API
void GetSystemInfo(Win32SystemInfo* system_info);

WIN32API
void* LocalAlloc(uint uFlags, size_t uBytes);

WIN32API
void* LocalFree(void* hMem);

WIN32API
ulong GetFullPathNameW(const wchar* lpFileName,
                                 ulong   nBufferLength,
                                 wchar*  lpBuffer,
                                 wchar** lpFilePart);

WIN32API
void DebugBreak(void);

typedef struct {
    ulong nLength;
    void* lpSecurityDescriptor;
    b32   bInheritHandle;
} Win32SecurityAttributes;


typedef enum {
    WIN32_GENERIC_ALL     = 0x10000000L,
    WIN32_GENERIC_EXECUTE = 0x20000000L,
    WIN32_GENERIC_WRITE   = 0x40000000L,
    WIN32_GENERIC_READ    = 0x80000000L,
} Win32AccessRights;

typedef enum {
    WIN32_FILE_SHARE_READ   = 0x00000001,
    WIN32_FILE_SHARE_WRITE  = 0x00000002,
    WIN32_FILE_SHARE_DELETE = 0x00000004,
} Win32ShareMode;

typedef enum {
    WIN32_CREATE_NEW        = 1,
    WIN32_CREATE_ALWAYS     = 2,
    WIN32_OPEN_EXISTING     = 3,
    WIN32_OPEN_ALWAYS       = 4,
    WIN32_TRUNCATE_EXISTING = 5,
} Win32CreationDisposition;

WIN32API
void* CreateFileW(const wchar* lpFileName,
                  ulong /*Win32AccessRights*/ dwDesiredAccess,
                  ulong /*Win32ShareMode*/ dwShareMode,
                  Win32SecurityAttributes* lpSecurityAttributes,
                  ulong /*Win32CreationDisposition*/ dwCreationDisposition,
                  ulong dwFlagsAndAttributes,
                  void* hTemplateFile);

typedef struct {
    ulong* Internal;
    ulong* InternalHigh;
    union {
        struct {
            ulong Offset;
            ulong OffsetHigh;
        };
        void* Pointer;
    };
    void* hEvent;
} Win32Overlapped;


typedef union {
    struct {
        ulong low_part;
        long high_part;
    };
    i64 quad_part;
} Win32LargeInteger;

typedef enum {
    WIN32_FILE_ATTRIBUTE_HIDDEN        = 0x2,
    WIN32_FILE_ATTRIBUTE_NORMAL        = 0x80,
    WIN32_FILE_ATTRIBUTE_DIRECTORY     = 0x10,
    WIN32_FILE_ATTRIBUTE_TEMPORARY     = 0x100,
    WIN32_FILE_ATTRIBUTE_REPARSE_POINT = 0x400,
    WIN32_INVALID_FILE_ATTRIBUTES      = 0xffffffff
} Win32FileAttribute;

WIN32API
b32 ReadFile(void* file, void* buffer, ulong number_of_bytes_to_read,
                    ulong* number_of_bytes_read, Win32Overlapped* overlapped);
WIN32API
b32 WriteFile(void* file,
              const void* buffer,
              ulong number_of_bytes_to_read,
              ulong* number_of_bytes_read,
              Win32Overlapped* lpOverlapped);

WIN32API
ulong GetCurrentDirectoryW(ulong buffer_length,
                                    wchar* buffer);
WIN32API
b32 GetFileSizeEx(void* hFile, Win32LargeInteger* lpFileSize);

WIN32API
b32 PathIsRelativeW(const wchar* path);

WIN32API
b32 CloseHandle(void*);

WIN32API
void* FindFirstFileW(const wchar* lpFileName,
                              Win32FindDataW* lpFindFileData);
WIN32API
b32 FindNextFileW(void* hFindFile,
                           Win32FindDataW* lpFindFileData);
WIN32API
b32 FindClose(void* hFindFile);


#define WIN32_CP_UTF8 65001
#define WIN32_WC_NO_BEST_FIT_CHARS 0x00000400

WIN32API
int WideCharToMultiByte(uint CodePage,
                        ulong dwFlags,
                        const wchar* lpWideCharStr,
                        int cchWideChar,
                        char* lpMultiByteStr,
                        int cbMultiByte,
                        const char* lpDefaultChar,
                        b32* lpUsedDefaultChar);

WIN32API
int lstrlenW(const wchar* lpString);
WIN32API
wchar* lstrcpyW(wchar* lpString1, const wchar* lpString2);
WIN32API
wchar* lstrcatW(wchar* lpString1, const wchar* lpString2);


typedef enum {
    WIN32_ERROR_SUCCESS             = 0L,
    WIN32_ERROR_NO_MORE_FILES       = 18L,
    WIN32_ERROR_MORE_DATA           = 234L,
    WIN32_ERROR_INSUFFICIENT_BUFFER = 122L
} Win32Error;


WIN32API
ulong /* Win32Error */ GetLastError();



void base_global_init() {
    Win32SystemInfo system_info;
    GetSystemInfo(&system_info);
    base_global.page_size = system_info.dwPageSize;
    assert(0);
}


void* page_alloc(ulong num_pages) {
    if(!base_global.is_initialized) base_global_init();

    return malloc(base_global.page_size * num_pages);
    /* return VirtualAlloc(0, base_global.page_size * num_pages, */
    /*                     MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); */
}

void page_dealloc(void* page, ulong num_pages) {
    if(!base_global.is_initialized) base_global_init();
    free(page);
//    VirtualFree(page, num_pages * base_global.page_size, MEM_RELEASE);
}
void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) {
    if(!base_global.is_initialized) base_global_init();
    return realloc(pages, base_global.page_size * new_num_pages);
}


void print_error(String s) {
    
}



static
wchar* utf8_to_wchar(String str) {
#if 0
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                     str.data, str.length, NULL, 0);
    LPWSTR result = LocalAlloc(LMEM_FIXED, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                        str.data, str.length, result, length);
    return result;
#else
    return NULL;
#endif
}

static
wchar* utf8_to_wchar_c(char* str) {
#if 0
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                     str, -1, NULL, 0);
    LPWSTR result = LocalAlloc(LMEM_FIXED, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                        str, -1, result, length);
    return result;
#else
    return NULL;
#endif
}

char* wchar_to_utf8_c(wchar* str) {
#if 0
    int length = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                                    str, -1, NULL, 0, NULL, NULL);
    char* result = LocalAlloc(LMEM_FIXED, length);
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        str, -1, result, length, NULL, NULL);
    return result;
#else
    return NULL;
#endif
}

static
String wchar_to_utf8(wchar* str) {
#if 0
    int length = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                                    str, -1, NULL, 0, NULL, NULL);
    String result = {
        .data = LocalAlloc(LMEM_FIXED, length),
        .length = length - 1
    };
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        str, -1, result.data, length, NULL, NULL);
    return result;
#else
    return (String){};
#endif
}


void path_load_stats(Path* path) {
    GetFileAttributesExW(path->path_name_, WIN32_GET_FILEEX_INFO_STANDARD, &path->data_);
}

void path_init_c(Path* path, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    ulong path_name_length = GetFullPathNameW(wname, 0, NULL, NULL);
    path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_name_length);
    GetFullPathNameW(wname, path_name_length, path->path_name_, NULL);
    LocalFree(wname);
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    path_load_stats(path);
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
    path_load_stats(path);
}

void path_init_from_c(Path* path, Path* dir, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    path_load_stats(path);
}


void path_init_from(Path* path, Path* dir, String name) {
    wchar* wname = utf8_to_wchar(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    path_load_stats(path);
}

void path_create_c(Path* path, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    ulong path_name_length = GetFullPathNameW(wname, 0, NULL, NULL);
    path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_name_length);
    GetFullPathNameW(wname, path_name_length, path->path_name_, NULL);
    LocalFree(wname);
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
    path_load_stats(path);
}

void path_create(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
    path_load_stats(path);
}

void path_create_from_c(Path* path, Path* dir, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
    path_load_stats(path);
}


void path_create_from(Path* path, Path* dir, String name) {
    wchar* wname = utf8_to_wchar(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
    path_load_stats(path);
}


void path_init_working_directory(Path* path) {
    ulong path_name_length = GetCurrentDirectoryW(0, NULL);
    path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_name_length);
    GetCurrentDirectoryW(path_name_length, path->path_name_);
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->handle_ = CreateFileW(orig->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    int path_length = lstrlenW(orig->path_name_) + 1;
    copy->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar_t));
    lstrcpyW(copy->path_name_, orig->path_name_);
    path_load_stats(copy);
}

Buffer path_open_file_aligned(Path* path, Arena* arena, int alignment) {
    ulong file_size = path_get_file_size(path);
    Buffer result = arena_alloc_buffer(arena, file_size, alignment);
    ReadFile(path->handle_, result.begin, file_size, NULL, NULL);
    return result;
}

Buffer path_open_file(Path* path, Arena* arena) {
    return path_open_file_aligned(path, arena, 1);
}

void path_close(Path* path) {
    CloseHandle(path->handle_);
    LocalFree(path->path_name_);
}

/* TODO
void path_navigate_c(Path* path, char* name) {
    int new_fd = openat(path->fd_, name, O_RDONLY);
    path_close(path);
    path->fd_ = new_fd;
    path->has_stats = 0;
    path->invalid = 0;
}
*/

/* TODO?!
void path_navigate(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}
*/


Buffer path_read_file_aligned(Path* path, Arena* arena, int alignment) {
    Win32LargeInteger file_size;
    GetFileSizeEx(path->handle_, &file_size);
    Buffer result = arena_alloc_buffer(arena, file_size.quad_part, alignment);
    ulong size_read;
    ReadFile(path->handle_, result.begin, file_size.quad_part, &size_read, 0);
    return result;
}

Buffer path_read_file(Path* path, Arena* arena) {
    return path_read_file_aligned(path, arena, 1);
}


void path_write_file(Path* path, Buffer buffer) {
    WriteFile(path->handle_, buffer.begin, buffer.end - buffer.begin, NULL, NULL);
}



int path_is_directory(Path* path) {
    path_load_stats(path);
    return path->data_.dwFileAttributes & WIN32_FILE_ATTRIBUTE_DIRECTORY;
}

ulong path_get_file_size(Path* path) {
    path_load_stats(path);
    Win32LargeInteger file_size_win;
    file_size_win.low_part  = path->data_.nFileSizeLow;
    file_size_win.high_part = path->data_.nFileSizeHigh;
    return (ulong)file_size_win.quad_part;
}

String path_get_name(Path* path, Arena* arena) {
    int path_name_length = WideCharToMultiByte(
        WIN32_CP_UTF8, WIN32_WC_NO_BEST_FIT_CHARS,
        path->path_name_, -1, NULL, 0, NULL, NULL);
    String result;
    result.data   = arena_alloc_array(arena, path_name_length, char);
    result.length = path_name_length;
    WideCharToMultiByte(WIN32_CP_UTF8, WIN32_WC_NO_BEST_FIT_CHARS,
                        path->path_name_, -1, result.data,
                        path_name_length, NULL, NULL);
    return result;
}

String path_get_base_name(Path* path, Arena* arena) {
    ulong path_name_size = GetFullPathNameW(path->path_name_, 0, NULL, NULL);
    wchar_t* base_name_win;
    wchar_t* path_name_win = arena_alloc_array(arena, path_name_size, wchar_t);
    GetFullPathNameW(path->path_name_, path_name_size, path_name_win, &base_name_win);
    String base_name = wchar_to_utf8(base_name_win);
    String result = { .length = base_name.length };
    result.data = arena_alloc_array(arena, base_name.length, char);
    for(int i = 0; i < result.length; i++) {
        result.data[i] = base_name.data[i];
    }
    LocalFree(base_name.data);
    return result;
    
}

int dir_iter_next(DirIter* dir_iter) {
    if(dir_iter->last_iteration) {
        return 1;
    }
    if(!dir_iter->first_iteration) {
        path_close(&dir_iter->path);
        dir_iter->last_iteration = 0;
    }
    dir_iter->handle_ =
        CreateFileW(dir_iter->find_file_data_.cFileName,
                            WIN32_GENERIC_READ, WIN32_FILE_SHARE_READ, 0,
                            WIN32_OPEN_EXISTING, 0, 0);
    
    b32 error =  FindNextFileW(dir_iter->handle_, &dir_iter->find_file_data_);
    if(!error) {
        if(GetLastError() == WIN32_ERROR_NO_MORE_FILES) {
            dir_iter->last_iteration = 1;
        } else {
            c_error_format("Error occured\n");
            exit(1);
        }
    }
    return 0;
}

DirIter dir_iter_begin(Path* path) {
    DirIter result = { 0 };
    int path_length = lstrlenW(path->path_name_) + 3;
    wchar* search_name = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
    lstrcpyW(search_name, path->path_name_);
    lstrcatW(search_name, L"\\*");
    result.handle_ = FindFirstFileW(search_name, &result.find_file_data_);
    //TODO: Error
    result.dir_path = path;
    result.last_iteration = 1;
    result.last_iteration = 0;
    return result;
}

void dir_iter_end(DirIter* dir_iter) {
    path_close(&dir_iter->path);
    FindClose(dir_iter->handle_);
}


#endif
