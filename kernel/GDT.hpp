#/**
 *@file gdt.h
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

#ifndef GDT_HPP
#define GDT_HPP

#include <cstdint>
#include "Common.hpp"

// struct to represent gdtr
struct GDTR{
    uint16_t table_limit;
    uint64_t table_base_address;
} PACKED_STRUCT;

// implementation of a general segment descriptor
struct GDTEntry {
    // segment limit is divided into 2 parts
    // fist part is bit 0 to 15 in first dword
    // second pat is bit 16 to 19 in second dword
    uint16_t segment_limit_low;

    // base address is divided into 3 parts
    // first part is bit 16 to 31 in first dword
    // second part is bit 0 to 7 in second dword
    // third and final part is from bit 24 to 32 in second dword
    uint16_t base_address_low;
    uint8_t base_address_middle;

    // bit 0 to 3 is for type field
    // bit 4 is S bit (user segment or system segment)
    // bits 5 and 6 make up the DPL (descriptor privilege level) field
    // bit 7 is P bit (present bit)
    uint8_t access_flags;

    // this field contains the higher 4 bits of segment limit (bits 0:3)
    // then there is the AVL Bit (bit 4)
    // then a reserved bit (bit 5)
    // then a D/B bit (bit 6)
    // and at last the Granularity (G) bit (bit 7)
    uint8_t attributes;

    // finally the higher part of base_address
    uint8_t base_address_high;
} PACKED_STRUCT;

struct GDT{
    GDTEntry null;
    GDTEntry kernelCode;
    GDTEntry kernelData;
    GDTEntry userCode;
    GDTEntry userData;
} PACKED_STRUCT __attribute__((aligned(0x1000)));

// install kernel's global descriptor table in gdtr
void InstallGDT();

#endif // GDT_HPP
