#if defined(LINUX)

#define INVALID_FD (-1)

static __inline long linux_syscall0(long n) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall1(long n, long a1) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall2(long n, long a1, long a2) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall3(long n, long a1, long a2, long a3) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall4(long n, long a1, long a2, long a3, long a4) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall5(long n, long a1, long a2, long a3, long a4, long a5) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}


#define LINUX_SYSCALL_NARGS_X_(a,b,c,d,e,f,g,h,n,...) n
#define LINUX_SYSCALL_NARGS_(...) LINUX_SYSCALL_NARGS_X_(__VA_ARGS__,7,6,5,4,3,2,1,0,)
#define LINUX_SYSCALL_CONCAT_X_(a,b) a##b
#define LINUX_SYSCALL_CONCAT_(a,b) LINUX_SYSCALL_CONCAT_X_(a,b)
#define LINUX_SYSCALL_DISP_(b,...) LINUX_SYSCALL_CONCAT_(b, LINUX_SYSCALL_NARGS_(__VA_ARGS__))(__VA_ARGS__)

#define linux_syscall(...) LINUX_SYSCALL_DISP_(linux_syscall,__VA_ARGS__)

enum {
    LINUX_SYSCALL_READ           =   0,
    LINUX_SYSCALL_WRITE          =   1,
    LINUX_SYSCALL_OPEN           =   2,
    LINUX_SYSCALL_CLOSE          =   3,
    LINUX_SYSCALL_FSTAT          =   5,
    LINUX_SYSCALL_LSTAT          =   6,
    LINUX_SYSCALL_MMAP           =   9,
    LINUX_SYSCALL_MUNMAP         =  11,
    LINUX_SYSCALL_RT_SIGPROCMASK =  14,
    LINUX_SYSCALL_MREMAP         =  25,
    LINUX_SYSCALL_DUP            =  32,
    LINUX_SYSCALL_READLINK       =  89,
    LINUX_SYSCALL_GETTID         = 186,
    LINUX_SYSCALL_TKILL          = 200,
    LINUX_SYSCALL_EXIT_GROUP     = 231,
    LINUX_SYSCALL_OPENAT         = 257,
} LinuxSyscall;

enum {
    OPEN_FLAGS_READ           =       0,
    OPEN_FLAGS_WRITE          =       1,
    OPEN_FLAGS_READ_WRITE     =       2,

    OPEN_FLAGS_LARGE_FILE     =       0,
    OPEN_FLAGS_CREATE         =    0x40,
    OPEN_FLAGS_CLOSE_ON_EXEC  = 0x80000,

    OPEN_FLAGS_COMMON         = OPEN_FLAGS_LARGE_FILE | OPEN_FLAGS_CLOSE_ON_EXEC,
} LinuxOpenFlags;

enum {
    MISC_FLAGS_CURRENT_DIR_FD =    -100,
} LinuxMiscFlags;

enum {
    FILE_MODE_MASK            =  0x170000,
    FILE_MODE_DIRECTORY       =  0x040000,
} LinuxFileMode;

enum {
    MEMORY_FLAGS_READ         =  1,
    MEMORY_FLAGS_WRITE        =  2,
    MEMORY_FLAGS_PRIVATE      =  2,
    MEMORY_FLAGS_ANONYMOUS    = 32,
    MEMORY_FLAGS_MAYMOVE      =  1,
} LinuxMemoryFlags;

long linux_syscall_read(int file_descriptor, void* buf, unsigned long count) {
    return linux_syscall(LINUX_SYSCALL_READ, (long)file_descriptor, (long)buf, (long)count);
}

long linux_syscall_write(int file_descriptor, const void* buf, unsigned long count) {
    return linux_syscall(LINUX_SYSCALL_WRITE, (long)file_descriptor, (long)buf, (long)count);
}

long linux_syscall_open(const char* file_path, int flags, int mode) {
    return linux_syscall(LINUX_SYSCALL_OPEN, (long)file_path, (long)flags, (long)mode);
}

int linux_syscall_close(int file_descriptor) {
    return (int)linux_syscall(LINUX_SYSCALL_CLOSE, (long)file_descriptor);
}

long linux_syscall_fstat(int file_descriptor, Stat_* stat) {
    return linux_syscall(LINUX_SYSCALL_FSTAT, (long)file_descriptor, (long)stat);
}

long linux_syscall_lstat(const char* path_name, Stat_* stat) {
    return linux_syscall(LINUX_SYSCALL_LSTAT, (long)path_name, (long)stat);
}

void* linux_syscall_mmap(void* addr, unsigned long length, int prot,
                   int flags, int file_descriptor, i64 offset) {
    return (void*)linux_syscall(LINUX_SYSCALL_MMAP, (long)addr, (long)length, (long)prot, (long)flags,
                   (long)file_descriptor, (long)offset);
}

