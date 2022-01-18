/**
 *@file Bitmap.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/14/2022
 *@brief Bitmap helper class
 r@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef UTILS_BITMAP_HPP
#define UTILS_BITMAP_HPP

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// bitmap helps you store 8 bools in 1 byte
struct Bitmap{
    // default constructor
    Bitmap() = default;

    // Create a new bitmap of given size and given buffer
    // Size is the size of buffer and not number of bits
    Bitmap(size_t size, uint8_t* buffer);

    // Get value of bit at given index.
    // idx is the index of bit
    bool operator [] (size_t idx);

    // Set value of bit to true at given index
    void SetBit(size_t idx);
    // Set value of bit to false at given index
    void UnsetBit(size_t idx);

    // size of buffer and not the number of booleans required
    size_t size;
    uint8_t *buffer;
};

#endif // UTILS_BITMAP_HPP
