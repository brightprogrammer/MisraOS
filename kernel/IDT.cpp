 /**
 *@file IDT.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/11/22
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

#include "IDT.hpp"

static idt_pointer_t idt_pointer;

void set_idt_entry_offset(idt_entry_t *idt_entry, uint64_t offset){
    idt_entry->offset_low = (uint16_t)(offset & 0xffff);
    idt_entry->offset_middle = (uint16_t)(offset & 0xffff0000);
    idt_entry->offset_high = (uint32_t)(offset & 0xffffffff00000000);
}

uint64_t get_idt_entry_offset(idt_entry_t* idt_entry){
    uint64_t offset = 0;
    offset = ((uint64_t)idt_entry->offset_high << 32) | ((uint64_t)idt_entry->offset_middle << 16) | ((uint64_t)idt_entry->offset_low);

    return offset;
}

void initialize_interrupt_descriptor_table(){
    idt_pointer.limit = 0x0fff;
    idt_pointer.offset = (uint64_t)0;
}
