/**
 *@file VirtualMemoryManager.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/20/2022
 *@brief Handles virtual memory mapping and stuffs
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef VIRTUALMEMORYMANAGER_HPP
#define VIRTUALMEMORYMANAGER_HPP

#include <cstdint>
#include "PhysicalMemoryManager.hpp"

#define HIGHER_HALF_MEM_VIRT_OFFSET 0xffff800000000000
#define KERNEL_VIRT_BASE 0xffffffff80000000

// page and page directory pointer use the same structure
struct PageDirectoryEntry {
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
    PageDirectoryEntry entries[512];
} __attribute__((aligned(0x1000)));


// vmm implementation
struct VirtualMemoryManager{
    // create virtual memory manager
    VirtualMemoryManager(PageTable* PML4Address);

    // map physical address to given virtual address
    void MapMemory(uint64_t virtualAddress, uint64_t physicalAddress);

    // check if page map for a virtual address exists or not

private:
    // get's the next level in page table tree
    PageTable* GetNextLevel(PageTable* pageTable, uint64_t entryIndex, bool allocate);

    static inline PhysicalMemoryManager pmm = {};
    PageTable* pml4;
};

#endif // VIRTUALMEMORYMANAGER_HPP
