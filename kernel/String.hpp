/**
 *@file String.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef STRING_HPP
#define STRING_HPP

#include <stddef.h>
#include <stdint.h>

// get the size of given null terminated string
size_t strlen(const char* str);
const char* itostr(int64_t n);
const char* utostr(uint64_t n);
const char* utohexstr(uint64_t n);

// memory functions
// set each byte of src memory to character c for size n
void memset(void* src, char c, size_t n);

#endif // STRING_HPP
