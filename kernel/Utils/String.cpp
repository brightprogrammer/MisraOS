/**
 *@file string.c
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

#include "String.hpp"

static char int_to_string_buffer[128];

// get length of string
size_t strlen(const char* str){
    size_t sz = 0;
    while(*(str + sz)){
        sz++;
    }

    return sz;
}

// convert uint64_t to string
const char* itostr(int64_t n){
    // get size of string
    uint8_t digits = 1;

    uint8_t is_negative = 0;
    if(n < 0){
        is_negative = 1;
    }

    int_to_string_buffer[0] = '-';

    int64_t x = n;
    if(is_negative) x *= -1;
    while ((x /= 10) > 0)
        digits++;

    // convert to string
    x = n;
    if(is_negative) x *= -1;
    for(int8_t index = digits - 1; index >= 0; index--){
        int_to_string_buffer[index + is_negative] = (x % 10) + '0';
        x /= 10;
    }

    // null terminate string
    int_to_string_buffer[digits + is_negative] = '\0';

    return int_to_string_buffer;
}

// convert uint64_t to string
const char* utostr(uint64_t n){
    // get size of string
    uint8_t digits = 1;
    uint64_t x = n;
    while ((x /= 10) > 0)
        digits++;

    // convert to string
    x = n;
    for(int8_t index = digits - 1; index >= 0; index--){
        int_to_string_buffer[index] = (x % 10) + '0';
        x /= 10;
    }

    // null terminate string
    int_to_string_buffer[digits] = '\0';

    return int_to_string_buffer;
}

// convert uint64_t to hex string
const char* utohexstr(uint64_t n){
    // calculate size of hex string
    uint8_t size = 0;
    uint64_t x = n;
    while(x){
        x >>= 4;
        size++;
    }

    // hex digits in characters
    const char* hex_digits = "0123456789abcdef";

    x = n;
    for(int8_t i = size - 1; i >= 0; i--){
        // null terminate string in the end
        if(x == 0){
            int_to_string_buffer[size] = 0;
            // null terminate one more for safety
            int_to_string_buffer[size + 1] = 0;
            break;
        }

        // get lowest nibble and convert it to corresponding hex value
        uint8_t first_nibble = (uint8_t)x & 0x0f;
        int_to_string_buffer[i] = hex_digits[first_nibble];
        x = x >> 4;
    }

    int_to_string_buffer[size] = 0;
    // null terminate one more for safety
    int_to_string_buffer[size + 1] = 0;

    return int_to_string_buffer;
}

// compare two memory regions for n bytes
int64_t memcmp(const void* m1, const void* m2, size_t n){
    if(n > 8){
        // check remainder bytes
        const uint8_t* u8m1 = reinterpret_cast<const uint8_t*>(m1);
        const uint8_t* u8m2 = reinterpret_cast<const uint8_t*>(m2);
        size_t i = 0;
        for(i = 0; i < n % 8; i++){
            if(u8m1[i] != u8m2[i]) return (u8m1[i] - u8m2[i]);
        }

        // comparing 8 bytes at once will be faster since 64 bit register will
        // be used at once, this means less looping
        const uint64_t* u64m1 = reinterpret_cast<const uint64_t*>(u8m1 + i);
        const uint64_t* u64m2 = reinterpret_cast<const uint64_t*>(u8m2 + i);
        for(i = 0; i < n / 8 ; i++){
            if(u64m1[i] != u64m2[i]) return (u64m1[i] - u64m2[i]);
        }
    }else{
        // check remainder bytes
        const uint8_t* u8m1 = reinterpret_cast<const uint8_t*>(m1);
        const uint8_t* u8m2 = reinterpret_cast<const uint8_t*>(m2);
        size_t i = 0;
        for(i = 0; i < n; i++){
            if(u8m1[i] != u8m2[i]) return (u8m1[i] - u8m2[i]);
        }
    }

    return 0;
}

// copies the given byte to a bigger memory space by repeating it again and again
template<typename t>
t repeat_expand(char c){
    if(sizeof(t) == 64){
        return (c | c << 8 | c << 16 | c << 24 | uint64_t(c) << 32 | uint64_t(c) << 48 |  uint64_t(c) << 56);
    } else if (sizeof(t) == 32){
        return (c | c << 8 | c << 16 | c << 24);
    } else if (sizeof(t) == 16){
        return (c | c << 8);
    } else {
        return c;
    }
}

// memset
void* memset(void* dst, char c, size_t n){
    if(n > 8){
        // set remainder bytes in the beginning to given byte
        uint8_t* u8dst = reinterpret_cast<uint8_t*>(dst);
        size_t i = 0;
        for(i = 0; i < n % 8; i++){
            u8dst[i] = c;
        }

        // memsetting 8 bytes at once will be faster
        uint64_t* u64dst = reinterpret_cast<uint64_t*>(u8dst + i);
        uint64_t C = repeat_expand<uint64_t>(c);
        for(i = 0; i < n / 8 ; i++){
            u64dst[i] = C;
        }
    }else{
        // if less than 8 then just use byte by byte method
        uint8_t* u8dst = reinterpret_cast<uint8_t*>(dst);
        for(size_t i = 0; i < n; i++){
            u8dst[i] = c;
        }
    }

    return dst;
}

// copy memory from src to dst
void* memcpy(void *dst, const void* src, size_t n){
    if(n >= 8){
        // set remainder bytes in the beginning to given byte
        const uint8_t* u8src = reinterpret_cast<const uint8_t*>(src);
        uint8_t* u8dst = reinterpret_cast<uint8_t*>(dst);
        size_t i = 0;
        for(i = 0; i < n % 8; i++){
            u8dst[i] = u8src[i];
        }

        // memsetting 8 bytes at once will be faster
        const uint64_t* u64src = reinterpret_cast<const uint64_t*>(u8src + i);
        uint64_t* u64dst = reinterpret_cast<uint64_t*>(u8dst + i);
        for(i = 0; i < n / 8 ; i++){
            u64dst[i] = u64src[i];
        }
    }else{
        // if less than 8 then just use byte by byte method
        const uint8_t* u8src = reinterpret_cast<const uint8_t*>(src);
        uint8_t* u8dst = reinterpret_cast<uint8_t*>(dst);
        for(size_t i = 0; i < n; i++){
            u8dst[i] = u8src[i];
        }
    }

    return dst;
}

// check whether two strings are same or not
int64_t strcmp(const char* s1, const char* s2){
    if(strlen(s1) != strlen(s2)){
        return -1;
    }else{
        return memcmp(s1, s2, strlen(s1));
    }
}

// copy contents fromm src string to dst string
char* strcpy(char* dst, const char *src){
    return reinterpret_cast<char*>(memcpy(dst, src, strlen(src)));
}

// concatenate two strings
char* strcat(char* dst, const char* src){
    memcpy(dst + strlen(dst), src, strlen(src));
    dst[strlen(dst) + strlen(src) - 1] = 0;
    return dst;
}

// check if number (digit)
bool isdigit(char c){
    return (c >= '0') && (c <= '9');
}

// check if uppercase
bool isupper(char c){
    if(c >= 'A' && c <= 'Z'){
        return true;
    }

    return false;
}

// check if lowercase
bool islower(char c){
    if(c >= 'a' && c <= 'z'){
        return true;
    }

    return false;
}

// convert to uppercase
char toupper(char c){
    if(!isalpha(c)) return c;

    if(isupper(c)) return c;
    else{
        return 'A' + (c - 'a');
    }
}

// convert to lowercase
char tolower(char c){
    if(!isalpha(c)) return c;

    if(islower(c)) return c;
    else{
        return 'a' + (c - 'A');
    }
}

// check if alphabet
bool isalpha(char c){
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}


// check if alphabet or number
bool isalphanum(char c){
    return isalpha(c) || isdigit(c);
}
