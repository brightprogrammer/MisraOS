#include "VirtualMemoryManager.hpp"
#include "PhysicalMemoryManager.hpp"
#include "String.hpp"
#include "Print.hpp"

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


VirtualMemoryManager::VirtualMemoryManager(PageTable* pageMapTable){
    pml4 = pageMapTable;
}

// get next level of paging
PageTable* VirtualMemoryManager::GetNextLevel(PageTable* pageTable, uint64_t entryIndex, bool allocate){
    PageDirectoryEntry* pageDirectoryEntry = &pageTable->entries[entryIndex];
    PageTable* pageDirectoryPointer = nullptr;

    // if page directory entry is not present and allocation is allowed, then allocate it
    if(!pageDirectoryEntry->present){
        // if allocation isn't allowed then return nullptr
        if(!allocate){
            return nullptr;
        }

        // create page directory pointer
        pageDirectoryPointer = reinterpret_cast<PageTable*>(pmm.AllocatePage());
        memset(reinterpret_cast<void*>(pageDirectoryPointer), 0, PAGE_SIZE);

        // shift by 12 bits to align it to 0x1000 boundary
        pageDirectoryEntry->address = reinterpret_cast<uint64_t>(pageDirectoryPointer) >> 12;
        pageDirectoryEntry->present = true;
        pageDirectoryEntry->readWrite = true;
    }else{
        pageDirectoryPointer = reinterpret_cast<PageTable*>(pageDirectoryEntry->address << 12);
    }

    return pageDirectoryPointer;
}

void VirtualMemoryManager::MapMemory(uint64_t virtualAddress, uint64_t physicalAddress){
    uint64_t vaddr = virtualAddress;
    // get page index
    vaddr >>= 12;
    uint64_t pageIndex = vaddr & 0x1ff; // 0x1ff = 511 or 512 - 1

    // find table index
    vaddr >>= 9;
    uint64_t pml1Index = vaddr & 0x1ff;

    // find page directory index
    vaddr >>= 9;
    uint64_t pml2Index = vaddr & 0x1ff;

    // find page directory pointer index
    // this index is used in page map level 4 array to point to a page directory
    vaddr >>= 9;
    uint64_t pml3Index = vaddr & 0x1ff;

    // get page directory pointer from PML4
    PageTable* pml3 = GetNextLevel(pml4, pml3Index, true);
    if(pml3 == nullptr){
        PrintError("Failed to get PML3\n");
    }

    // get page directory from page directory pointer
    PageTable* pml2 = GetNextLevel(pml3, pml2Index, true);
    if(pml2 == nullptr){
        PrintError("Failed to get PML2\n");
    }


    // get page table from page directory
    PageTable* pml1 = GetNextLevel(pml2, pml1Index, true);
    if(pml1 == nullptr){
        PrintError("Failed to get PML1\n");
    }


    // get page from page table
    PageDirectoryEntry *pde = &pml1->entries[pageIndex];
    if(pde == nullptr){
        PrintError("Failed to get Page\n");
    }

    pde->address = (physicalAddress >> 12);
    pde->present = true;
    pde->readWrite = true;
}
