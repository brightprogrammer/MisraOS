/**
 *@file Bitmap.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/14/2022
 *@brief Bitmap helps us store 8 bools in one byte.
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "Bitmap.hpp"

// create bitmap
Bitmap::Bitmap(size_t size, uint8_t* buffer)
: size(size), buffer(buffer){}

// set bit at given index
void Bitmap::SetBit(size_t idx){
    // check if given index is in range of bitmap size
    if(size < (idx / 8))
        return;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    // orring with 1 will always set it to 1
    buffer[buf_idx] = buffer[buf_idx] | (1 << bit_offset);
}

// unset bit at given index
void Bitmap::UnsetBit(size_t idx){
    // check if given index is in range of bitmap size
    if(size < (idx / 8))
        return;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    // and it with 0 to set it off
    buffer[buf_idx] = buffer[buf_idx] & (~(1 << bit_offset));
}

// if idx is out of range then false is returned
bool Bitmap::operator[](uint64_t idx){
    // check if index is in range
    if(size < (idx / 8))
        return false;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    return (buffer[buf_idx] >> bit_offset) & 0x1;
}
