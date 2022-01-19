/**
 *@file MemoryManager.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/19/2022
 *@brief Memory Manager
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/


#ifndef MEMORY_HPP
#define MEMORY_HPP

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

struct MemoryManager{
    // create new memory manager
    MemoryManager(uint64_t numEntries, stivale2_mmap_entry* mmapEntries);

    // print memory statistics
    void ShowStatistics();

    // allocate a single page
    [[nodiscard]] uint64_t AllocatePage();

    // allocate multiple pages at once
    // if requested number of pages is more than 512
    // then the last entry of the returned array will store address of another uint64_t array
    // that contains the address of allocated pages
    [[nodiscard]] uint64_t* AllocatePages(size_t n);

    // free a single page
    void FreePage(uint64_t page);

    // free multiple pages at a time
    // if n is more than 512 then the last element must be a pointer to
    // another array that contains addresses of other pages to be freed!
    void FreePages(uint64_t* pages, size_t n);
private:
    // keep track of how much memory free, reserved and used
    size_t freeMemory = 0;
    size_t usedMemory = 0;
    size_t reservedMemory = 0;

    // base address of all pages
    uint64_t *pageStack = nullptr;
    // current size of stack
    // index of element after stack's top
    // analogous to stack pointer
    size_t currentStackSize = 0;
    // maximum number of pages
    size_t pageStackMaxSize = 0;
    // number of pages used by stack
    size_t numPagesUsedByStack = 0;

    // keep the memory map to check that we don't accidentially deallocate a reserved block
    uint64_t numMemmapEntries = 0;
    stivale2_mmap_entry* memmapEntries;
};

#endif // MEMORY_HPP
