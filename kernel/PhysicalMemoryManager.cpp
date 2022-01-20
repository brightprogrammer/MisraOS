/**
 *@file PhysicalMemoryManager.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/19/2022
 *@brief Memory Manager
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/


#include "PhysicalMemoryManager.hpp"
#include "Constants.hpp"
#include "Print.hpp"
#include "Bootloader/Util.hpp"
#include "String.hpp"

// defines a memory block
struct MemoryBlock{
    uint64_t base = 0;
    uint64_t size = 0;

    uint64_t GetLimit(){
        return base + size;
    }
};

PhysicalMemoryManager::PhysicalMemoryManager(uint64_t numEntries, stivale2_mmap_entry* mmapEntries){
    if(isInitialized){
        PrintWarning("Another initialization of PhysicalMemoryManager");
    }

    numMemmapEntries = numEntries;
    memmapEntries = mmapEntries;

    // First step is to find the largest claimable block
    // we'll keep our pages array at the beginning of the largest block
    // We also have to find the total available memory
    MemoryBlock largestMemBlock;
    for(size_t i = 0; i < numEntries; i++){
        if(mmapEntries[i].length > largestMemBlock.size){
            largestMemBlock = {.base = mmapEntries[i].base, .size = mmapEntries[i].length};
        }

        // if memory is usable, then it's free
        if(mmapEntries[i].type == STIVALE2_MMAP_USABLE){
            freeMemory += mmapEntries[i].length;
        }else{
            reservedMemory += mmapEntries[i].length;
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
        PrintError("Insufficient memory to initialize PhysicalMemoryManager\n");
        PrintDebug("\tLargest memory block size : "); PrintDebug(utostr(largestMemBlock.size / KB)); PrintDebug(" KB\n");
        PrintDebug("\tMemory required : "); PrintDebug(utostr(numPagesUsedByStack * 4)); PrintDebug(" KB\n");
        InfiniteHalt();
    }

    PrintDebug("Memory Manager Initialization Stats :\n");
    PrintDebug("\tLargest memory block size : "); PrintDebug(utostr(largestMemBlock.size / KB)); PrintDebug(" KB\n");
    PrintDebug("\tMemory required : "); PrintDebug(utostr(numPagesUsedByStack * PAGE_SIZE / KB)); PrintDebug(" KB\n");

    // set pages at the start of this memory region
    pageStack = reinterpret_cast<uint64_t*>(largestMemBlock.base);
    // mark this memory as used
    freeMemory -= numPagesUsedByStack * PAGE_SIZE;
    usedMemory += numPagesUsedByStack * PAGE_SIZE;

    // Find usable page frames in small blocks first.
    // If these pages are at the bottom of the stack then
    // there is less chance of them being allocated.
    for(size_t i = 0 ; i < numEntries; i++){
        // if this block is usable and not the largest block
        if((mmapEntries[i].type == STIVALE2_MMAP_USABLE) && // memory is usable
           (mmapEntries[i].base != largestMemBlock.base) && // not the lagest block
           (mmapEntries[i].length >= PAGE_SIZE)){ // has atleast 1 page
            // start getting pages
            for(size_t i = currentStackSize; i < mmapEntries[i].length / PAGE_SIZE; i++){
                pageStack[i] = mmapEntries[i].base + i*PAGE_SIZE;
                currentStackSize++;
            }
        }
    }

    // Finally find available pages in largest memory block.
    uint64_t startAddress = largestMemBlock.base + numPagesUsedByStack * PAGE_SIZE;
    uint64_t maxAvailablePages =  (largestMemBlock.size - (numPagesUsedByStack * PAGE_SIZE)) / PAGE_SIZE;
    uint64_t j = 0;
    for(size_t i = currentStackSize; i < maxAvailablePages; i++){
        // check if page's limit is less than memory block's limit
        if((startAddress + (j + 1) * PAGE_SIZE) <= largestMemBlock.GetLimit()){
            pageStack[i] = startAddress + (j * PAGE_SIZE);
            currentStackSize++;
            j++;
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
        PrintError("Out Of Memory!");
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
        PrintWarning("Attemt to free a reserved page! : ");
        PrintWarning(utohexstr(page)); PrintWarning("\n");
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
        PrintError("Call to free pages with size greater than max allowed size");
    }
}

// print memmoy statistics
void PhysicalMemoryManager::ShowStatistics(){
    PrintDebug("Memory Manager : Memory Statistics\n");
    PrintDebug("\tFree Memory : "); PrintDebug(utostr(freeMemory / KB)); PrintDebug(" KB\n");
    PrintDebug("\tUsed Memory : "); PrintDebug(utostr(usedMemory / KB)); PrintDebug(" KB\n");
    PrintDebug("\tReserved Memory : "); PrintDebug(utostr(reservedMemory / KB)); PrintDebug(" KB\n");
    PrintDebug("\tFree Pages : "); PrintDebug(utostr(currentStackSize)); PrintDebug("\n");
    PrintDebug("\tTotal Pages : "); PrintDebug(utostr(pageStackMaxSize)); PrintDebug("\n");
}
