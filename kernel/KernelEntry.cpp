/**
 *@file KernelEntry.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief defines kernel's entry point
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

#include "Renderer/Framebuffer.hpp"
#include "Renderer/FontRenderer.hpp"
#include "PhysicalMemoryManager.hpp"
#include "VirtualMemoryManager.hpp"
#include "Utils/String.hpp"
#include "Printf.hpp"
#include "IDT.hpp"
#include "GDT.hpp"
#include "Interrupts.hpp"
#include "IO.hpp"
#include "ACPI.hpp"
#include "Puts.hpp"
#include "Common.hpp"

// The following will be our kernel's entry point.
// This function is called by Entry function in Entry.cpp in kernel/Bootloader
// The Entry function initializes some necessary things required by kernel
// You might want to see that function if you are reading this for the first time
void KernelEntry() {
    // draw this string onto the screen
    Printf("Misra OS | Copyright Siddharth Mishra (c) 2022 | BSD 3-Clause License\n");

    // asm code to jump to same position again and again
    // asm volatile (".byte 0xeb, 0xef");

    // create pmm
    PhysicalMemoryManager pmm;
    Printf("[+] Created Physical Memory Manager\n");

    // show memory statistics
    pmm.ShowStatistics();

    // // create vmm
    VirtualMemoryManager vmm;
    Printf("[+] Created Virtual Memory Manager\n");

    // load gdt
    Printf("[+] Initializing Global Descriptor Table\n");
    InstallGDT();

    // install idt
    Printf("[+] Initializing Interrupt Descriptor Table\n");
    InstallIDT();

    // remap pic
    RemapPIC();

    RSDPDescriptor rsdp;

    SDTHeader* sdtHeader = reinterpret_cast<SDTHeader*>(rsdp.GetSDTAddress());
    // if rev == 0 then rsdt is used which has 4 bytes for each address
    // if rev != 0 then xsdt is used which has 8 bytes for each address
    uint64_t addrsize = rsdp.rev > 0 ? 8 : 4;
    // total entries except the header
    uint64_t entries = (sdtHeader->length - sizeof(SDTHeader)) / addrsize;
    // address of array of address of SDTs is just after table header
    uint64_t tableAddr = reinterpret_cast<uint64_t>(sdtHeader + 1);

    for(uint64_t i = 0; i < entries; i++){
        uint64_t sdtaddr;
        // add phys offset to convert physical to virtual address
        if(rsdp.rev == 0){
            sdtaddr = reinterpret_cast<uint32_t*>(tableAddr)[i] + MEM_PHYS_OFFSET;
        }else{
            sdtaddr = reinterpret_cast<uint64_t*>(tableAddr)[i] + MEM_PHYS_OFFSET;
        }

        SDTHeader* sdt = reinterpret_cast<SDTHeader*>(sdtaddr);
        for(uint8_t j = 0; j < 4; j++){
            PutChar(sdt->signature[j]);
        }
        PutChar('\n');
    }
}
