#/**
 *@file string.c
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 **/

#include "string.h"

// get length of string
size_t strlen(const char* str){
    size_t sz = 0;
    while(*(str + sz)){
        sz++;
    }

    return sz;
}
