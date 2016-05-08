#ifndef MOCKUP_FREESPACE_H
#define MOCKUP_FREESPACE_H

typedef struct freespace_chunk {
    u32 address;
    u32 size;
    u32 fill;
    struct freespace_chunk* next;
    struct freespace_chunk* prev;
} freespace_chunk_t;

typedef struct {
    freespace_chunk_t freespace_list_sentinal;
} freespace_manager_t;

void freespace_manager_init(freespace_manager_t* manager);
void freespace_manager_deinit(freespace_manager_t* manager);
void freespace_manager_empty(freespace_manager_t* manager);
u32  freespace_manager_request(freespace_manager_t* manager, u32 size);
int  freespace_manager_add_freespace(freespace_manager_t* manager, u32 address, u32 size);

// TODO: freespace_manager_t is currently completely ROM agnostic. Missing things:
//  * check bank boundaries
//  * check for overlapping freespace pools
//  * merge contigous freespace pools

#endif //MOCKUP_FREESPACE_H
