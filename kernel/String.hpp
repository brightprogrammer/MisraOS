/**
 *@file String.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>
#include <cstdint>

// get the size of given null terminated string
size_t strlen(const char* str);
// convert 64 bit integer to string
const char* itostr(int64_t n);
// convert given uint64_t value to string
const char* utostr(uint64_t n);
// convert uint64_t to hex string, no 0x prefix is given
const char* utohexstr(uint64_t n);

// NOTE : the memory checks are not always byte by byte
// sometimes memory checks use uint64_t values too!
// so don't assume the interface to be same as std C/C++ interface

// compare first n bytes of memory of m1 and m2
int64_t memcmp(const void* m1, const void* m2, size_t n);
// set first n bytes of src to given character
void* memset(void *src, char c, size_t n);
// copy first n bytes of memory from src to dst
void* memcpy(void *dst, const void *src, size_t n);

// check whether two strings are same or not
int64_t strcmp(const char* s1, const char* s2);
// copy contents from src string to dst string
char *strcpy(char *dst, const char *src);
// concatenate src to dst's right side
char *strcat(char *dst, const char *src);
// check if given char is digit or not
bool isdigit(char c);

#endif // STRING_HPP
