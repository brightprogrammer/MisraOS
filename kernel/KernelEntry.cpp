/**
 *@file KernelEntry.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief defines kernel's entry point
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "Renderer/Framebuffer.hpp"
#include "Renderer/FontRenderer.hpp"
#include "String.hpp"
#include "GDT.hpp"
#include "PhysicalMemoryManager.hpp"
#include "VirtualMemoryManager.hpp"
#include "Printf.hpp"

// The following will be our kernel's entry point.
void KernelEntry() {
    // draw this string onto the screen
    Printf("Misra OS | Copyright Siddharth Mishra (c) 2022 | CC BY-SA 3.0\n");

    // asm code to jump to same position again and again
    // asm volatile (".byte 0xeb, 0xef");

    // load gdt
    Printf("[+] Initializing Global Descriptor Table\n");
    InstallGDT();

    // create pmm
    PhysicalMemoryManager pmm;
    Printf("[+] Created Physical Memory Manager\n");

    // show memory statistics
    pmm.ShowStatistics();

    // create vmm
    VirtualMemoryManager vmm;
    Printf("[+] Created Virtual Memory Manager\n");
}
