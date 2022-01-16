/**
 *@file page_frame_allocator.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief PageFrameAllocator
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef PAGE_FRAME_ALLOCTOR_H_
#define PAGE_FRAME_ALLOCTOR_H_

#include <stdint.h>
#include "bitmap.h"
#include "stivale2.h"

// initializes page frame allocator
// multiple calls to this function are useless
void createPageFrameAllocator(uint64_t numEntry, struct stivale2_mmap_entry *entries);

// free a page at that contains the given address
void freePage(void *address);
// allocate  a page that contains the given address
void allocatePage(void *address);
// unreserve (free) a page
void unreservePage(void *address);
// reserve (allocate) a page
void reservePage(void *address);
// free more than one pages at a time
void freePages(void *address, uint64_t numPages);
// allocate more than one pages at a time
void allocatePages(void *address, uint64_t numPages);
// unreserve more than one pages at a time
void unreservePages(void *address, uint64_t numPages);
// reserve pages
void reservePages(void *address, uint64_t numPages);

// get free memory
uint64_t getFreeMemory();
// get used memory
uint64_t getUsedMemory();
// get reserved memory
uint64_t getReservedMemory();

// request a free page address
void* findFreePage();

// prints memory stats
void printMemoryStats();

#endif // PAGE_FRAME_ALLOCTOR_H_
