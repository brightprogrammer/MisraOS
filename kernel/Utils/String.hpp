/**
 *@file String.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 *@copyright BSD 3-Clause License

 Copyright (c) 2022, Siddharth Mishra
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
// check if given character is in upper case or not
bool isupper(char c);
// check if given char is in lower case or not
bool islower(char c);
// convert to upper case
char toupper(char c);
// convert to lower case
char tolower(char c);
// check if given char is alphabet
bool isalpha(char c);
// check if givne char is alphabet or number
bool isalphanum(char c);

#endif // STRING_HPP
