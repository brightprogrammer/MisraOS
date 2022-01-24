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
#include "PhysicalMemoryManager.hpp"

#define HIGHER_HALF_MEM_VIRT_OFFSET 0xffff800000000000
#define KERNEL_VIRT_BASE 0xffffffff80000000

// page and page directory pointer use the same structure
struct Page {
    // is the page directory that this entry is pointing to present?
    bool present : 1;
    // is the page in question is read only or read/write?
    bool readWrite : 1;
    // is me memory accessible to supervisor only or to user also
    bool supervisorOnly : 1;
    bool writeThrough : 1;
    // do you want this page to be cached?
    bool cacheDisable : 1;
    // set by cpu when this page is accessed
    bool accessed : 1;
    bool reserved0 : 1;
    // on allocation of more than one pages at a time
    // an array is returned with address of allocated pages in it
    // if largerPages is set then that array will be treated as a large
    // page which is made by combining all those pages
    // this large page will be max to max 2MB in size
    bool largerPages : 1;
    bool reserved2 : 1;
    uint8_t available : 3;
    // address of page
    uint64_t address : 52;
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
    void MapMemory(uint64_t virtualAddress, uint64_t physicalAddress);

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

    // vmm's own pmm
    static inline PhysicalMemoryManager pmm = {};

    // root element of the page map tree
    PageTable* pml4 = nullptr;
};

#endif // VIRTUALMEMORYMANAGER_HPP
