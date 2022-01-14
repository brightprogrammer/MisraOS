#include "memory.h"

uint64_t get_usable_memory_size(uint64_t num_maps, struct stivale2_mmap_entry *mmap_entries){
    uint64_t total_size = 0;
    for(uint64_t i = 0; i < num_maps; i++){
        if(mmap_entries[i].type == STIVALE2_MMAP_USABLE){
            total_size += mmap_entries[i].length;
        }
    }

    return total_size;
}

void initialize_memory_manager(struct stivale2_struct_tag_memmap* mmap){}
