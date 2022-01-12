/**
 *@file string.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 **/

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>
#include <stdint.h>

// get the size of given null terminated string
size_t strlen(const char* str);
const char* itostr(int64_t n);
const char* utostr(uint64_t n);
const char* utohexstr(uint64_t n);

#endif // STRING_H_
