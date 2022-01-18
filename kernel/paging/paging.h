/**
 *@file paging.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief paging related data
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef PAGING_H_
#define PAGING_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {

    bool present : 1;

} PageDirectoryEntry;

#endif // PAGING_H_
