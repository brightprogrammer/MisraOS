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

// The following will be our kernel's entry point.
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

    // create vmm
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
    PortOutByte(PICMASTER_DATA, 0b11111101);
    PortOutByte(PICSLAVE_DATA, 0b11111111);

    asm("sti");

    asm volatile("int $0x10");
}
