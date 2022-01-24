/**
 *@file MemoryManager.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/19/2022
 *@brief Memory Manager
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

#ifndef PHYSICALMEMORYMANAGER_HPP
#define PHYSICALMEMORYMANAGER_HPP

#include <cstdint>
#include <cstddef>
#include "Bootloader/BootInfo.hpp"
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

#define PAGE_SIZE uint64_t(4*KB)

// manages page allocation
struct PhysicalMemoryManager{
    // create new memory manager
    PhysicalMemoryManager();

    // print memory statistics
    static void ShowStatistics();

    // getters
    static uint64_t GetFreeMemory();
    static uint64_t GetUsedMemory();
    static uint64_t GetReservedMemory();
    static uint64_t GetTotalMemory();

    // allocate a single page
    // NOTE : Allocate page will always return PhysicalAddress + 0xffff800000000000
    [[nodiscard]] static uint64_t AllocatePage();

    // allocate multiple pages at once
    // each entry in the returned array corresponds to a new page
    // max size is 512 pages at a time or 2MB
    // NOTE : return addresses with higher half offset (same as allocate page - singular)
    [[nodiscard]] static uint64_t* AllocatePages(size_t n);

    // free a single page
    static void FreePage(uint64_t page);

    // free multiple pages at a time
    // max size limit is 512 or 2MB
    static void FreePages(uint64_t* pages, size_t n);
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
    static inline MemMapEntry* memmapEntries = nullptr;
};

#endif // PHYSICALMEMORYMANAGER_HPP
