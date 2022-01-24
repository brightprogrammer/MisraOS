/**
 *@file PhysicalMemoryManager.cpp
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

#include "PhysicalMemoryManager.hpp"
#include "Constants.hpp"
#include "Printf.hpp"
#include "String.hpp"

#include "Bootloader/BootInfo.hpp"
#include "Bootloader/Util.hpp"

#define MEM_PHYS_OFFSET 0xffff800000000000

// defines a memory block
struct MemoryBlock{
    uint64_t base = 0;
    uint64_t size = 0;

    uint64_t GetLimit(){
        return base + size;
    }
};

PhysicalMemoryManager::PhysicalMemoryManager(){
    if(isInitialized){
        return;
    }

    numMemmapEntries = BootInfo::GetMemmapCount();
    memmapEntries = BootInfo::GetMemmap();

    // First step is to find the largest claimable block
    // we'll keep our pages array at the beginning of the largest block
    // We also have to find the total available memory
    MemoryBlock largestMemBlock;
    for(size_t i = 0; i < numMemmapEntries; i++){
        if(memmapEntries[i].length > largestMemBlock.size){
            largestMemBlock = {.base = memmapEntries[i].base, .size = memmapEntries[i].length};
        }

        // if memory is usable, then it's free
        if(memmapEntries[i].type == STIVALE2_MMAP_USABLE){
            freeMemory += memmapEntries[i].length;
        }else{
            reservedMemory += memmapEntries[i].length;
        }
    }

    // Second step is to calculate the total size needed for the stack
    // calculate the size of page stack
    // page stack size is the number of pages that can be allocated
    pageStackMaxSize = freeMemory / PAGE_SIZE;
    // calculate required numer of pages to allocate for stack
    numPagesUsedByStack = ((pageStackMaxSize * 8) / PAGE_SIZE) + 1;
    // check if largest block can provide this much space or not
    if(largestMemBlock.size <= numPagesUsedByStack * PAGE_SIZE){
        Printf("[-] Insufficient memory to initialize PhysicalMemoryManager\n");
        Printf("\tLargest memory block size : %li KB\n", (largestMemBlock.size / KB));
        Printf("\tMemory required : %li KB\n", (numPagesUsedByStack * PAGE_SIZE / KB));
        InfiniteHalt();
    }

    // set pages at the start of this memory region
    pageStack = reinterpret_cast<uint64_t*>(largestMemBlock.base + MEM_PHYS_OFFSET);
    // mark this memory as used
    freeMemory -= numPagesUsedByStack * PAGE_SIZE;
    usedMemory += numPagesUsedByStack * PAGE_SIZE;

    // Find usable page frames in small blocks first.
    // If these pages are at the bottom of the stack then
    // there is less chance of them being allocated.
    for(size_t i = 0 ; i < numMemmapEntries; i++){
        // if this block is usable and not the largest block
        if((memmapEntries[i].type == STIVALE2_MMAP_USABLE) && // memory is usable
           (memmapEntries[i].base != largestMemBlock.base) && // not the lagest block
           (memmapEntries[i].length >= PAGE_SIZE)){ // has atleast 1 page

            // start getting pages
            uint64_t numPages = memmapEntries[i].length / PAGE_SIZE;
            for(size_t j = 0; j < numPages ; j++){
                // if this page's limit is less than this block's limit then add it
                if((memmapEntries[i].base + (j + 1) * PAGE_SIZE) <= memmapEntries[i].base + memmapEntries[i].length){
                    pageStack[currentStackSize] = MEM_PHYS_OFFSET + memmapEntries[i].base + (j * PAGE_SIZE);
                    currentStackSize++;
                }
            }
        }
    }

    // Finally find available pages in largest memory block.
    uint64_t startAddress = largestMemBlock.base + numPagesUsedByStack * PAGE_SIZE;
    uint64_t maxAvailablePages =  (largestMemBlock.size - (numPagesUsedByStack * PAGE_SIZE)) / PAGE_SIZE;

    for(size_t i = 0; i < maxAvailablePages; i++){
        // check if page's limit is less than memory block's limit
        if((startAddress + (i + 1) * PAGE_SIZE) <= largestMemBlock.GetLimit()){
            // limine already maps addresses to higher half
            // so we have to add that offset to all page addresses
            pageStack[currentStackSize] = MEM_PHYS_OFFSET + startAddress + (i * PAGE_SIZE);
            currentStackSize++;
        }
    }
}

uint64_t PhysicalMemoryManager::GetFreeMemory(){ return freeMemory; }
uint64_t PhysicalMemoryManager::GetUsedMemory(){ return usedMemory; }
uint64_t PhysicalMemoryManager::GetReservedMemory(){ return reservedMemory; }
uint64_t PhysicalMemoryManager::GetTotalMemory(){ return freeMemory + usedMemory + reservedMemory; }

// allocate's a single page
// this pops out the top element from stack and returns the value
uint64_t PhysicalMemoryManager::AllocatePage(){
    if(currentStackSize == 0){
        Printf("Out Of Memory!");
        InfiniteHalt();
    }

    freeMemory -= PAGE_SIZE;
    usedMemory += PAGE_SIZE;

    currentStackSize--;
    return pageStack[currentStackSize];
}

// allocate more than one pages at a time
// max allowed size to allocate at a time is 512 pages
// this is equivalent to 2MB memory at a time
uint64_t* PhysicalMemoryManager::AllocatePages(size_t n){
    // n = 512 means 1 page
    if(n <= 512){
        // allocate memory for return array
        uint64_t* pageFrames = reinterpret_cast<uint64_t*>(AllocatePage());
        // allocate requested number of pages and return the array
        for(size_t i = 0 ; i < n; i++){
            pageFrames[i] = AllocatePage();
        }

        return pageFrames;
    }

    return nullptr;
}

// free a single page
void PhysicalMemoryManager::FreePage(uint64_t page){
    bool freeable = true;
    page -= MEM_PHYS_OFFSET;
    for(uint64_t i = 0; i < numMemmapEntries; i++){
        if(memmapEntries[i].type != STIVALE2_MMAP_USABLE){
            // check if any part of this page is inside a reserved region
            if((page + PAGE_SIZE > memmapEntries[i].base) &&
               (page <= memmapEntries[i].base + memmapEntries[i].length)){
                freeable = false;
            }
        }
    }

    if(freeable){
        pageStack[currentStackSize] = page;
        currentStackSize++;
        usedMemory -= PAGE_SIZE;
        freeMemory += PAGE_SIZE;
    }else{
        Printf("Attemt to free a reserved page! : Address = %lx\n", page);
    }
}

// free pages at addresses stored at the value provided in the element of
// this pages array
void PhysicalMemoryManager::FreePages(uint64_t *pages, size_t n){
    if(n <= 512){
        for(size_t i = 0; i < n; i++){
            FreePage(pages[i]);
        }
    }else{
        Printf("Call to free pages with size greater than max allowed size");
    }
}

// print memmoy statistics
void PhysicalMemoryManager::ShowStatistics(){
    Printf("[+] Memory Stats : \n");
    Printf("\tFree Memory : %lu KB\n", (freeMemory/KB));
    Printf("\tUsed Memory : %lu KB\n", (usedMemory/KB));
    Printf("\tReserved Memory : %lu KB\n", (reservedMemory/KB));
    Printf("\tFree Pages : %lu pages\n", (currentStackSize));
    Printf("\tTotal Pages : %lu pages\n", (pageStackMaxSize));
}
