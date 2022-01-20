/**
 *@file MemoryManager.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/19/2022
 *@brief Memory Manager
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/


#ifndef PHYSICALMEMORYMANAGER_HPP
#define PHYSICALMEMORYMANAGER_HPP

#include <cstdint>
#include <cstddef>
#include "Bootloader/stivale2.h"
#include "Constants.hpp"

// Stack based page frame allocator :
// store available pages in a stack
// when a page is allocated, remote it from top of stack
// and decrease the stack size
// when it's free'd then push it to the top of stack
// this gives O(1) search time
// Other options :
// 1 - Bitmap Allocator
// 2 - Buddy Allocator
// 3 - Slab Allocator (claimed to be the best)

#define PAGE_SIZE (4*KB)

// manages page allocation
struct PhysicalMemoryManager{
    // default constructor
    PhysicalMemoryManager() = default;

    // create new memory manager
    PhysicalMemoryManager(uint64_t numEntries, stivale2_mmap_entry* mmapEntries);

    // print memory statistics
    void ShowStatistics();

    // getters
    uint64_t GetFreeMemory();
    uint64_t GetUsedMemory();
    uint64_t GetReservedMemory();
    uint64_t GetTotalMemory();

    // allocate a single page
    [[nodiscard]] uint64_t AllocatePage();

    // allocate multiple pages at once
    // each entry in the returned array corresponds to a new page
    // max size is 512 pages at a time or 2MB
    [[nodiscard]] uint64_t* AllocatePages(size_t n);

    // free a single page
    void FreePage(uint64_t page);

    // free multiple pages at a time
    // max size limit is 512 or 2MB
    void FreePages(uint64_t* pages, size_t n);
private:
    // check of PMM is already initialized or not
    static inline bool isInitialized = false;

    // keep track of how much memory free, reserved and used
    static inline size_t freeMemory = 0;
    static inline size_t usedMemory = 0;
    static inline size_t reservedMemory = 0;

    // base address of all pages
    static inline uint64_t *pageStack = nullptr;
    // current size of stack
    // index of element after stack's top
    // analogous to stack pointer
    static inline size_t currentStackSize = 0;
    // maximum number of pages
    static inline size_t pageStackMaxSize = 0;
    // number of pages used by stack
    static inline size_t numPagesUsedByStack = 0;

    // keep the memory map to check that we don't accidentially deallocate a reserved block
    static inline uint64_t numMemmapEntries = 0;
    static inline stivale2_mmap_entry* memmapEntries = nullptr;
};

#endif // PHYSICALMEMORYMANAGER_HPP
