/**
 *@file KernelEntry.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief defines kernel's entry point
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "Renderer/Framebuffer.hpp"
#include "Renderer/FontRenderer.hpp"
#include "Print.hpp"
#include "String.hpp"
#include "GDT.hpp"
#include "Utils/Bitmap.hpp"
#include "Bootloader/Util.hpp"
#include "PhysicalMemoryManager.hpp"
#include "VirtualMemoryManager.hpp"

// The following will be our kernel's entry point.
extern "C" [[noreturn]] void KernelEntry(struct stivale2_struct *tagList) {
  // Let's get the terminal structure tag from the bootloader.
    struct stivale2_struct_tag_framebuffer *framebuffer_tag;
    framebuffer_tag = reinterpret_cast<stivale2_struct_tag_framebuffer*>(StivaleGetTag(tagList, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID));

    // Check if the tag was actually found.
    if (framebuffer_tag == NULL) {
        InfiniteHalt();
    }

    // get framebuffer details
    Framebuffer framebuffer(framebuffer_tag);
    // prepare renderer
    FontRenderer fontRenderer(framebuffer);
    // set default font renderer
    SetDefaultFontRenderer(fontRenderer);

    // draw this string onto the screen
    Print("Misra OS | Copyright Siddharth Mishra (c) 2022 | CC BY-SA 3.0\n\n");

    // asm code to jump to same position again and again
    // asm volatile (".byte 0xeb, 0xef");

    // load gdt
    PrintDebug("Initializing Global Descriptor Table\n");
    InstallGDT();

    // get the memmap tag given to kernel by the bootloader
    struct stivale2_struct_tag_memmap *memmap_tag;
    memmap_tag = reinterpret_cast<stivale2_struct_tag_memmap*>(StivaleGetTag(tagList, STIVALE2_STRUCT_TAG_MEMMAP_ID));

    // check if tag is valid
    if(memmap_tag == NULL){
        PrintError("Failed to get memory map.");

        // hang
        InfiniteHalt();
    }else{
        PrintDebug("Got the memory map.\n");
    }

    // NOTE : Initialize memory manager only once
    PhysicalMemoryManager physicalMemoryManager(memmap_tag->entries, memmap_tag->memmap);
    physicalMemoryManager.ShowStatistics();

    // create page map table
    PageTable* pageMapTable = reinterpret_cast<PageTable*>(physicalMemoryManager.AllocatePage());
    memset(reinterpret_cast<void*>(pageMapTable), 0, PAGE_SIZE);

    // // create vmm
     VirtualMemoryManager virtualMemoryManager(pageMapTable);

    // create identity map
    // i.e each physical address is mapped to itself
    for(uint64_t t = 0; t < physicalMemoryManager.GetTotalMemory(); t += PAGE_SIZE){
        virtualMemoryManager.MapMemory(t, t);
    }

    // map the framebuffer to itself
    uint64_t fbBase = reinterpret_cast<uint64_t>(framebuffer.address);
    uint64_t fbSize = static_cast<uint64_t>(framebuffer.width) * static_cast<uint64_t>(framebuffer.height) + PAGE_SIZE;
    for(uint64_t i = fbBase; i < fbSize; i += PAGE_SIZE){
        virtualMemoryManager.MapMemory(i, i);
    }

    physicalMemoryManager.ShowStatistics();
    //InfiniteHalt();

    // load the page map table in cr3 register
    asm volatile("mov %0, %%cr3"
                 :
                 : "r" (pageMapTable));

    PrintDebug("Virtual Memory Manager is up and running!");

    // We're done, just InfiniteHalt...
    for (;;) {
        asm ("hlt");
    }
}
