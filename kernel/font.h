/**
 *@file font.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/09/2022
 *@brief Manage fonts.
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY 3.0
 **/

#ifndef FONT_H_
#define FONT_H_

#include <stdint.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

// return the statically defined font array
uint8_t* get_font_bitmap_array();

// contains basic font related information
typedef struct {
    uint8_t *bitmap;
    uint8_t font_width;
    uint8_t font_height;
} font_t;

#endif // FONT_H_