int linux_syscall_munmap(void* start, size_t len) {
    return (int)linux_syscall(LINUX_SYSCALL_MUNMAP, (long)start, (long)len);
}

int linux_syscall_rt_sigprocmask(int how, const ulong* signal_set,
                           ulong* old_signal_set, ulong signal_set_size) {
    return (int)linux_syscall(LINUX_SYSCALL_RT_SIGPROCMASK, (long)how, (long)signal_set,
                        (long)old_signal_set, (long)signal_set_size);
}

void* linux_syscall_mremap(void* old_addr, unsigned long old_size,
                     unsigned long new_size, int flags, void* new_addr) {
    return (void*)linux_syscall(LINUX_SYSCALL_MREMAP, (long)old_addr, (long)old_size, (long)new_size, (long)flags,
                   (long)new_addr);
}

int linux_syscall_dup(int file_descriptor) {
    return (int)linux_syscall(LINUX_SYSCALL_DUP, (long)file_descriptor);
}

int linux_syscall_gettid() {
    return (int)linux_syscall(LINUX_SYSCALL_GETTID);
}

int linux_syscall_tkill(int thread_id, int signal) {
    return (int)linux_syscall(LINUX_SYSCALL_TKILL, (long)thread_id, (long)signal);
}

long linux_syscall_readlink(const char* restrict path_name, char* restrict buffer, unsigned long buffer_size) {
    return linux_syscall(LINUX_SYSCALL_READLINK, (long)path_name, (long)buffer, (long)buffer_size);
}

void linux_syscall_exit_group(int status) {
    linux_syscall(LINUX_SYSCALL_EXIT_GROUP, (long)status);
}

long linux_syscall_openat(const char* file_path, int file_descriptor, int flags, int mode) {
    return linux_syscall(LINUX_SYSCALL_OPENAT, (long)file_descriptor, (long)file_path, (long)flags, (long)mode);
}


void print(String s) {
    linux_syscall_write(1, s.data, s.length);
}

void error(String s) {
    linux_syscall_write(2, s.data, s.length);
}

void c_print_format(char* format, ...) {
    char buffer[1024];
    
    VariableArgumentList arg_list;
    variable_argument_begin(arg_list, format);
    int length = c_string_format_list(buffer, 1024, format, arg_list);
    variable_argument_end(arg_list);
    
    linux_syscall_write(1, buffer, length);
}

void c_error_format(char* format, ...) {
    char buffer[1024];
    
    VariableArgumentList arg_list;
    variable_argument_begin(arg_list, format);
    int length = c_string_format_list(buffer, 1024, format, arg_list);
    variable_argument_end(arg_list);
    
    linux_syscall_write(2, buffer, length);
}


_Noreturn
void exit(int status) {
    linux_syscall_exit_group(status);
    for(;;) {}
}


int raise_signal(int signal) {
    int thread_id;
    int result;
    ulong signal_set[8];
    static const unsigned long app_mask[] = {
        0x7fffffff, 0xfffffffc
    };
    linux_syscall_rt_sigprocmask(0 /*SIG_BLOCK*/, app_mask, signal_set, 8);
    thread_id = linux_syscall_gettid();
    result    = linux_syscall_tkill(thread_id, signal);
    linux_syscall_rt_sigprocmask(2 /*SIG_SETMASK*/, signal_set, 0, 8);
    return result;
}


void base_global_init() {
    base_global.page_size      = 4096;
    base_global.is_initialized = 1;
}

void* page_alloc(ulong num_pages) {
    if(!base_global.is_initialized) base_global_init();
    return linux_syscall_mmap(NULL, base_global.page_size * num_pages, MEMORY_FLAGS_READ | MEMORY_FLAGS_WRITE,
                              MEMORY_FLAGS_PRIVATE | MEMORY_FLAGS_ANONYMOUS, 0, 0);
}

void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) {
    if(!base_global.is_initialized) base_global_init();
    return linux_syscall_mremap(pages, base_global.page_size * old_num_pages,
                          base_global.page_size * new_num_pages, MEMORY_FLAGS_MAYMOVE, 0);
}


void page_dealloc(void* page, ulong num_pages) {
    if(!base_global.is_initialized) base_global_init();
    linux_syscall_munmap(page, num_pages * base_global.page_size);
}

