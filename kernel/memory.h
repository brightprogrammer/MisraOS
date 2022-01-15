#ifndef MEMORY_H_
#define MEMORY_H_

#include<stdint.h>
#include "stivale2.h"

// init pmm, vmm and other components
void initMemoryManager(struct stivale2_struct_tag_memmap *mmap);

#endif // MEMORY_H_
