/**
 *@file bitmap.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/14/2022
 *@brief Bitmap helps us store 8 bools in one byte.
 r@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// bitmap helps you store 8 bools in 1 byte
typedef struct {
    // size of buffer and not the number of booleans required
    size_t size;
    uint8_t *buffer;
} Bitmap;

// get bitmap bit from given bitmap at given index
// if index is out of range the false is returned
bool bitmapGetBit(Bitmap *bitmap, uint64_t idx);
// set bitmap bit of given bitmap at given index
// if index is out of range then nothing is set (early return)
void bitmapSetBit(Bitmap *bitmap, uint64_t idx, bool value);

#endif // BITMAP_H_
