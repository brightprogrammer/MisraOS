/**
 *@file string.c
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/10/22
 *@brief Provide basic string utilities to kernel
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

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

// memset
void memset(void* src, char c, size_t n){
    uint8_t* u8src = (uint8_t*)src;
    for(size_t i = 0; i < n; i++){
        u8src[i] = c;
    }
}
