/**
 *@file font.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/09/2022
 *@brief Manage fonts.
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef FONT_HPP
#define FONT_HPP

#include <stdint.h>
#include "../Utils/Bitmap.hpp"

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

// return the statically defined font array
Bitmap GetFontBitmap();

// contains basic font related information
struct Font{
    Bitmap bitmap;
    uint8_t font_width;
    uint8_t font_height;
};

#endif // FONT_HPP
