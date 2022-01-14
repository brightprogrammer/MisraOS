/**
 *@file kernel.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief kernel header file
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY 3.0
 **/

#ifndef KERNEL_H_
#define KERNEL_H_

// we can't include most of the headers
// only selected headers ans others require libc
// which we won't have when our kernel boots up
#include <stdint.h>
#include <stddef.h>

// stivale 2 specification header
#include "stivale2.h"

// we can't include most of the headers
// only selected headers ans others require libc
// which we won't have when our kernel boots up
#define NULLADDR 0

#endif // KERNEL_H_
