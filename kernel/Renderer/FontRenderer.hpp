 /**
 *@file renderer.h
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some drawing on screen (framebuffer)
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdint.h>
#include "Framebuffer.hpp"
#include "Font.hpp"

// size of single \t
#define TAB_SIZE 4

// font renderer is responsible for drawing fonts on framebuffer
struct FontRenderer{
    // this constructor will use default font
    FontRenderer(const Framebuffer& framebuffer) noexcept;

    // this constructor will use given font
    FontRenderer(const Framebuffer& framebuffer, const Font& font) noexcept;

    // draw a single character on the screen at (xpos, ypos)
    void DrawCharacter(char c);
    // draw a string whose first character is at (xpos, ypos)
    void DrawString(const char* str);

    // framebuffer to draw to
    Framebuffer framebuffer;

    // font to use for rendering
    Font font;

    // colour has 4 components
    // each component is 1 byte
    // the order of components is [alpha | red | green | blue] or (ARGB)
    uint32_t foregroundColour;
    uint32_t backgroundColour;

    // position of cursor
    uint32_t xpos;
    uint32_t ypos;
};

// setter for default font renderer
void SetDefaultFontRenderer(FontRenderer& fontRenderer);
// getter for default font renderer
FontRenderer* GetDefaultFontRenderer();

#endif // RENDERER_H_
