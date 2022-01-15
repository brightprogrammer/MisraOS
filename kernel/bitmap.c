/**
 *@file bitmap.c
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/14/2022
 *@brief Bitmap helps us store 8 bools in one byte.
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "bitmap.h"

void bitmapSetBit(Bitmap *bitmap, uint64_t idx, bool value){
    // check if given index is in range of bitmap size
    if(bitmap->size < (idx / 8))
        return;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    if(value == true){
        // orring with 1 will always set it to 1
        bitmap->buffer[buf_idx] = bitmap->buffer[buf_idx] | (1 << bit_offset);
    }else{
        // if the bit is already flagged then xor it with 1 to set it off
        // else leave it off
        bitmap->buffer[buf_idx] = bitmap->buffer[buf_idx] & (~(1 << bit_offset));
    }
}

// if idx is out of range then false is returned
bool bitmapGetBit(Bitmap *bitmap, uint64_t idx){
    // check if index is in range
    if(bitmap->size < (idx / 8))
        return false;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    return (bitmap->buffer[buf_idx] >> bit_offset) & 0x1;
}
