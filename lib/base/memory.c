Buffer buffer(void* data, ulong length) {
    return (Buffer){
        .begin = data,
        .end   = data + length
    };
}

ulong buffer_length(Buffer buffer) {
    return buffer.end - buffer.begin;
}

void zero_buffer(Buffer buffer) {
    __builtin_memset(buffer.begin, 0, buffer_length(buffer));
}

void copy_buffer(Buffer src, Buffer dest) {
    ulong length_src = buffer_length(src);
    ulong length_dest = buffer_length(dest);
    ulong length = min(length_src, length_dest);

    __builtin_memcpy(dest.begin, src.begin, length);    
}

ulong size_to_pages(ulong size) {
    if(!base_global.is_initialized) base_global_init();
    return (size + base_global.page_size - 1) / base_global.page_size;
}


Arena arena_create(ulong size) {
    if(!base_global.is_initialized) base_global_init();
    Buffer buffer = {
        .begin = page_alloc(size_to_pages(size)),
        .end   = (void*)((char*)buffer.begin + size)
    };
    return (Arena){
        .buffer = buffer,
        .current = buffer.begin
    };
}

Arena arena_create_buffer(Buffer buffer) {
    return (Arena){
        .buffer = buffer,
        .current = buffer.begin
    };
}

void arena_clear(Arena* a) {
    a->current = a->buffer.begin;
}

void* arena_alloc(Arena* a, ulong size, ulong align) {
    assert(a && a->current >= a->buffer.begin && a->current <= a->buffer.end);
    if(!a) return 0;
    align = align == 0 ? 0 : align - 1;
    void* result = ((void*)(((iptr)((char*)a->current + align)&~align)));
    void* end = (void*)((char*)result + size); 
    assert(end < a->buffer.end);
    if(end >= a->buffer.end) return NULL;
    a->current = end;
    return result;
}

Buffer arena_alloc_buffer(Arena* a, ulong size, ulong align) {
    Buffer result;
    result.begin = arena_alloc(a, size, align);
    result.end   = (void*)((char*)result.begin + size);
    return result;
}

Arena arena_subarena(Arena* a, ulong size) {
    void* data = arena_alloc(a, size, alignof(void*));
    Arena result = {
        .buffer.begin = data,
        .buffer.end   = data + size,
        .current      = data
    };
    return result;
}

void free_list_init(FreeList* free_list_sentinel) {
    free_list_sentinel->next = free_list_sentinel;
    free_list_sentinel->prev = free_list_sentinel;
}

void* free_list_alloc(FreeList* free_list_sentinel, ulong num_pages) {
    if(!base_global.is_initialized) base_global_init();
    void* result = NULL;
    for(FreeList* entry = free_list_sentinel->next;
        entry != free_list_sentinel;
        entry = entry->next) {
        if(num_pages < entry->num_pages) {
            result = entry->ptr + num_pages * base_global.page_size;
            entry->num_pages -= num_pages;
            break;
        } else if(num_pages == entry->num_pages) {
            result = entry->ptr;
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
            break;
        }
    }
    if(!result) result = page_alloc(num_pages);
    return result;
}

void free_list_add(FreeList* free_list_sentinel, void* ptr, ulong num_pages) {
    FreeList* entry = ptr;
    entry->ptr = ptr;
    entry->num_pages = num_pages;
    entry->prev = free_list_sentinel;
    entry->next = free_list_sentinel->next;
    entry->prev->next = entry;
    entry->next->prev = entry;
}

Temp temp_begin(Arena* a) {
    return (Temp) {
        .arena = a,
        .old_current = a->current
    };
}

void temp_end(Temp temp) {
    temp.arena->current = temp.old_current;
}
