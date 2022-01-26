/**
 *@file VirtualMemoryManager.hpp
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
 **/

#ifndef VIRTUALMEMORYMANAGER_HPP
#define VIRTUALMEMORYMANAGER_HPP

#include <cstdint>

#define MEM_PHYS_OFFSET uint64_t(0xffff800000000000)
#define KERNEL_VIRT_BASE uint64_t(0xffffffff80000000)

enum PageFlags {
    MAP_PRESENT = 1 << 0,
    MAP_READ_WRITE = 1 << 1,
    MAP_SUPERVISOR_ONLY = 1 << 2,
    MAP_WRITE_THROUGH = 1 << 3,
    MAP_CACHE_DISABLED = 1 << 4,
    MAP_ACCESSED = 1 << 5,
    MAP_LARGER_PAGES =  1 << 7,
    MAP_CUSTOM0 = 1 << 9,
    MAP_CUSTOM1 = 1 << 10,
    MAP_CUSTOM2 = 1 << 11,
    MAP_NO_EXECUTE = uint64_t(1) << 63 // only if supported
};

// page and page directory pointer use the same structure
struct Page {
    uint64_t value; //
    // set given flags to true
    void SetFlags(uint64_t flags);
    // set given flags to false
    void UnsetFlags(uint64_t flags);
    // if given flags are true then true is returned
    bool GetFlags(uint64_t flags);
    // set physical address
    void SetAddress(uint64_t address);
    // get physical address (4kb aligned always)
    uint64_t GetAddress();
};


// page table and page map level 4 use the same structure
struct PageTable {
    Page entries[512];
} __attribute__((aligned(0x1000)));


// vmm implementation
struct VirtualMemoryManager{
    // create virtual memory manager
    VirtualMemoryManager();

    // map physical address to given virtual address
    // after physical and virtual address, you pass flags
    void MapMemory(uint64_t virtualAddress, uint64_t physicalAddress, uint64_t flags);

    // create page mapping
    void CreatePageMap();

    // get page for given virtual address
    // if allocate is true then required page and page tables
    // will be allocated if not already allocated
    Page* GetPage(uint64_t vaddr, bool allocate);

    // load this page table in cr3 register
    void LoadPageTable();
private:

    // get's the next level in page table tree
    PageTable* GetNextLevel(PageTable* pageTable, uint64_t entryIndex, bool allocate);

    // root element of the page map tree
    PageTable* pml4 = nullptr;

    // store pml4 physicall address
    uint64_t pml4PhysicalAddress = 0;
};

#endif // VIRTUALMEMORYMANAGER_HPP
