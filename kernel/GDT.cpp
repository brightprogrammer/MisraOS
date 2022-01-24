/**
 *@file gdt.c
 *@author Siddharth Mishra
 *@date 01/11/22
 *@brief global descriptor table function definitions
 *@copyright BSD 3-Clause License

 Copyright (c) 2022, Siddharth Mishra
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

    // for(;;){
    //     asm volatile("hlt");
    // }

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

    // fill gdt:
    // createGDTEntry(access_flags, attributes/granularity)
    // null descriptor has all fields set to 0 (null)
    default_gdt.null = CreateGDTEntry(0x00, 0x00);
    default_gdt.kernel_code = CreateGDTEntry(0x9b, 0x20);
    default_gdt.kernel_data = CreateGDTEntry(0x92, 0x00);
    default_gdt.user_code = CreateGDTEntry(0xfb, 0x20);
    default_gdt.user_data = CreateGDTEntry(0xf2, 0x00);

    // reload gdt address in gdtr
    LoadGDTR();
}
