/**
 *@file page_frame_allocator.c
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/15/2022
 *@brief Page Frame Allocator implementation
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "page_frame_allocator.h"
#include "memory.h"
#include "string.h"
#include "renderer.h"

// this is the total memory available that can be allocated
uint64_t freeMemory;
// this memory is reserved by other things, like bootloader etc...
uint64_t reservedMemory;
// this is the memory that is allocated and can be free'd in future
uint64_t usedMemory;

// to check if PFA is already allocated or not
bool initialized = false;

// Bitmap allocator creates a bitmap and marks
// bits as 0 or 1 corresponding to whether the page is
// allocated or not. If a page is allocated then corresponding
// bit is flagged. To free the page bit we just unset bit.
Bitmap pageBitmap;

// create page frame allocator for the biggest memory chunk
void createPageFrameAllocator(uint64_t numEntries, struct stivale2_mmap_entry *entries){
    // if memmap is already initialized then return
    if(initialized) return;
    initialized = true;

    // init stats
    freeMemory = 0;
    usedMemory = 0;
    reservedMemory = 0;

    // largest map available to us
    struct stivale2_mmap_entry *largestMap = NULL;
    size_t largestFreeMemSize = 0;

    // first we find the mem block with largest usable size
    for(uint64_t i = 0; i < numEntries; i++){
        if(entries[i].type == STIVALE2_MMAP_USABLE){
            if(entries[i].length > largestFreeMemSize){
                largestMap = entries + i;
                largestFreeMemSize = entries[i].length;
            }
        }
    }

    // get total memory
    // initially we assume that total memory is free'd
    // then we start marking them as allocated or free'd
    // depending on whether they are usable or not
    for(uint64_t i = 0; i < numEntries; i++){
        freeMemory += entries[i].length;
    }

    // create bitmap to keep track of allocated pages
    uint64_t bitmapSize = freeMemory / 4096 + 1;
    pageBitmap.size = bitmapSize;
    pageBitmap.buffer = (uint8_t*)largestMap->base;

    // unset all bits in bitmap
    memset(pageBitmap.buffer, 0, bitmapSize);

    // allocate memory for bitmap
    // we've already used the region
    // we're just marking it as allocated
    // +1 is to allocate atleast the page at the starting of buffer
    allocatePages(&pageBitmap, (bitmapSize / 4096) + 1);

    // reserve unused pages
    for(size_t i = 0; i < numEntries; i++){
        // if memory is not marked usable by bootloader
        // then mark it as reserved inside kernel
        if(entries[i].type != STIVALE2_MMAP_USABLE){
            reservePages((void*)entries[i].base, entries[i].length / 4096 + 1);
        }
    }
}

// free allocated page
void freePage(void *address){
    uint64_t index = ((uint64_t)address / 8);
    // if page was allocated then free it
    // to free a page we just have to mark it freed!
    if(bitmapGetBit(&pageBitmap, index) == true){
        bitmapSetBit(&pageBitmap, index, false);
        freeMemory += 4096;
        usedMemory -= 4096;
    }
}

// free more than one pages at a time
void freePages(void *address, uint64_t numPages){
    for(size_t i = 0; i < numPages; i++){
        freePage((void*)((uint64_t)address + i*4096));
    }
}

// allocate page
void allocatePage(void *address){
    uint64_t index = ((uint64_t)address / 8);
    // if page was allocated then free it
    if(bitmapGetBit(&pageBitmap, index) == false){
    // to free a page we just have to mark it freed!
        bitmapSetBit(&pageBitmap, index, true);
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

// allocate more than one pages at once
void allocatePages(void *address, uint64_t numPages){
    for(size_t i = 0; i < numPages; i++){
        allocatePage((void*)((uint64_t)address + i*4096));
    }
}

// free allocated page
void unreservePage(void *address){
    uint64_t index = ((uint64_t)address / 8);
    // if page was allocated then free it
    // to free a page we just have to mark it freed!
    if(bitmapGetBit(&pageBitmap, index) == true){
        bitmapSetBit(&pageBitmap, index, false);
        freeMemory += 4096;
        reservedMemory -= 4096;
    }
}

// unreserve more than one pages at once
void unreservePages(void *address, uint64_t numPages){
    for(size_t i = 0; i < numPages; i++){
        unreservePage((void*)((uint64_t)address + i*4096));
    }
}

// allocate page
void reservePage(void *address){
    uint64_t index = ((uint64_t)address / 8);
    // if page was allocated then free it
    // to free a page we just have to mark it freed!
    if(bitmapGetBit(&pageBitmap, index) == false){
        bitmapSetBit(&pageBitmap, index, true);
        freeMemory -= 4096;
        reservedMemory += 4096;
    }
}

// reserve more than one pages at once
void reservePages(void *address, uint64_t numPages){
    for(size_t i = 0; i < numPages; i++){
        reservePage((void*)((uint64_t)address + i*4096));
    }
}

// get free memory
uint64_t getFreeMemory(){
    return freeMemory;
}

// get used memory
uint64_t getUsedMemory(){
    return usedMemory;
}

// get reserved memory
uint64_t getReservedMemory(){
    return reservedMemory;
}

// find free page address
uint64_t lastPageSearchIndex = 0;
void* findFreePage(){
    for(uint64_t i = lastPageSearchIndex; i < pageBitmap.size; i++){
        if(bitmapGetBit(&pageBitmap, i) == false){
            allocatePage((void*)(i * 4096));
            return (void*)(i * 4096);
        }
    }

    // TODO : Create a page frame swap to allocate memory on disk
    // if not available in ram
    return NULL;
}

void printMemoryStats(){
    drawString("Displaying memory stats : \n");
    drawString("\t[+] Total free memmory : 0x");
    drawString(utohexstr(getFreeMemory()));
    drawString("\n\t[+] Total used memory : 0x");
    drawString(utohexstr(getUsedMemory()));
    drawString("\n\t[+] Total reserved memory : 0x");
    drawString(utohexstr(getReservedMemory()));
    drawString("\n");
}
