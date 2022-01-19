#include "MemoryManager.hpp"
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

MemoryManager::MemoryManager(uint64_t numEntries, stivale2_mmap_entry* mmapEntries):
numMemmapEntries(numEntries), memmapEntries(mmapEntries){
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
        PrintError("Insufficient memory to initialize MemoryManager\n");
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
    uint64_t maxAvailablePages =  (largestMemBlock.size - numPagesUsedByStack*PAGE_SIZE) / PAGE_SIZE;
    for(size_t i = currentStackSize; i < maxAvailablePages; i++){
        // check if page's limit is less than memory block's limit
        if(startAddress + (i + 1) * PAGE_SIZE < largestMemBlock.GetLimit()){
            pageStack[i] = startAddress + i * PAGE_SIZE;
            currentStackSize++;
        }
    }
}

// allocate's a single page
// this pops out the top element from stack and returns the value
uint64_t MemoryManager::AllocatePage(){
    if(currentStackSize == 0){
        PrintError("Out Of Memory!");
        InfiniteHalt();
    }

    freeMemory -= PAGE_SIZE;
    usedMemory += PAGE_SIZE;

    currentStackSize--;
    return pageStack[currentStackSize - 1];
}

// allocate more than one pages at a time
// if requested number of pages is more than 512 then the last
// entry of the returned array will store address of another uint64_t array
// that contains the address of allocated pages
uint64_t* MemoryManager::AllocatePages(size_t n){
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

    if(n > 512){
        // allocate memory for return array
        uint64_t* pageFrames = reinterpret_cast<uint64_t*>(AllocatePage());
        // allocate requested number of pages and return the array
        for(size_t i = 0 ; i < 511; i++){
            pageFrames[i] = AllocatePage();
        }

        // last element of return array will now point to another array
        pageFrames[511] = reinterpret_cast<uint64_t>(AllocatePages(n - 512));

        return pageFrames;
    }

    return nullptr;
}

// free a single page
void MemoryManager::FreePage(uint64_t page){
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

// if n is more than 512 then the last element must be a pointer to
// another array that contains addresses of other pages to be freed!
void MemoryManager::FreePages(uint64_t *pages, size_t n){
    if(n <= 512){
        for(size_t i = 0; i < n; i++){
            FreePage(pages[i]);
        }
    }

    if(n > 512){
        for(size_t i = 0; i < 511; i++){
            FreePage(pages[i]);
        }

        // free rest of the pages
        FreePages(reinterpret_cast<uint64_t*>(pages[511]), n - 512);
    }
}

// print memmoy statistics
void MemoryManager::ShowStatistics(){
    PrintDebug("Memory Manager : Memory Statistics\n");
    PrintDebug("\tFree Memory : "); PrintDebug(utostr(freeMemory / KB)); PrintDebug(" KB\n");
    PrintDebug("\tUsed Memory : "); PrintDebug(utostr(usedMemory / KB)); PrintDebug(" KB\n");
    PrintDebug("\tReserved Memory : "); PrintDebug(utostr(reservedMemory / KB)); PrintDebug(" KB\n");
    PrintDebug("\tFree Pages : "); PrintDebug(utostr(currentStackSize)); PrintDebug("\n");
    PrintDebug("\tTotal Pages : "); PrintDebug(utostr(pageStackMaxSize)); PrintDebug("\n");
}
