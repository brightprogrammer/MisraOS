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

    PrintWarning(utohexstr(reinterpret_cast<uint64_t>(tagList)));
    Print("\n");


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

    for(size_t i = 0; i < memmap_tag->entries; i++){
        if(memmap_tag->memmap[i].type == STIVALE2_MMAP_USABLE)
            PrintDebug("USABLE MEMORY RANGE : 0x");
        else PrintDebug("UNUSABLE MEMORY RANGE : 0x");

        PrintDebug(utohexstr(memmap_tag->memmap[i].base));
        PrintDebug(" - 0x");
        PrintDebug(utohexstr(memmap_tag->memmap[i].base + memmap_tag->memmap[i].length));
        PrintDebug("\n");
    }

    // NOTE : Initialize memory manager only once
    PhysicalMemoryManager pmm(memmap_tag->entries, memmap_tag->memmap);
    pmm.ShowStatistics();

    // create page map table
    PageTable* pageMapTable = reinterpret_cast<PageTable*>(pmm.AllocatePage());
    memset(reinterpret_cast<void*>(pageMapTable), 0, PAGE_SIZE);

    // create vmm
    VirtualMemoryManager vmm(pageMapTable);

    // create identity map
    for(size_t i = 0; i < pmm.GetTotalMemory(); i += PAGE_SIZE){
        vmm.MapMemory(i, i);
    }

    // get kernel address
    stivale2_struct_tag_kernel_base_address* kaddr_tag = reinterpret_cast<stivale2_struct_tag_kernel_base_address*>(StivaleGetTag(tagList, STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID));
    uint64_t kernelPhyBase = kaddr_tag->physical_base_address;
    uint64_t kernelVirtBase = kaddr_tag->virtual_base_address;

    PrintWarning(utohexstr(reinterpret_cast<uint64_t>(tagList)));
    Print("\n");

    PrintDebug("Got kernel base address : Virtual = 0x"); PrintDebug(utohexstr(kaddr_tag->virtual_base_address));
    PrintDebug(" Physical = 0x"); PrintDebug(utohexstr(kaddr_tag->physical_base_address)); Print("\n");

    stivale2_struct_tag_pmrs* pmrs_tag = reinterpret_cast<stivale2_struct_tag_pmrs*>(StivaleGetTag(tagList, STIVALE2_STRUCT_TAG_PMRS_ID));
    stivale2_pmr* pmrs = pmrs_tag->pmrs;
    uint64_t numPmrs = pmrs_tag->entries;

    PrintDebug("Found "); PrintDebug(utostr(numPmrs)); PrintDebug(" PMR entries\n");
    for(size_t i = 0; i < numPmrs; i++){
        PrintDebug("PMR : Base = 0x");
        PrintDebug(utohexstr(pmrs[i].base));
        PrintDebug(" Length = ");
        PrintDebug(utostr(pmrs[i].length / KB));
        PrintDebug(" KB\n");
    }

    // map kernel acc to pmrs
    for(uint64_t i = 0; i < numPmrs; i++){
        uint64_t virtAddr = pmrs[i].base;
        uint64_t phyAddr = kernelPhyBase  + (virtAddr - kernelVirtBase);
        uint64_t length = pmrs[i].length;

        PrintDebug("VMM : Mapping 0x"); PrintDebug(utohexstr(phyAddr)); PrintDebug(" to 0x"); PrintDebug(utohexstr(virtAddr));
        PrintDebug(" and length : "); PrintDebug(utostr(length / KB)); PrintDebug(" KB\n");

        for(uint64_t j = 0; j < length; j+= PAGE_SIZE){
            vmm.MapMemory(virtAddr + j, phyAddr + j);
        }
    }

    // map the framebuffer to itself
    uint64_t fbBase = reinterpret_cast<uint64_t>(framebuffer.address);
    uint64_t fbSize = framebuffer.pitch * framebuffer.height + PAGE_SIZE;
    for(uint64_t i = fbBase; i < fbSize; i += PAGE_SIZE){
        vmm.MapMemory(i, i);
    }

    pmm.ShowStatistics();
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
