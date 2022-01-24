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

#ifndef IDT_HPP
#define IDT_HPP

#include <stdint.h>

// type attr field structure
// | 7 | 6,5 | 4 | 3,2,1,0  |
// | P | DPL | S | GateType |
// p bit mist always be true if this gate is a valid gate!
#define IDT_TYPE_ATTR_INTERRUPT_GATE 0b10001110
#define IDT_TYPE_ATTR_CALL_GATE 0b10001100
#define IDT_TYPE_ATTR_TRAP_GATE 0b10001111

// idt entry structure
// interrupt gate and trap gate have same structure
// https://wiki.osdev.org/Interrupt_Descriptor_Table
typedef struct {
    uint16_t offsetLow;
    uint16_t selector; // switch to the "selected" gdt or ldt segment segment
    uint8_t ist;

    /*
     --- examples of typeAttr value ---
     64-bit Interrupt Gate: 0x8E (p=1, dpl=0b00, type=0b1110 => type_attributes=0b1000_1110=0x8E)
     64-bit Trap Gate: 0x8F (p=1, dpl=0b00, type=0b1111 => type_attributes=1000_1111b=0x8F)

     --- examples of type values ---
     0b1110 or 0xE: 64-bit Interrupt Gate
     0b1111 or 0xF: 64-bit Trap Gate
     */
    uint8_t typeAttr;

    uint16_t offsetMiddle;
    uint32_t offsetHigh;

    // this is reserved for future use
    uint32_t reserved;

    void SetOffset(uint64_t offset);
    uint64_t GetOffset();
} __attribute__((packed)) IDTEntry;

// idtr register structure
typedef struct {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed)) IDTR;

// you know what this does!
void InstallIDT();

#endif // IDT_HPP
