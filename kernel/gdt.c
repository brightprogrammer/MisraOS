/**
 *@file gdt.c
 *@author Siddharth Mishra
 *@date 01/11/22
 *@brief global descriptor table function definitions
 **/

#include "gdt.h"

// create default gdt
__attribute__((aligned(0x1000))) gdt_t default_gdt;
// pointer to global_descriptor table
gdt_pointer_t gdt_ptr;

// load gdt address in gdtr register
void load_gdt(gdt_pointer_t* gdt_ptr){
    asm volatile("lgdt %0": :"m"(gdt_ptr));
}

gdt_entry_t create_gdt_entry(uint8_t access, uint8_t attributes){
    gdt_entry_t gdt_entry;

    gdt_entry.segment_limit_low = 0;
    gdt_entry.base_address_low = 0;
    gdt_entry.base_address_middle = 0;
    gdt_entry.base_address_high = 0;
    gdt_entry.access_flags = access;
    gdt_entry.attributes = attributes;

    return gdt_entry;
}


// initializes global descriptor table
void initialize_global_descriptor_table(){
    // prepare pointer to gdt
    // minus 1 to get the last valid byte address in gdt
    gdt_ptr.table_limit = sizeof(gdt_t) - 1;
    gdt_ptr.table_base_address = (uint64_t)&default_gdt;

    // fill gdt
    default_gdt.null = create_gdt_entry(0x00, 0x00);
    default_gdt.kernel_code = create_gdt_entry(0x9a, 0xa0);
    default_gdt.kernel_data = create_gdt_entry(0x92, 0xa0);
    default_gdt.user_null = create_gdt_entry(0x00, 0x00);
    default_gdt.user_code = create_gdt_entry(0x9a, 0xa0);
    default_gdt.user_data = create_gdt_entry(0x92, 0xa0);

    // reload gdt address in gdtr
    load_gdt(&gdt_ptr);
}