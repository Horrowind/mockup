#include "freespace.h"

void freespace_manager_init(freespace_manager_t* manager) {
    manager->freespace_list_sentinal.next = &manager->freespace_list_sentinal;
    manager->freespace_list_sentinal.prev = &manager->freespace_list_sentinal;
}

void freespace_manager_deinit(freespace_manager_t* manager) {
    freespace_chunk_t* chunk = manager->freespace_list_sentinal.next;
    while(chunk != &manager->freespace_list_sentinal) {
        freespace_chunk_t* next = chunk->next;
        free(chunk);
        chunk = next;
    }
}

void freespace_manager_empty(freespace_manager_t* manager) {
    freespace_chunk_t* chunk = manager->freespace_list_sentinal.next;
    while(chunk != &manager->freespace_list_sentinal) {
        chunk->fill = 0;
        chunk = chunk->next;
    }
}
    
u32 freespace_manager_request(freespace_manager_t* manager, u32 requested_size) {
    u32 freespace_addr = 0;
    freespace_chunk_t* chunk = manager->freespace_list_sentinal.next;
    while(chunk != &manager->freespace_list_sentinal) {
        if(chunk->size - chunk->fill >= requested_size) {
            freespace_addr = chunk->address + chunk->fill;
            chunk->fill += requested_size;
            break;
        }
        chunk = chunk->next;
    }
    return freespace_addr;
}

int  freespace_manager_add_freespace(freespace_manager_t* manager, u32 address, u32 size) {
    freespace_chunk_t* new_chunk = malloc(sizeof(freespace_chunk_t));
    new_chunk->address = address;
    new_chunk->size = size;
    new_chunk->fill = 0;
    new_chunk->next = &manager->freespace_list_sentinal;
    new_chunk->prev = manager->freespace_list_sentinal.prev;
    manager->freespace_list_sentinal.prev->next = new_chunk;
    manager->freespace_list_sentinal.prev = new_chunk;
    return 1;
}
