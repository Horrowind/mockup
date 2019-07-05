void zero_buffer(Buffer buffer) {
    c_memset(buffer.begin, 0, buffer_length(buffer));
}

void copy_buffer(Buffer src, Buffer dest) {
    ulong length_src = buffer_length(src);
    ulong length_dest = buffer_length(dest);
    ulong length = min(length_src, length_dest);

    c_memcpy(dest.begin, src.begin, length);    
}

void* c_memcpy(void* restrict dest, const void* restrict src, unsigned long n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    u32 w, x;

    for (; (uptr)s % 4 && n; n--) *d++ = *s++;

    if ((uptr)d % 4 == 0) {
        for (; n>=16; s+=16, d+=16, n-=16) {
            *(u32 *)(d+0) = *(u32 *)(s+0);
            *(u32 *)(d+4) = *(u32 *)(s+4);
            *(u32 *)(d+8) = *(u32 *)(s+8);
            *(u32 *)(d+12) = *(u32 *)(s+12);
        }
        if (n&8) {
            *(u32 *)(d+0) = *(u32 *)(s+0);
            *(u32 *)(d+4) = *(u32 *)(s+4);
            d += 8; s += 8;
        }
        if (n&4) {
            *(u32 *)(d+0) = *(u32 *)(s+0);
            d += 4; s += 4;
        }
        if (n&2) {
            *d++ = *s++; *d++ = *s++;
        }
        if (n&1) {
            *d = *s;
        }
        return dest;
    }

    if (n >= 32) switch ((uptr)d % 4) {
    case 1:
        w = *(u32 *)s;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        n -= 3;
        for (; n>=17; s+=16, d+=16, n-=16) {
            x = *(u32 *)(s+1);
            *(u32 *)(d+0) = (w >> 24) | (x << 8);
            w = *(u32 *)(s+5);
            w = *(u32 *)(s+5);
            *(u32 *)(d+4) = (x >> 24) | (w << 8);
            x = *(u32 *)(s+9);
            *(u32 *)(d+8) = (w >> 24) | (x << 8);
            w = *(u32 *)(s+13);
            *(u32 *)(d+12) = (x >> 24) | (w << 8);
        }
        break;
    case 2:
        w = *(u32 *)s;
        *d++ = *s++;
        *d++ = *s++;
        n -= 2;
        for (; n>=18; s+=16, d+=16, n-=16) {
            x = *(u32 *)(s+2);
            *(u32 *)(d+0) = (w >> 16) | (x << 16);
            w = *(u32 *)(s+6);
            *(u32 *)(d+4) = (x >> 16) | (w << 16);
            x = *(u32 *)(s+10);
            *(u32 *)(d+8) = (w >> 16) | (x << 16);
            w = *(u32 *)(s+14);
            *(u32 *)(d+12) = (x >> 16) | (w << 16);
        }
        break;
    case 3:
        w = *(u32 *)s;
        *d++ = *s++;
        n -= 1;
        for (; n>=19; s+=16, d+=16, n-=16) {
            x = *(u32 *)(s+3);
            *(u32 *)(d+0) = (w >> 8) | (x << 24);
            w = *(u32 *)(s+7);
            *(u32 *)(d+4) = (x >> 8) | (w << 24);
            x = *(u32 *)(s+11);
            *(u32 *)(d+8) = (w >> 8) | (x << 24);
            w = *(u32 *)(s+15);
            *(u32 *)(d+12) = (x >> 8) | (w << 24);
        }
        break;
    }
    if (n&16) {
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
    }
    if (n&8) {
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
    }
    if (n&4) {
        *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
    }
    if (n&2) {
        *d++ = *s++; *d++ = *s++;
    }
    if (n&1) {
        *d = *s;
    }
    return dest;

    for (; n; n--) *d++ = *s++;
    return dest;
}


