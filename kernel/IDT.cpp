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
#include "PhysicalMemoryManager.hpp"
#include "VirtualMemoryManager.hpp"
#include "Interrupts.hpp"
#include "Printf.hpp"

#define IDT_ENTRY_OFFSET_LOW_MASK uint64_t(0xffff)
#define IDT_ENTRY_OFFSET_MIDDLE_MASK uint64_t(0xffff0000)
#define IDT_ENTRY_OFFSET_HIGH_MASK uint64_t(0xffffffff00000000)

static IDTR idtr;

// set offset in this idt entry
void IDTEntry::SetOffset(uint64_t offset){
    offsetLow = uint16_t(offset & IDT_ENTRY_OFFSET_LOW_MASK);
    offsetMiddle = uint16_t((offset & IDT_ENTRY_OFFSET_MIDDLE_MASK) >> 16);
    offsetHigh = uint32_t((offset & IDT_ENTRY_OFFSET_HIGH_MASK) >> 32);
}

// get offset from this idt entry
uint64_t IDTEntry::GetOffset(){
    uint64_t offset = 0;

    offset = (uint64_t(offsetHigh) << 32) |
        (uint64_t(offsetMiddle) << 16) |
        uint64_t(offsetLow);

    return offset;
}

void InstallIDT(){
    idtr.limit = PAGE_SIZE - 1;
    idtr.offset = PhysicalMemoryManager::AllocatePage();

    // create page fault handler
    // note that we're actually allocating the interrupt gate here at 0xe offset
    // 0xe offset is for page fault interrupt
    IDTEntry* pageFaultInt = reinterpret_cast<IDTEntry*>(idtr.offset + 0x0e * sizeof(IDTEntry));
    pageFaultInt->SetOffset(reinterpret_cast<uint64_t>(PageFaultHandler));
  //  uint64_t off = pageFaultInt->GetOffset();
    pageFaultInt->typeAttr = IDT_TYPE_ATTR_INTERRUPT_GATE;
    pageFaultInt->selector = 0x08; // offset of kernelCode in GDT

    asm volatile ("lidt %0"
                  :
                  : "m"(idtr));

//    while(true) asm("hlt");
}
