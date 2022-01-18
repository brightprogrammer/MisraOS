/**
 *@file gdt.c
 *@author Siddharth Mishra
 *@date 01/11/22
 *@brief global descriptor table function definitions
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "gdt.h"

// create default gdt
static __attribute__((aligned(0x1000))) GDT default_gdt;
// pointer to global_descriptor table
static GDTR gdtr;

// load gdt address in gdtr register
void loadGDTR(){
    // load gdtr
    asm volatile("lgdt %0"
                 :
                 : "m"(gdtr));
    // jump to code segment
    asm volatile("push $0x08\n" // offset to code segment
                 "lea 1f(%%rip), %%rax\n"
                 "push %%rax\n"
                 "lretq\n"
                 "1:\n" // 1: is an anonymous label, we will jump to this using the lretq instruction above
                 :
                 :
                 : "rax", "memory");
    // point to data segment
    asm volatile("mov %0, %%ds\n"
                 "mov %0, %%es\n"
                 "mov %0, %%gs\n"
                 "mov %0, %%fs\n"
                 "mov %0, %%ss\n"
                 :
                 : "a"((uint16_t)0x10)); // offset to data segment
}

GDTEntry createGDTEntry(uint8_t access, uint8_t attributes){
    GDTEntry gdtEntry;

    gdtEntry.segment_limit_low = 0;
    gdtEntry.base_address_low = 0;
    gdtEntry.base_address_middle = 0;
    gdtEntry.base_address_high = 0;
    gdtEntry.access_flags = access;
    gdtEntry.attributes = attributes;

    return gdtEntry;
}


// initializes global descriptor table
void initGDT(){
    // prepare pointer to gdt
    // minus 1 to get the last valid byte address in gdt
    gdtr.table_limit = sizeof(GDT) - 1;
    gdtr.table_base_address = (uint64_t)&default_gdt;

    // fill gdt:
    // createGDTEntry(access_flags, attributes/granularity)
    default_gdt.null = createGDTEntry(0x00, 0x00);
    default_gdt.kernel_code = createGDTEntry(0x9a, 0x20);
    default_gdt.kernel_data = createGDTEntry(0x92, 0x00);
    default_gdt.user_code = createGDTEntry(0xfa, 0x20);
    default_gdt.user_data = createGDTEntry(0xf2, 0x00);

    // reload gdt address in gdtr
    loadGDTR();
}