void* c_memset(void* dest, int c, ulong n) {
    uchar* s = dest;
    ulong k;

    /* Fill head and tail with minimal branching. Each
     * conditional ensures that all the subsequently used
     * offsets are well-defined and in the dest region. */

    if (!n) return dest;
    s[0] = c;
    s[n-1] = c;
    if (n <= 2) return dest;
    s[1] = c;
    s[2] = c;
    s[n-2] = c;
    s[n-3] = c;
    if (n <= 6) return dest;
    s[3] = c;
    s[n-4] = c;
    if (n <= 8) return dest;

    /* Advance pointer to align it at a 4-byte boundary,
     * and truncate n to a multiple of 4. The previous code
     * already took care of any head/tail that get cut off
     * by the alignment. */

    k = -(uptr)s & 3;
    s += k;
    n -= k;
    n &= -4;
    u32 c32 = ((u32)-1)/255 * (uchar)c;

    /* In preparation to copy 32 bytes at a time, aligned on
     * an 8-byte bounary, fill head/tail up to 28 bytes each.
     * As in the initial byte-based head/tail fill, each
     * conditional below ensures that the subsequent offsets
     * are valid (e.g. !(n<=24) implies n>=28). */

    *(u32 *)(s+0) = c32;
    *(u32 *)(s+n-4) = c32;
    if (n <= 8) return dest;
    *(u32 *)(s+4) = c32;
    *(u32 *)(s+8) = c32;
    *(u32 *)(s+n-12) = c32;
    *(u32 *)(s+n-8) = c32;
    if (n <= 24) return dest;
    *(u32 *)(s+12) = c32;
    *(u32 *)(s+16) = c32;
    *(u32 *)(s+20) = c32;
    *(u32 *)(s+24) = c32;
    *(u32 *)(s+n-28) = c32;
    *(u32 *)(s+n-24) = c32;
    *(u32 *)(s+n-20) = c32;
    *(u32 *)(s+n-16) = c32;

    /* Align to a multiple of 8 so we can fill 64 bits at a time,
     * and avoid writing the same bytes twice as much as is
     * practical without introducing additional branching. */

    k = 24 + ((uptr)s & 4);
    s += k;
    n -= k;

    /* If this loop is reached, 28 tail bytes have already been
     * filled, so any remainder when n drops below 32 can be
     * safely ignored. */

    u64 c64 = c32 | ((u64)c32 << 32);
    for (; n >= 32; n-=32, s+=32) {
        *(u64 *)(s+0) = c64;
        *(u64 *)(s+8) = c64;
        *(u64 *)(s+16) = c64;
        *(u64 *)(s+24) = c64;
    }
    return dest;
}




Buffer buffer(void* data, ulong length) {
    return (Buffer){
        .begin = data,
        .end   = data + length
    };
}

ulong buffer_length(Buffer buffer) {
    return buffer.end - buffer.begin;
}

Arena arena_create(ulong size) {
    void* data = get_memory(size);
    return (Arena){
        .buffer = buffer(data, size),
        .current = data
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
#if DEBUG
    c_print_format("Arena(%p): %p %p %p\n", a, a->buffer.begin, a->current, a->buffer.end);
#endif
    assert(a && a->current >= a->buffer.begin && a->current <= a->buffer.end);
    if(!a) return 0;
    align = align == 0 ? 0 : align - 1;
    void* result = ((void*)(((uptr)ptr_add(a->current, align))&~align));
    void* end = (void*)((char*)result + size); 
    assert(end < a->buffer.end);
    if(end >= a->buffer.end) return NULL;
    a->current = end;
    //debug_break;
    return result;
}

Buffer arena_alloc_buffer(Arena* a, ulong size, ulong align) {
    Buffer result;
    result.begin = arena_alloc(a, size, align);
    result.end   = (void*)((char*)result.begin + size);
    return result;
}

void* arena_dealloc(Arena* a, ulong size, ulong align) {
    assert(a && a->current >= a->buffer.begin && a->current <= a->buffer.end);
    if(!a) return NULL;
    align = align == 0 ? 0 : align - 1;
    void* result = ((void*)(((iptr)(ptr_sub(a->current, size))&~align)));
    assert(a->buffer.begin <= result);
    if(a->buffer.begin > result) return NULL;
    a->current = result;
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

void free_list_init(FreeList* free_list, Arena arena, ulong page_size) {
    free_list->arena = arena;
    free_list->sentinel.next = &free_list->sentinel;
    free_list->sentinel.prev = &free_list->sentinel;
    free_list->page_size = page_size;
}


void* free_list_alloc(FreeList* free_list, ulong size) {
    ulong num_pages = (size + free_list->page_size) / free_list->page_size;
    void* result = NULL;
    for(FreeListEntry* entry = free_list->sentinel.next;
        entry != &free_list->sentinel;
        entry = entry->next) {
        if(num_pages < entry->num_pages) {
            result = entry->ptr + num_pages * free_list->page_size;
            entry->num_pages -= num_pages;
            break;
        } else if(num_pages == entry->num_pages) {
            result = entry->ptr;
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
            break;
        }
    }
    if(!result) result = arena_alloc(&free_list->arena, free_list->page_size * num_pages, free_list->page_size);
    return result;
}

Buffer free_list_alloc_buffer(FreeList* free_list, ulong size) {
    Buffer result;
    result.begin = free_list_alloc(free_list, size);
    result.end   = (void*)((char*)result.begin + size);
    return result;
}

void free_list_dealloc(FreeList* free_list, void* ptr, ulong size) {
    ulong num_pages = (size + free_list->page_size) / free_list->page_size;
    FreeListEntry* entry = ptr;
    entry->ptr = ptr;
    entry->num_pages = num_pages;
    entry->prev = &free_list->sentinel;
    entry->next = free_list->sentinel.next;
    entry->prev->next = entry;
    entry->next->prev = entry;
}

void  free_list_dealloc_buffer(FreeList* free_list, Buffer buffer) {
    free_list_dealloc(free_list, buffer.begin, buffer_length(buffer));
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
