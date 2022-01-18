/**
 *@file gdt.c
 *@author Siddharth Mishra
 *@date 01/11/22
 *@brief global descriptor table function definitions
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "GDT.hpp"

// create default gdt
static GDT __attribute__((aligned(0x1000))) default_gdt;
// pointer to global_descriptor table
static GDTR gdtr;

// load gdt address in gdtr register
void LoadGDTR(){
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

// create a new gdt entry using given values
// in long mode, all fields are ignored, except these two.
GDTEntry CreateGDTEntry(uint8_t access, uint8_t attributes){
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
void InstallGDT(){
    // prepare pointer to gdt
    // minus 1 to get the last valid byte address in gdt
    gdtr.table_limit = sizeof(GDT) - 1;
    gdtr.table_base_address = (uint64_t)&default_gdt;

    // // fill gdt:
    // // createGDTEntry(access_flags, attributes/granularity)
    // // null descriptor has all fields set to 0 (null)
    // default_gdt.null = CreateGDTEntry(0x00, 0x00);
    // // if code segment is conforming then code segments that don't have the same dpl
    // // won't be able to run the conforming code!
    // // access flags : present-bit = 1, descriptor-privilege-level = 0, conforming-bit = 0
    // // attributes : d-bit = 0, l-bit = 1
    // // l-bit decides whether the program is running in 64 bit mode or not
    // // for now d-bit must always be set to 0 (acc. to amd64 manuals)
    // default_gdt.kernel_code = CreateGDTEntry(0x90, 0x20);
    // // access flags : present-bit = 1, rest other fields are ignored
    // // attributes : ignored
    // default_gdt.kernel_data = CreateGDTEntry(0xa0, 0x00);
    // // access flags : present-bit = 1, descriptor-privilege-level = 3, conforming-bit = 0
    // // attributes : d-bit = 0, l-bit = 1
    // default_gdt.user_code = CreateGDTEntry(0xf8, 0x20);
    // // same as kernel's data segment descriptor
    // default_gdt.user_data = CreateGDTEntry(0xa0, 0x00);

    // fill gdt:
    // createGDTEntry(access_flags, attributes/granularity)
    // null descriptor has all fields set to 0 (null)
    default_gdt.null = CreateGDTEntry(0x00, 0x00);
    // if code segment is conforming then code segments that don't have the same dpl
    // won't be able to run the conforming code!
    // access flags : present-bit = 1, descriptor-privilege-level = 0, conforming-bit = 0
    // attributes : d-bit = 0, l-bit = 1
    // l-bit decides whether the program is running in 64 bit mode or not
    // for now d-bit must always be set to 0 (acc. to amd64 manuals)
    default_gdt.kernel_code = CreateGDTEntry(0x9a, 0x20);
    // access flags : present-bit = 1, rest other fields are ignored
    // attributes : ignored
    default_gdt.kernel_data = CreateGDTEntry(0x90, 0x00);
    // access flags : present-bit = 1, descriptor-privilege-level = 3, conforming-bit = 0
    // attributes : d-bit = 0, l-bit = 1
    default_gdt.user_code = CreateGDTEntry(0xfa, 0x20);
    // same as kernel's data segment descriptor
    default_gdt.user_data = CreateGDTEntry(0x90, 0x00);

    // reload gdt address in gdtr
    LoadGDTR();
}
