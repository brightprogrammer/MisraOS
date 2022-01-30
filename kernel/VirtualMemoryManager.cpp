/**
 *@file VirtualMemoryManager.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/20/2022
 *@brief Handles virtual memory mapping and stuffs
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

#include "VirtualMemoryManager.hpp"
#include "PhysicalMemoryManager.hpp"
#include "Utils/String.hpp"
#include "Printf.hpp"

#include "Bootloader/BootInfo.hpp"

#define PAGE_PHYSICAL_ADDRESS_MASK 0x000ffffffffff000

// turn on given flags
void Page::SetFlags(uint64_t flags){
    value |= flags;
}

// turn off given flags
void Page::UnsetFlags(uint64_t flags){
    value &= (~flags | PAGE_PHYSICAL_ADDRESS_MASK);
}

// get state of given flags
bool Page::GetFlags(uint64_t flags){
    return (value & flags) > 0 ? true : false;
}

// get address of this mapped page
uint64_t Page::GetAddress(){
    // physical address is 40 bits only
    return (value & PAGE_PHYSICAL_ADDRESS_MASK) >> 12;
}

// set address of this page
void Page::SetAddress(uint64_t address){
    value &= 0xfff0000000000fff;
    value |= (address << 12) & PAGE_PHYSICAL_ADDRESS_MASK;
}

VirtualMemoryManager::VirtualMemoryManager(){
    // create's page table root entry
    CreatePageMap();

    MemMapEntry* memmap = BootInfo::GetMemmap();
    uint64_t memmapCount = BootInfo::GetMemmapCount();

    // map first 4 gb of memory to higher half
    // this memeory
    // for(uint64_t p = 0; p < 4*GB; p += PAGE_SIZE){
    //     MapMemory(MEM_PHYS_OFFSET + p, p, MAP_PRESENT | MAP_READ_WRITE);
    // }

    for(size_t i = 0; i < memmapCount; i++){
        if(memmap[i].type != STIVALE2_MMAP_KERNEL_AND_MODULES){
            for(size_t p = 0; p < memmap[i].length; p += PAGE_SIZE){
                uint64_t paddr = memmap[i].base + p;
                uint64_t vaddr = MEM_PHYS_OFFSET + paddr;
                MapMemory(vaddr, paddr, MAP_PRESENT | MAP_READ_WRITE);
            }
        }else{
            for(size_t p = 0; p < memmap[i].length; p += PAGE_SIZE){
                uint64_t paddr = memmap[i].base + p;
                uint64_t vaddr = KERNEL_VIRT_BASE + p;
                MapMemory(vaddr, paddr, MAP_PRESENT | MAP_READ_WRITE);
            }
        }
    }

    // uint64_t krnlPhysBase = BootInfo::GetKernelPhysicalBase();
    // for (uintptr_t p = 0; p < 2*GB; p += PAGE_SIZE){
    //     uint64_t paddr = krnlPhysBase + p;
    //     uint64_t vaddr = KERNEL_VIRT_BASE + p;
    //     MapMemory(vaddr, paddr, MAP_PRESENT | MAP_READ_WRITE);
    // }

    // load page table into cr3 register
    LoadPageTable();
}

// this will create the root node of the page map tree
void VirtualMemoryManager::CreatePageMap(){
    if(pml4 == nullptr){
        // create new page map
        uint64_t pml4VirtualAddress = PhysicalMemoryManager::AllocatePage();
        pml4PhysicalAddress = pml4VirtualAddress - MEM_PHYS_OFFSET;
        pml4 = reinterpret_cast<PageTable*>(pml4VirtualAddress);

        // and set all elements to 0
        memset(pml4, 0, PAGE_SIZE);
    }else{
        Printf("[!] Attempt to recreate prexisting root level page map!\n");
    }
}

void VirtualMemoryManager::LoadPageTable(){
    // load the page map table in cr3 register
    asm volatile("mov %0, %%cr3"
                 :
                 : "r" (pml4PhysicalAddress)); // map takes
}

// get next level of paging
PageTable* VirtualMemoryManager::GetNextLevel(PageTable* pageTable, uint64_t entryIndex, bool allocate){
    Page* pte = &pageTable->entries[entryIndex];
    PageTable* pt = nullptr;

    // if page directory entry is not present and allocation is allowed, then allocate it
    if(!pte->GetFlags(MAP_PRESENT)){
        // if allocation isn't allowed then return nullptr
        if(!allocate){
            return nullptr;
        }

        // create page directory pointer
        uint64_t vaddr = PhysicalMemoryManager::AllocatePage();
        uint64_t paddr = vaddr - MEM_PHYS_OFFSET;
        pt = reinterpret_cast<PageTable*>(vaddr);
        memset(reinterpret_cast<void*>(pt), 0, PAGE_SIZE);

        // shift by 12 biits to align it to 0x1000 boundary
        pte->SetAddress(paddr >> 12);
        pte->SetFlags(MAP_PRESENT | MAP_READ_WRITE);
    }else{
        uint64_t paddr = pte->GetAddress() << 12;
        uint64_t vaddr = paddr + MEM_PHYS_OFFSET;
        pt = reinterpret_cast<PageTable*>(vaddr);
    }

    return pt;
}

// map given physical memory to virtual memory wiht given flags
void VirtualMemoryManager::MapMemory(uint64_t virtualAddress, uint64_t physicalAddress, uint64_t flags){
    // get page table entry
    Page* pte = GetPage(virtualAddress, true);
    if(pte == nullptr) return;

    // map physical address 4kb aligned
    pte->SetAddress(physicalAddress >> 12);
    pte->SetFlags(flags);
}

// get's you a single page corresponding to the given virtual address:w
Page* VirtualMemoryManager::GetPage(uint64_t vaddr, bool allocate){
    // cache this value
    uint64_t virtualAddr = vaddr;

    // get page index
    vaddr >>= 12;
    uint64_t pageIndex = vaddr & 0x1ff; // 0x1ff = 511 or 512 - 1

    // find table index
    vaddr >>= 9;
    uint64_t pml1Index = vaddr & 0x1ff;

    // find page directory index
    vaddr >>= 9;
    uint64_t pml2Index = vaddr & 0x1ff;

    // find page directory pointer index
    vaddr >>= 9;
    uint64_t pml3Index = vaddr & 0x1ff;

    // get page directory pointer from PML4
    PageTable* pml3 = GetNextLevel(pml4, pml3Index, allocate);
    if(pml3 == nullptr){
        Printf("[-] PML3 for vaddr(%lx) doesn't exists or failed to allocate\n", virtualAddr);
        return nullptr;
    }

    // get page directory from page directory pointer
    PageTable* pml2 = GetNextLevel(pml3, pml2Index, allocate);
    if(pml2 == nullptr){
        Printf("[-] PML2 for vaddr(%lx) doesn't exists or failed to allocate\n", virtualAddr);
        return nullptr;
    }


    // get page table from page directory
    PageTable* pml1 = GetNextLevel(pml2, pml1Index, allocate);
    if(pml1 == nullptr){
        Printf("[-] PML1 for vaddr(%lx) doesn't exists or failed to allocate\n", virtualAddr);
        return nullptr;
    }


    // get page from page table
    Page *pte = &pml1->entries[pageIndex];
    if(pte == nullptr){
        Printf("[-] Page Table Entry for vaddr(%lx) doesn't exists or failed to allocate\n", virtualAddr);
        return nullptr;
    }

//     while(true) asm("hlt");

    return pte;
}
