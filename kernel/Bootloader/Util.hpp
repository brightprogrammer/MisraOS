#ifndef BOOTLOADER_UTIL_HPP
#define BOOTLOADER_UTIL_HPP

#include "stivale2.h"

// get stivale tag
// tagList is a stivale2_struct pointer given to kernel by the bootloader
// in the entry point. This can be enumerated to get different tags in the list
//
// tags are requested to the bootloader by the kernel and are returned by the bootloader
// to ther kernel in the form of a linked list.
void *StivaleGetTag(struct stivale2_struct *tagList, uint64_t id);

// halt system indefinitely
void InfiniteHalt();

#endif // BOOTLOADER_UTIL_HPP
