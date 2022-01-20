#include "VirtualMemoryManager.hpp"
#include "PhysicalMemoryManager.hpp"
#include "String.hpp"

// A page map level 4 (PML4) contains 512 page directory pointer tables (PDP Tables)
// A PDP table is an array of 512 page ditectories (PDs)
// A PD contains 512 page tables (PTs)
// A PT contains 512 pages
//
// so the level of heirarchy is :
// 4 - PageMapLevel4
// 3 - PageDirectoryPointers
// 2 - PageDirectorys
// 1 - PageTables
// 0 - Pages
//
// each level is basically an array of uint64_t
// each uint64_t element in that array is address of a page
// each page can contain 512 uint64_t values
// so each level has 512 entries for a lower level
// each of this 512 entries represent a page and hence described by page table entry
// and a page table is just a collection of this 512 page table entries
// this means that each level is actually a page table and each entry in the table
// is a page table entry that stores address of lower level's page table


VirtualMemoryManager::VirtualMemoryManager(PageTable* pageMapLevel4):
pageMapLevel4(pageMapLevel4){}

void VirtualMemoryManager::MapMemory(uint64_t virtualAddress, uint64_t physicalAddress){
    uint64_t vaddr = virtualAddress;
    // get page index
    vaddr >>= 12;
    uint64_t pageIndex = vaddr & 0x1ff; // 0x1ff = 511 or 512 - 1

    // find table index
    vaddr >>= 9;
    uint64_t pageTableIndex = vaddr & 0x1ff;

    // find page directory index
    vaddr >>= 9;
    uint64_t pageDirectoryIndex = vaddr & 0x1ff;

    // find page directory pointer index
    // this index is used in page map level 4 array to point to a page directory
    vaddr >>= 9;
    uint64_t pageDirectoryPointerIndex = vaddr & 0x1ff;

    // don't need to initialize physicalAllocat for again and again
    // we only need an instance here
    static PhysicalMemoryManager physicalAllocator = {};

    // get page directory pointer from PML4
    PageDirectoryEntry pageDirectoryEntry = pageMapLevel4->entries[pageDirectoryPointerIndex];
    PageTable* pageDirectoryPointer;
    if(!pageDirectoryEntry.present){
        // create page directory pointer
        pageDirectoryPointer = reinterpret_cast<PageTable*>(physicalAllocator.AllocatePage());
        memset(reinterpret_cast<void*>(pageDirectoryPointer), 0, PAGE_SIZE);

        // shift by 12 bits to align it to 0x1000 boundary
        pageDirectoryEntry.address = reinterpret_cast<uint64_t>(pageDirectoryPointer) >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pageMapLevel4->entries[pageDirectoryPointerIndex] = pageDirectoryEntry;
    }else{
        pageDirectoryPointer = reinterpret_cast<PageTable*>(reinterpret_cast<uint64_t>(pageDirectoryEntry.address) << 12);
    }

    // get page directory from page directory entry
    pageDirectoryEntry = pageDirectoryPointer->entries[pageDirectoryIndex];
    PageTable* pageDirectory;
    if(!pageDirectoryEntry.present){
        // create page directory
        pageDirectory = reinterpret_cast<PageTable*>(physicalAllocator.AllocatePage());
        memset(reinterpret_cast<void*>(pageDirectory), 0, PAGE_SIZE);

        // shift by 12 bits to align it to 0x1000 boundary
        pageDirectoryEntry.address = reinterpret_cast<uint64_t>(pageDirectory) >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pageDirectoryPointer->entries[pageDirectoryIndex] = pageDirectoryEntry;
    }else{
        pageDirectory = reinterpret_cast<PageTable*>(reinterpret_cast<uint64_t>(pageDirectoryEntry.address) << 12);
    }

    // get page table from page directory
    pageDirectoryEntry = pageDirectory->entries[pageTableIndex];
    PageTable* pageTable;
    if(!pageDirectoryEntry.present){
        // create page table
        pageTable = reinterpret_cast<PageTable*>(physicalAllocator.AllocatePage());
        memset(reinterpret_cast<void*>(pageTable), 0, PAGE_SIZE);

        // shift by 12 bits to align it to 0x1000 boundary
        pageDirectoryEntry.address = reinterpret_cast<uint64_t>(pageTable) >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pageDirectory->entries[pageTableIndex] = pageDirectoryEntry;
    }else{
        pageTable = reinterpret_cast<PageTable*>(reinterpret_cast<uint64_t>(pageDirectoryEntry.address) << 12);
    }

    // get page from page table
    pageDirectoryEntry = pageTable->entries[pageIndex];
    pageDirectoryEntry.address = physicalAddress >> 12;
    pageDirectoryEntry.present = true;
    pageDirectoryEntry.readWrite = true;
    pageTable->entries[pageIndex] = pageDirectoryEntry;
}
