#if defined(LINUX)
#define INVALID_FD (-1)

#if defined(TCC)
long linux_syscall0(long n);
long linux_syscall1(long n, long a1);
long linux_syscall2(long n, long a1, long a2);
long linux_syscall3(long n, long a1, long a2, long a);
long linux_syscall4(long n, long a1, long a2, long a3, long a4);
long linux_syscall5(long n, long a1, long a2, long a3, long a4, long a5);
long linux_syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6);
#else
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
#endif //TCC

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
    LINUX_SYSCALL_GETDENTS       =  78,    
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
    FILE_MODE_MASK            = 0xf000,
    FILE_MODE_DIRECTORY       = 0x4000,
} LinuxFileMode;

enum {
    MEMORY_FLAGS_READ         =  1,
    MEMORY_FLAGS_WRITE        =  2,
    MEMORY_FLAGS_PRIVATE      =  2,
    MEMORY_FLAGS_ANONYMOUS    = 32,
    MEMORY_FLAGS_MAYMOVE      =  1,
} LinuxMemoryFlags;

static inline
long linux_syscall_read(int file_descriptor, void* buf, unsigned long count) {
    return linux_syscall(LINUX_SYSCALL_READ, (long)file_descriptor, (long)buf, (long)count);
}

static inline
long linux_syscall_write(int file_descriptor, const void* buf, unsigned long count) {
    return linux_syscall(LINUX_SYSCALL_WRITE, (long)file_descriptor, (long)buf, (long)count);
}

static inline
long linux_syscall_open(const char* file_path, int flags, int mode) {
    return linux_syscall(LINUX_SYSCALL_OPEN, (long)file_path, (long)flags, (long)mode);
}

static inline
int linux_syscall_close(int file_descriptor) {
    return (int)linux_syscall(LINUX_SYSCALL_CLOSE, (long)file_descriptor);
}

static inline
long linux_syscall_fstat(int file_descriptor, Stat_* stat) {
    return linux_syscall(LINUX_SYSCALL_FSTAT, (long)file_descriptor, (long)stat);
}

static inline
long linux_syscall_lstat(const char* path_name, Stat_* stat) {
    return linux_syscall(LINUX_SYSCALL_LSTAT, (long)path_name, (long)stat);
}

static inline
void* linux_syscall_mmap(void* addr, unsigned long length, int prot,
                   int flags, int file_descriptor, i64 offset) {
    return (void*)linux_syscall(LINUX_SYSCALL_MMAP, (long)addr, (long)length, (long)prot, (long)flags,
                   (long)file_descriptor, (long)offset);
}

static inline
int linux_syscall_munmap(void* start, ulong len) {
    return (int)linux_syscall(LINUX_SYSCALL_MUNMAP, (long)start, (long)len);
}

static inline
int linux_syscall_rt_sigprocmask(int how, const ulong* signal_set,
                           ulong* old_signal_set, ulong signal_set_size) {
    return (int)linux_syscall(LINUX_SYSCALL_RT_SIGPROCMASK, (long)how, (long)signal_set,
                        (long)old_signal_set, (long)signal_set_size);
}

static inline
void* linux_syscall_mremap(void* old_addr, ulong old_size,
                     unsigned long new_size, int flags, void* new_addr) {
    return (void*)linux_syscall(LINUX_SYSCALL_MREMAP, (long)old_addr, (long)old_size, (long)new_size, (long)flags,
                   (long)new_addr);
}

static inline
int linux_syscall_dup(int file_descriptor) {
    return (int)linux_syscall(LINUX_SYSCALL_DUP, (long)file_descriptor);
}

static inline
int linux_syscall_gettid() {
    return (int)linux_syscall(LINUX_SYSCALL_GETTID);
}


typedef struct {
    i64 d_ino;
    i64 d_off;
    ushort d_reclen;
    uchar d_type;
    char d_name[256];
} DirEntry_;

static inline
int linux_syscall_getdents(int fd, DirEntry_* buf, ulong len) {
    return (int)linux_syscall(LINUX_SYSCALL_GETDENTS, fd, (long)buf, len);
}

static inline
int linux_syscall_tkill(int thread_id, int signal) {
    return (int)linux_syscall(LINUX_SYSCALL_TKILL, (long)thread_id, (long)signal);
}

static inline
long linux_syscall_readlink(const char* restrict path_name, char* restrict buffer, unsigned long buffer_size) {
    return linux_syscall(LINUX_SYSCALL_READLINK, (long)path_name, (long)buffer, (long)buffer_size);
}

static inline
void linux_syscall_exit_group(int status) {
    linux_syscall(LINUX_SYSCALL_EXIT_GROUP, (long)status);
}

