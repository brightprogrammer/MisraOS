 /**
 *@file renderer.h
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some drawing on screen (framebuffer)
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <stdint.h>
#include "Framebuffer.hpp"
#include "Font.hpp"
#include "Colors.hpp"

// size of single \t
#define TAB_SIZE 4

// font renderer is responsible for drawing fonts on framebuffer
struct FontRenderer{
    // default renderer
    // use framebuffer given in the boot info
    FontRenderer();

    // draw a single character on the screen at (xpos, ypos)
    void DrawCharacter(char c);
    // draw a string whose first character is at (xpos, ypos)
    void DrawString(const char* str);

    uint32_t foregroundColour = COLOR_CAFEBABE;
    uint32_t backgroundColour = COLOR_BLACK;

    // position of cursor
    uint32_t xpos = 0;
    uint32_t ypos = 0;

private:
    // famebuffer to draw to
    Framebuffer framebuffer;
    // font to use for rendering
    Font font;
};

// draw single character using defualt renderer
void DrawCharacter(char c);
// draw a string using default renderer
void DrawString(const char* str);

// create default font renderer
void CreateDefaultFontRenderer();
// setter for default font renderer
void SetDefaultFontRenderer(FontRenderer& fontRenderer);
// getter for default font renderer
FontRenderer& GetDefaultFontRenderer();

#endif // RENDERER_HPP