void path_init_c(Path* path, char* name) {
    path->fd_       = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_open(name, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}

void path_create_c(Path* path, char* name) {
    path->fd_       = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_read_  = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_write_ = linux_syscall_open(name, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_create(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_create_c(path, name_c);

}

void path_init_from_c(Path* path, Path* dir, char* name) {
    path->fd_       = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_init_from(Path* path, Path* dir, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_init_from_c(path, dir, name_c);
}

void path_create_from_c(Path* path, Path* dir, char* name) {
    path->fd_       = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_read_  = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_write_ = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_create_from(Path* path, Path* dir, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_create_from_c(path, dir, name_c);
}

void path_init_working_directory(Path* path) {
    path_init_c(path, ".");
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->fd_ = linux_syscall_dup(orig->fd_);
    linux_syscall_fstat(copy->fd_, &copy->stat_);
    copy->fd_read_ = linux_syscall_dup(orig->fd_read_);
    copy->fd_write_ = linux_syscall_dup(orig->fd_write_);
}

Buffer path_read_file_aligned(Path* path, Arena* arena, int alignment) {
    size_t file_size = path->stat_.st_size;
    Buffer result = arena_alloc_buffer(arena, file_size, alignment);
    linux_syscall_read(path->fd_read_, result.begin, file_size);
    return result;
}

Buffer path_read_file(Path* path, Arena* arena) {
    return path_read_file_aligned(path, arena, 1);
}

void path_write_file(Path* path, Buffer buffer) {
    linux_syscall_write(path->fd_write_, buffer.begin, buffer.end - buffer.begin);
}

void path_close(Path* path) {
    if(path->fd_ != INVALID_FD) {
        linux_syscall_close(path->fd_);
    }
    if(path->fd_read_ != INVALID_FD) {
        linux_syscall_close(path->fd_read_);
    }
    if(path->fd_write_ != INVALID_FD) {
        linux_syscall_close(path->fd_write_);
    }
}

void path_navigate_c(Path* path, char* name) {
    int new_fd = linux_syscall_openat(name, path->fd_, OPEN_FLAGS_READ | OPEN_FLAGS_COMMON, 0);
    path_close(path);
    path->fd_ = new_fd;
    linux_syscall_fstat(path->fd_, &path->stat_);
    path->fd_read_  = linux_syscall_openat(name, path->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_openat(name, path->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
}

void path_navigate(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}

int path_is_directory(Path* path) {
    return path->stat_.st_mode == FILE_MODE_DIRECTORY;
}

ulong path_get_file_size(Path* path) {
    return path->stat_.st_size;
}

String path_get_name(Path* path, Arena* arena) {
    char proc_path_name[256];
    c_string_format(proc_path_name, 256, "/proc/self/fd/%i", path->fd_);
    Stat_ sb;
    int e;
    if((e = linux_syscall_lstat(proc_path_name, &sb)) < 0) {
        c_error_format("Error: lstat");
        exit(1);
    }
    int supposed_file_name_length = sb.st_size + 1;
    if(sb.st_size == 0) supposed_file_name_length = PATH_MAX;
    String result;
    result.data   = arena_alloc_array(arena, supposed_file_name_length, char);
    int length = linux_syscall_readlink(proc_path_name, result.data,
                             supposed_file_name_length);
    if(length == -1) {
        c_error_format("Error: readlink");
        exit(1);
    }
    result.length = length;
    return result;
}


String path_get_base_name(Path* path, Arena* arena) {
    String result;
    result.data = arena_alloc_array(arena, 256, char);
    Temp temp = temp_begin(arena);
    String path_name = path_get_name(path, arena);
    char* last_char = path_name.data + path_name.length - 1;
    char* last_slash = last_char;
    // If path_name == '/':
    if(*last_char == '/' && path_name.length == 1) {
        result.data[0] = '/';
        result.length  = 1;
        return result;
    }
    
    // If path_name ends in '/'s, do not include these '/':
    while(*last_char == '/') {
        last_char--;
        last_slash--;
    }

    //Search for the last slash:
    for(; last_slash >= path_name.data; last_slash--) {
        if(*last_slash == '/') {
            break;
        }
    }

    if(last_slash >= path_name.data - 1) {
        result.length = last_char - last_slash + 1;
        c_string_copy(result.data, last_slash + 1, result.length);
    } else {
        assert(0 && "This should not happen.");
        exit(1);
    }
    
    temp_end(temp);
    return result;
}

int dir_iter_next(DirIter* dir_iter) {
    struct dirent* de;
    do {
        de = readdir(dir_iter->dp_);
        if(!de) return 0;
        if(c_string_equal(de->d_name, "."))
            continue;
        if(c_string_equal(de->d_name, ".."))
            continue;
        break;
    } while(1);
    path_close(&dir_iter->path);
    dir_iter->path.fd_       = linux_syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    dir_iter->path.fd_read_  = linux_syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    dir_iter->path.fd_write_ = linux_syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(dir_iter->path.fd_, &dir_iter->path.stat_);
    return 1;
}

DirIter dir_iter_begin(Path* path) {
    DirIter result = { };
    result.dp_ = fdopendir(path->fd_);
    if(!result.dp_) {
        //c_error_format("%s\n", strerror(errno));
        exit(1);
    }
    result.dir_path = path;
    return result;
}

void dir_iter_end(DirIter* dir_iter) {
    path_close(&dir_iter->path);
    closedir(dir_iter->dp_);
}

#undef INVALID_FD



#endif //defined(LINUX)