static inline
long linux_syscall_openat(const char* file_path, int file_descriptor, int flags, int mode) {
    return linux_syscall(LINUX_SYSCALL_OPENAT, (long)file_descriptor, (long)file_path, (long)flags, (long)mode);
}

void print(String s) {
    linux_syscall_write(1, s.data, s.length);
}

void error(String s) {
    linux_syscall_write(2, s.data, s.length);
}

void c_print(char* c) {
    linux_syscall_write(1, c, c_string_length(c));
}

void c_error(char* c) {
    linux_syscall_write(2, c, c_string_length(c));
}

no_return
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

void* get_memory(ulong size) {
    ulong num_pages = (size + 4095) / 4096;
    return linux_syscall_mmap(NULL, 4096 * num_pages, MEMORY_FLAGS_READ | MEMORY_FLAGS_WRITE,
                              MEMORY_FLAGS_PRIVATE | MEMORY_FLAGS_ANONYMOUS, 0, 0);
}

void path_init_c(Path* path, char* name) {
    path->fd_       = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_open(name, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

#define MAX_PATH_LENGTH 4096

void path_init(Path* path, String name) {
    char name_c[MAX_PATH_LENGTH];
    if(name.length >= MAX_PATH_LENGTH - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, MAX_PATH_LENGTH);
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
    char name_c[MAX_PATH_LENGTH];
    if(name.length >= MAX_PATH_LENGTH - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, MAX_PATH_LENGTH);
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
    char name_c[MAX_PATH_LENGTH];
    if(name.length >= MAX_PATH_LENGTH - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, MAX_PATH_LENGTH);
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
    char name_c[MAX_PATH_LENGTH];
    if(name.length >= MAX_PATH_LENGTH - 1) {
        c_error_format("File name \"%.*s\" is to long.\n", (int)name.length, name.data);
    }
    c_string_copy(name_c, name.data, MAX_PATH_LENGTH);
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
    ulong file_size = path->stat_.st_size;
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
    char name_c[MAX_PATH_LENGTH];
    if(name.length >= MAX_PATH_LENGTH - 1) {
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
    if(sb.st_size == 0) supposed_file_name_length = MAX_PATH_LENGTH;
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


static
int fdopendir_(int fd, DirStream_* dir) {
    Stat_ st;

    if(linux_syscall_fstat(fd, &st) < 0) {
        return -1;
    }
    if((st.st_mode & FILE_MODE_MASK) != FILE_MODE_DIRECTORY) {
        return -2;
    }

    //TODO:
    //fcntl(fd, F_SETFD, FD_CLOEXEC);
    dir->fd = fd;
    return 0;
}

static
DirEntry_* readdir_(DirStream_* dir) {
    DirEntry_* de;

    if (dir->buf_pos >= dir->buf_end) {
        int len = linux_syscall_getdents(dir->fd, (void*)dir->buf, sizeof(dir->buf));
        if (len <= 0) {
            //TODO:
            //if (len < 0 && len != -ENOENT) errno = -len;
            return NULL;
        }
        dir->buf_end = len;
        dir->buf_pos = 0;
    }
    de = (void *)(dir->buf + dir->buf_pos);
    dir->buf_pos += de->d_reclen;
    dir->tell = de->d_off;
    return de;
}

static
int closedir_(DirStream_* dir) {
    return linux_syscall_close(dir->fd);
}


int dir_iter_next(DirIter* dir_iter) {
    DirEntry_* de;
    do {
        de = readdir_(&dir_iter->dp_);
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
    if(fdopendir_(path->fd_, &result.dp_)) {
        //c_error_format("%s\n", strerror(errno));
        exit(1);
    }
    result.dir_path = path;
    return result;
}

void dir_iter_end(DirIter* dir_iter) {
    path_close(&dir_iter->path);
    closedir_(&dir_iter->dp_);
}

#if defined(BASE_GUI)
#include "dlfcn.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#if 0
void  gl_ActiveTexture(int slot);
void  gl_AttachShader(uint shader, uint program);
void  gl_BindBuffer(int target, uint buffer);
void  gl_BindTexture(int type, uint texture);
void  gl_BindVertexArray(uint vao);
void  gl_Begin(int mode);
void  gl_BlendEquation(int mode);
void  gl_BlendFunc(int sfactor, int dfactor);
void  gl_BufferData(int target, iptr length, const void * data, i32 usage);
void  gl_Clear(i32 mask);
void  gl_ClearColor(f32 r, f32 g, f32 b, f32 a);
void  gl_CompileShader(i32 shader);
i32   gl_CreateProgram(void);
i32   gl_CreateShader(i32 type);
void  gl_DeleteBuffers(int n, uint* buffers);
void  gl_DeleteProgram(i32 program);
void  gl_DeleteShader(i32 shader);
void  gl_DeleteTextures(int n, uint* textures);
void  gl_DetachShader(i32 program, i32 shader);
void  gl_Disable(i32 cap);
void  gl_DrawElements(i32 mode, i32 count, i32 type, void* offset);
void  gl_Enable(i32 cap);
void  gl_EnableVertexAttribArray(i32 index);
void  gl_End();
void  gl_GenBuffers(int n, uint* buffers);
void  gl_GenVertexArrays(int n, uint* arrays);
void  gl_GenTextures(int n, uint* textures);
i32   gl_GetAttribLocation(i32 program, const char* name);
void  gl_GetProgramiv(uint program, uint pname, int* params);
void  gl_GetShaderiv(uint shader, uint pname, int* params);
i32   gl_GetUniformLocation(i32 program, const char* name);
void  gl_LinkProgram(i32 program);
void* gl_MapBuffer(uint target, uint access);
void  gl_Scissor(i32 x, i32 y, i32 width, i32 height);
void  gl_ShaderSource(i32 shader, i32 count, const char** source, int* length);
void  gl_TexCoord2f(float s, float t);
void  gl_TexImage2D(i32 target, i32 level, i32 internalFormat, i32 width, i32 height, i32 border, i32 format, i32 type, void* data);
void  gl_TexParameteri(i32 target, i32 pname, i32 value);
void  gl_Uniform1i(i32 location, i32 value);
void  gl_UniformMatrix4fv(int  location, int  count, uchar transpose, float* value);
uchar gl_UnmapBuffer(uint target);
void  gl_UseProgram(i32 program);
void  gl_Vertex2f(float x, float y);
void  gl_VertexAttribPointer(i32 index, i32 size, i32 type, i32 normalized, i32 stride, const void* pointer);
void  gl_Viewport(i32 x, i32 y, i32 w, i32 h);


void gl_ActiveTexture(i32 slot) {
    glActiveTexture(slot);
}
void gl_AttachShader(uint shader, uint program) {
    glAttachShader(shader, program);
}
void  gl_Begin(int mode) {
    glBegin(mode);
}
void gl_BindBuffer(int target, uint buffer) {
    glBindBuffer(target, buffer);
}
void gl_BindTexture(int type, uint texture) {
    glBindTexture(type, texture);
}
void gl_BindVertexArray(uint vao) {
    glBindVertexArray(vao);
}
void gl_BlendEquation(i32 mode) {
    glBlendEquation(mode);
}
void gl_BlendFunc(i32 sfactor, i32 dfactor) {
    glBlendFunc(sfactor, dfactor);
}
void gl_BufferData(int target, iptr length, const void * data, i32 usage) {
    glBufferData(target, length, data, usage);
}
void gl_ClearBufferfv(i32 buffer, i32 drawbuffer, const float* value) {
    glClearBufferfv(buffer, drawbuffer, value);
}
void gl_CompileShader(i32 shader) {
    glCompileShader(shader);
}
i32 gl_CreateProgram(void) {
    return glCreateProgram();
}
i32 gl_CreateShader(i32 type) {
    return glCreateShader(type);
}
void gl_CullFace(i32 mode) {
    glCullFace(mode);
}
void gl_DeleteProgram(i32 program) {
    glDeleteProgram(program);
}
void gl_DeleteShader(i32 shader) {
    glDeleteShader(shader);
}
void gl_DetachShader(i32 program, i32 shader) {
    glDetachShader(program, shader);
}
void gl_Disable(i32 cap) {
    glDisable(cap);
}
void gl_DrawElements(i32 mode, i32 count, i32 type, void* offset) {
    glDrawElements(mode, count, type, offset);
}
void gl_Enable(i32 cap) {
    glEnable(cap);
}
void gl_EnableVertexAttribArray(i32 index) {
    glEnableVertexAttribArray(index);
}
void gl_End() {
    glEnd();
}
void gl_GenerateMipmap(i32 target) {
    glGenerateMipmap(target);
}
i32 gl_GetUniformLocation(i32 program, const char* name) {
    return glGetUniformLocation(program, name);
}
void gl_LinkProgram(i32 program) {
    glLinkProgram(program);
}
void gl_ShaderSource(i32 shader, i32 count, const char** source, int* length) {
    glShaderSource(shader, count, source, length);
}
void gl_TexParameterf(i32 target, i32 pname, f32 value) {
    glTexParameterf(target, pname, value);
}
void gl_TexParameteri(i32 target, i32 pname, i32 value) {
    glTexParameteri(target, pname, value);
}
void gl_TexStorage2D(i32 target, i32 levels, i32 internalformat, i32 width, i32 height) {
    glTexStorage2D(target, levels, internalformat, width, height);
}
void gl_TexSubImage2D(i32 target, i32 level, i32 xoff, i32 yoff, i32 width, i32 height, i32 format, i32 type, u8* data) {
    glTexSubImage2D(target, level, xoff, yoff, width, height, format, type, data);
}
void gl_Uniform1i(i32 location, i32 value) {
    glUniform1i(location, value);
}
void gl_Uniform3f(i32 location, f32 x, f32 y, f32 z) {
    glUniform3f(location, x, y, z);
}
void gl_UseProgram(i32 program) {
    glUseProgram(program);
}
void gl_VertexAttribPointer(i32 index, i32 size, i32 type, i32 normalized, i32 stride, const void* pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
void gl_Viewport(i32 x, i32 y, i32 w, i32 h) {
    glViewport(x, y, w, h);
}
i32  g_lGetAttribLocation(i32 program, const char* name) {
    return glGetAttribLocation(program, name);
}
void gl_Scissor(i32 x, i32 y, i32 width, i32 height) {
    glScissor(x, y, width, height);
}
void gl_Clear(i32 mask) {
    glClear(mask);
}
void gl_ClearColor(f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
}
void gl_TexCoord2f(float s, float t) {
    glTexCoord2f(s, t);
}
void gl_TexImage2D(i32 target, i32 level, i32 internalFormat, i32 width, i32 height, i32 border, i32 format, i32 type, void* data) {
    glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
}
void gl_DeleteTextures(int n, uint* textures) {
    glDeleteTextures(n, textures);
}
void gl_DeleteBuffers(int n, uint* buffers) {
    glDeleteBuffers(n, buffers);
}
void gl_GenBuffers(int n, uint* buffers) {
    glGenBuffers(n, buffers);
}
void gl_GenTextures(int n, uint* textures) {
    glGenTextures(n, textures);
}
void gl_GenVertexArrays(int n, uint* arrays) {
    glGenVertexArrays(n, arrays);
}
void gl_GetShaderiv(uint shader, uint pname, int* params) {
    glGetShaderiv(shader, pname, params);
}
void gl_GetProgramiv(uint program, uint pname, int* params) {
    glGetProgramiv(program, pname, params);
}
i32  gl_GetAttribLocation(i32 program, const char* name) {
    return glGetAttribLocation(program, name);
}
void* gl_MapBuffer(uint target, uint access) {
    return glMapBuffer(target, access);
}
void gl_UniformMatrix4fv(int location, int count, uchar transpose, float* value) {
    glUniformMatrix4fv(location, count, transpose, value);
}
uchar gl_UnmapBuffer(uint target) {
    return glUnmapBuffer(target);
}
void gl_Vertex2f(float x, float y) {
    glVertex2f(x, y);
}
#endif

#define GL_GET(name, ogl_name) name = (type_##name##_)dlsym(lib_gl, #ogl_name); \
    if(!name) error(L("[GL]: failed to load: " #name "\n"))

static
int has_extension(String str, String ext) {
    for(char* at = str.data; at < str.data + str.length - ext.length; at++) {
        if(string_equal(string(at, ext.length), ext)) return true;
    }
    return false;
}

int linux_load_opengl(OpenGLInfo *gl) {
    *gl = (OpenGLInfo){ };

    char* open_gl_library_names[] = {"libGL.so.1", "libGL.so"};
    void* lib_gl = NULL;
    
    for(ulong i = 0; i < array_length(open_gl_library_names); i++) {
        lib_gl = dlopen(open_gl_library_names[i], RTLD_NOW | RTLD_GLOBAL);
    }
    if(!lib_gl) {
        error(L("[GL]: Could not find libGL.so\n"));
        return 0;
    }

    GL_GET(gl_gen_textures, glGenTextures);
    GL_GET(gl_bind_texture, glBindTexture);
    GL_GET(gl_tex_parameter_int, glTexParameteri);
    GL_GET(gl_tex_image_2d, glTexImage2D);
    GL_GET(gl_enable, glEnable);
    GL_GET(gl_blend_equation, glBlendEquation);
    GL_GET(gl_blend_func, glBlendFunc);
    GL_GET(gl_disable, glDisable);
    GL_GET(gl_delete_textures, glDeleteTextures);
    GL_GET(gl_active_texture, glActiveTexture);
    GL_GET(gl_viewport, glViewport);
    GL_GET(gl_scissor, glScissor);
    GL_GET(gl_clear_color, glClearColor);
    GL_GET(gl_clear, glClear);
    GL_GET(gl_draw_elements, glDrawElements);
    GL_GET(gl_get_string, glGetString);
    GL_GET(gl_get_integer_vector, glGetIntegerv);
    
    int failed = 0;
    gl->version_str = string_from_c_string((char*)gl_get_string(GL_VERSION));
    gl_get_integer_vector(GL_MAJOR_VERSION, &gl->major_version);
    gl_get_integer_vector(GL_MINOR_VERSION, &gl->minor_version);
    if (gl->major_version < 2) {
        error(L("[GL]: Graphics card does not fullfill minimum OpenGL 2.0 support\n"));
        return 0;
    }
    gl->renderer_str     = string_from_c_string((char*)gl_get_string(GL_RENDERER));
    gl->glsl_version_str = string_from_c_string((char*)gl_get_string(GL_SHADING_LANGUAGE_VERSION));
    gl->extensions_str   = string_from_c_string((char*)gl_get_string(GL_EXTENSIONS));
    gl->vendor_str       = string_from_c_string((char*)gl_get_string(GL_VENDOR));

    if (string_equal(gl->vendor_str, L("ATI")) ||
        string_equal(gl->vendor_str, L("AMD"))) {
        gl->vendor = VENDOR_AMD;
    } else if (string_equal(gl->vendor_str, L("NVIDIA"))) {
        gl->vendor = VENDOR_NVIDIA;
    } else if (string_equal(gl->vendor_str, L("Intel"))) {
        gl->vendor = VENDOR_INTEL;
    } else gl->vendor = VENDOR_UNKNOWN;

    gl->vertex_buffer_obj_available   = has_extension(gl->extensions_str, L("GL_ARB_vertex_buffer_object"));
    gl->fragment_program_available    = has_extension(gl->extensions_str, L("GL_ARB_fragment_program"));
    gl->vertex_array_obj_available    = has_extension(gl->extensions_str, L("GL_ARB_vertex_array_object"));
    gl->frame_buffer_object_available = has_extension(gl->extensions_str, L("GL_ARB_framebuffer_object"));

#if 0
    GLInt n;
    gl_get_integer_vector(GL_NUM_EXTENSIONS, &n);
    for (GLInt i = 0; i < n; i++) {
        String extension = string_from_c_string((char*)gl_get_string_int(GL_EXTENSIONS, i));
        print(extension);
        print(L("\n"));
        if(string_equal(extension, L("GL_ARB_vertex_buffer_object"))) ...
    }
#endif    

    int version = 10 * gl->major_version + gl->minor_version;
    /* Extensions */
    gl->glsl_available = (version >= 20);
    if(gl->glsl_available) {
        /* GLSL core in OpenGL > 2 */
        GL_GET(gl_create_shader, glCreateShader);
        GL_GET(gl_shader_source, glShaderSource);
        GL_GET(gl_compile_shader, glCompileShader);
        GL_GET(gl_get_shader_int_vector, glGetShaderiv);
        GL_GET(gl_get_shader_info_log, glGetShaderInfoLog);
        GL_GET(gl_delete_shader, glDeleteShader);
        GL_GET(gl_create_program, glCreateProgram);
        GL_GET(gl_attach_shader, glAttachShader);
        GL_GET(gl_detach_shader, glDetachShader);
        GL_GET(gl_link_program, glLinkProgram);
        GL_GET(gl_use_program, glUseProgram);
        GL_GET(gl_get_program_int_vector, glGetProgramiv);
        GL_GET(gl_get_program_info_log, glGetProgramInfoLog);
        GL_GET(gl_delete_program, glDeleteProgram);
        GL_GET(gl_get_uniform_location, glGetUniformLocation);
        GL_GET(gl_get_attrib_location, glGetAttribLocation);
        GL_GET(gl_uniform1_int, glUniform1i);
        GL_GET(gl_uniform_matrix_4fv, glUniformMatrix4fv);
        GL_GET(gl_get_shader_info_log, glGetShaderInfoLog);
    }
    if(gl->vertex_buffer_obj_available) {
        /* GL_ARB_vertex_buffer_object */
        GL_GET(gl_gen_buffers, glGenBuffers);
        GL_GET(gl_bind_buffer, glBindBuffer);
        GL_GET(gl_buffer_data, glBufferData);
        GL_GET(gl_map_buffer, glMapBuffer);
        GL_GET(gl_unmap_buffer, glUnmapBuffer);
        GL_GET(gl_delete_buffers, glDeleteBuffers);
    }
    if (gl->fragment_program_available) {
        /* GL_ARB_vertex_program / GL_ARB_fragment_program  */
        GL_GET(gl_vertex_attrib_pointer, glVertexAttribPointer);
        GL_GET(gl_enable_vertex_attrib_array, glEnableVertexAttribArray);
        GL_GET(gl_disable_vertex_attrib_array, glDisableVertexAttribArray);
    }
    if(gl->vertex_array_obj_available) {
        /* GL_ARB_vertex_array_object */
        GL_GET(gl_gen_vertex_arrays, glGenVertexArrays);
        GL_GET(gl_bind_vertex_array, glBindVertexArray);
        GL_GET(gl_delete_vertex_arrays, glDeleteVertexArrays);
    }
    if(!gl->vertex_buffer_obj_available) {
        error(L("[GL] Error: GL_ARB_vertex_buffer_object is not available!\n"));
        failed = true;
    }
    if (!gl->fragment_program_available) {
        error(L("[GL] Error: GL_ARB_fragment_program is not available!\n"));
        failed = true;
    }
    if(!gl->vertex_array_obj_available) {
        error(L("[GL] Error: GL_ARB_vertex_array_object is not available!\n"));
        failed = true;
    }
    if(!gl->frame_buffer_object_available) {
        error(L("[GL] Error: GL_ARB_framebuffer_object is not available!\n"));
        failed = true;
    }

    return !failed;
}


struct XWindow {
};

static int gl_err = false;
static int gl_error_handler(Display *dpy, XErrorEvent *ev) {
    unused(dpy);
    unused(ev);
    error(L("Error!!!\n"));
    gl_err = true;
    return 0;
}

void gui_init(GuiContext* context, Arena* arena) {
    unused(gl_buffer_sub_data);

    /* Platform */
    PlatformWindow* win = &context->window;
    *win = (PlatformWindow){ 0 };

    win->dpy = XOpenDisplay(NULL);
    if (!win->dpy) {
        error(L("Failed to open X display\n"));
        exit(1);
    }

    {
        /* check glx version */
        int glx_major, glx_minor;
        if (!glXQueryVersion(win->dpy, &glx_major, &glx_minor)) {
            error(L("[X11]: Error: Failed to query OpenGL version\n"));
            exit(1);
        }
        if ((glx_major == 1 && glx_minor < 3) || (glx_major < 1)) {
            error(L("[X11]: Error: Invalid GLX version!\n"));
            exit(1);
        }
    }
    {
        /* find and pick matching framebuffer visual */
        int fb_count;
        static GLint attr[] = {
            GLX_X_RENDERABLE,   True,
            GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,    GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
            GLX_RED_SIZE,       8,
            GLX_GREEN_SIZE,     8,
            GLX_BLUE_SIZE,      8,
            GLX_ALPHA_SIZE,     8,
            GLX_DEPTH_SIZE,     24,
            GLX_STENCIL_SIZE,   8,
            GLX_DOUBLEBUFFER,   True,
            None
        };
        GLXFBConfig *fbc;
        fbc = glXChooseFBConfig(win->dpy, DefaultScreen(win->dpy), attr, &fb_count);
        if (!fbc) {
            error(L("[X11]: Error: failed to retrieve framebuffer configuration\n"));
            exit(1);
        }
        {
            /* pick framebuffer with most samples per pixel */
            int i;
            int fb_best = -1, best_num_samples = -1;
            for (i = 0; i < fb_count; ++i) {
                XVisualInfo *vi = glXGetVisualFromFBConfig(win->dpy, fbc[i]);
                if (vi) {
                    int sample_buffer, samples;
                    glXGetFBConfigAttrib(win->dpy, fbc[i], GLX_SAMPLE_BUFFERS, &sample_buffer);
                    glXGetFBConfigAttrib(win->dpy, fbc[i], GLX_SAMPLES, &samples);
                    if ((fb_best < 0) || (sample_buffer && samples > best_num_samples))
                        fb_best = i, best_num_samples = samples;
                }
            }
            win->fbc = fbc[fb_best];
            XFree(fbc);
            win->vis = glXGetVisualFromFBConfig(win->dpy, win->fbc);
        }
    }
    {
        /* create window */
        win->cmap = XCreateColormap(win->dpy, RootWindow(win->dpy, win->vis->screen), win->vis->visual, AllocNone);
        win->swa.colormap =  win->cmap;
        win->swa.background_pixmap = None;
        win->swa.border_pixel = 0;
        win->swa.event_mask =
            ExposureMask | KeyPressMask | KeyReleaseMask |
            ButtonPress | ButtonReleaseMask| ButtonMotionMask |
            Button1MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask|
            PointerMotionMask| StructureNotifyMask;
        win->win = XCreateWindow(win->dpy, RootWindow(win->dpy, win->vis->screen), 0, 0,
                                1024, 768, 0, win->vis->depth, InputOutput,
                                win->vis->visual, CWBorderPixel|CWColormap|CWEventMask, &win->swa);
        if (!win->win) {
            error(L("[X11]: Failed to create window\n"));
            exit(1);
        }
        XFree(win->vis);
        XStoreName(win->dpy, win->win, "Demo");
        XMapWindow(win->dpy, win->win);
        win->wm_delete_window = XInternAtom(win->dpy, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(win->dpy, win->win, &win->wm_delete_window, 1);
    }
    {
        /* create opengl context */
        typedef GLXContext(*glxCreateContext)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
        int(*old_handler)(Display*, XErrorEvent*) = XSetErrorHandler(gl_error_handler);
        String extensions_str = string_from_c_string((char*)glXQueryExtensionsString(win->dpy, DefaultScreen(win->dpy)));
        glxCreateContext create_context = (glxCreateContext)
            glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        gl_err = false;
        if (!has_extension(extensions_str, L("GLX_ARB_create_context")) || !create_context) {
            error(L("[X11]: glXCreateContextAttribARB() not found...\n"));
            error(L("[X11]: ... using old-style GLX context\n"));
            win->gl_context = glXCreateNewContext(win->dpy, win->fbc, GLX_RGBA_TYPE, 0, True);
        } else {
            GLint attr[] = {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                None
            };
            win->gl_context = create_context(win->dpy, win->fbc, 0, True, attr);
            XSync(win->dpy, False);
            if (gl_err || !win->gl_context) {
                /* Could not create GL 3.0 context. Fallback to old 2.x context.
                 * If a version below 3.0 is requested, implementations will
                 * return the newest context version compatible with OpenGL
                 * version less than version 3.0.*/
                attr[1] = 1; attr[3] = 0;
                gl_err = false;
                error(L("[X11] Failed to create OpenGL 3.0 context\n"));
                error(L("[X11] ... using old-style GLX context!\n"));
                win->gl_context = create_context(win->dpy, win->fbc, 0, True, attr);
            }
        }
        XSync(win->dpy, False);
        XSetErrorHandler(old_handler);
        if (gl_err || !win->gl_context) {
            error(L("[X11]: Failed to create an OpenGL context\n"));
            exit(1);
        }
        glXMakeCurrent(win->dpy, win->win, win->gl_context);
    }

    OpenGLInfo ogl_info;
    linux_load_opengl(&ogl_info);
    gui_context_init(context, arena);
    context->running = 1;
}

void gui_deinit(GuiContext* context) {
    PlatformWindow* win = &context->window;
    gui_context_deinit(context);
    glXMakeCurrent(win->dpy, 0, 0);
    glXDestroyContext(win->dpy, win->gl_context);
    XUnmapWindow(win->dpy, win->win);
    XFreeColormap(win->dpy, win->cmap);
    XDestroyWindow(win->dpy, win->win);
    XCloseDisplay(win->dpy);
}

void gui_begin_frame(GuiContext* context) {
    PlatformWindow* win = &context->window;
    while (XPending(win->dpy)) {
        XEvent evt;
        XNextEvent(win->dpy, &evt);
        if (evt.type == ClientMessage) context->running = 0;
        if (XFilterEvent(&evt, win->win)) continue;
        if (evt.type == KeyPress || evt.type == KeyRelease) {
            /* Key handler */
            int ret, down = (evt.type == KeyPress);
            KeySym *code = XGetKeyboardMapping(win->dpy, (KeyCode)evt.xkey.keycode, 1, &ret);
            if (*code == XK_Shift_L || *code == XK_Shift_R) mu_input_key(context, GUI_KEY_SHIFT, down);
            //else if (*code == XK_Delete)    mu_input_key(context, MU_KEY_DEL, down);
            else if (*code == XK_Return)    mu_input_key(context, GUI_KEY_RETURN, down);
            //else if (*code == XK_Tab)       mu_input_key(context, MU_KEY_TAB, down);
            //else if (*code == XK_Left)      mu_input_key(context, MU_KEY_LEFT, down);
            //else if (*code == XK_Right)     mu_input_key(context, MU_KEY_RIGHT, down);
            //else if (*code == XK_Up)        mu_input_key(context, MU_KEY_UP, down);
            //else if (*code == XK_Down)      mu_input_key(context, MU_KEY_DOWN, down);
            else if (*code == XK_BackSpace) mu_input_key(context, GUI_KEY_BACKSPACE, down);
            //else if (*code == XK_Escape)    mu_input_key(context, MU_KEY_TEXT_RESET_MODE, down);
            //else if (*code == XK_Page_Up)   mu_input_key(context, MU_KEY_SCROLL_UP, down);
            //else if (*code == XK_Page_Down) mu_input_key(context, MU_KEY_SCROLL_DOWN, down);
            /*else if (*code == XK_Home) {
                mu_input_key(context, MU_KEY_TEXT_START, down);
                mu_input_key(context, MU_KEY_SCROLL_START, down);
            } else if (*code == XK_End) {
                mu_input_key(context, MU_KEY_TEXT_END, down);
               mu_input_key(context, MU_KEY_SCROLL_END, down);
            } else {
                if (*code == 'c' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_COPY, down);
                else if (*code == 'v' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_PASTE, down);
                else if (*code == 'x' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_CUT, down);
                else if (*code == 'z' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_TEXT_UNDO, down);
                else if (*code == 'r' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_TEXT_REDO, down);
                else if (*code == XK_Left && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_TEXT_WORD_LEFT, down);
                else if (*code == XK_Right && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_TEXT_WORD_RIGHT, down);
                else if (*code == 'b' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_TEXT_LINE_START, down);
                else if (*code == 'e' && (evt.xkey.state & ControlMask))
                    mu_input_key(context, MU_KEY_TEXT_LINE_END, down);
                else {
                    if (*code == 'i')
                        mu_input_key(context, MU_KEY_TEXT_INSERT_MODE, down);
                    else if (*code == 'r')
                        mu_input_key(context, MU_KEY_TEXT_REPLACE_MODE, down);
                    if (down) {
                        char buf[32];
                        KeySym keysym = 0;
                        if (XLookupString((XKeyEvent*)evt, buf, 32, &keysym, NULL) != NoSymbol)
                            nk_input_glyph(context, buf);
                    }
               }
               }*/
            XFree(code);
        } else if (evt.type == ButtonPress || evt.type == ButtonRelease) {
            /* Button handler */
            int down = (evt.type == ButtonPress);
            const int x = evt.xbutton.x, y = evt.xbutton.y;
            if (evt.xbutton.button == Button1) {
                if (down) {
                    mu_input_mousedown(context, x, y, GUI_MOUSE_BUTTON_LEFT);
                } else {
                    mu_input_mouseup(context, x, y, GUI_MOUSE_BUTTON_LEFT);
                }
            } else if (evt.xbutton.button == Button2) {
                if (down) {
                    mu_input_mousedown(context, x, y, GUI_MOUSE_BUTTON_MIDDLE);
                } else {
                    mu_input_mouseup(context, x, y, GUI_MOUSE_BUTTON_MIDDLE);
                }
            } else if (evt.xbutton.button == Button3) {
                if (down) {
                    mu_input_mousedown(context, x, y, GUI_MOUSE_BUTTON_RIGHT);
                } else {
                    mu_input_mouseup(context, x, y, GUI_MOUSE_BUTTON_RIGHT);
                }
            } else if(evt.xbutton.button == Button4) {
                mu_input_mousewheel(context, 1);
            } else if(evt.xbutton.button == Button5) {
                mu_input_mousewheel(context, -1);
            }
        } else if (evt.type == MotionNotify) {
            /* Mouse motion handler */
            const int x = evt.xmotion.x, y = evt.xmotion.y;
            mu_input_mousemove(context, x, y);
        }
    }
    gui_begin_frame_opengl(context);
}


void gui_end_frame(GuiContext* context) {
    PlatformWindow* win = &context->window;
    gui_end_frame_opengl(context);
    glXSwapBuffers(win->dpy, win->win);
}

void gui_set_window_name(GuiContext* context, String window_name) {
    PlatformWindow* win = &context->window;
    char buffer[256] = { 0 };
    c_string_copy(buffer, window_name.data, 256);
    XStoreName(win->dpy, win->win, buffer);
}

void gui_get_window_size(GuiContext* context, int* width, int* height) {
    PlatformWindow* win = &context->window;
    XWindowAttributes attr;
    XGetWindowAttributes(win->dpy, win->win, &attr);
    *width = attr.width;
    *height = attr.height;
}

void gui_get_framebuffer_size(GuiContext* context, int* width, int* height) {
    // TODO: Is this correct?
    PlatformWindow* win = &context->window;
    XWindowAttributes attr;
    XGetWindowAttributes(win->dpy, win->win, &attr);
    *width = attr.width;
    *height = attr.height;
}

b32 gui_should_continue(GuiContext* context) {
    return context->running;
}


#if 0
void get_cursor_position(GuiContext* context, int* x, int* y) {
    double xpos;
    double ypos;
    glfwGetCursorPos(context->window.handle, &xpos, &ypos);
    *x = xpos;
    *y = ypos;
}

KeyState get_key(GuiContext* context, Key key_code) {
    return glfwGetKey(context->window.handle, key_code);
}

KeyState get_mouse_button(GuiContext* context, int button) {
    return glfwGetMouseButton(context->window.handle, button);
}
#endif

extern int init(int argc, char** argv);
extern int update();
extern int deinit();
int main(int argc, char** argv) {
    int result = init(argc, argv);
    if(!result) return result;
    while(update());
    return deinit();
}

#endif //BASE_GUI

#undef MAX_PATH_LENGTH
#undef INVALID_FD

#endif //defined(LINUX)