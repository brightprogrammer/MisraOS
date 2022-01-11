/**
 *@file string.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 **/

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

// get the size of given null terminated string
size_t strlen(const char* str);

#endif // STRING_H_
