 /**
 *@file renderer.h
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some drawing on screen (framebuffer)
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY 3.0
 **/

#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdint.h>
#include "font.h"

// size of single \t
#define TAB_SIZE 4

// basic framebuffer information
// a framebuffer is where renderer draws things
typedef struct {
    uint32_t *address;
    uint32_t width;
    uint32_t height;
} Framebuffer;

// font renderer is responsible for drawing fonts on framebuffer
typedef struct {
    Framebuffer framebuffer;
    Font font;
    uint32_t foregroundColour;
    uint32_t backgroundColour;
    uint32_t xpos;
    uint32_t ypos;
} FontRenderer;

// create a font renderer with normal settings
FontRenderer createFontRenderer(Framebuffer framebuffer);
// setter for default font renderer
void setDefaultFontRenderer(FontRenderer* fontRenderer);
// getter for default font renderer
FontRenderer* getDefaultFontRenderer();

// draw character using the data in fontRenderer
void rendererDrawCharacter(FontRenderer* fontRenderer, char c);
// draw string using the data in fontRenderer
void rendererDrawString(FontRenderer* fontRenderer, const char* str);

// draw a character c using the default renderer
// NOTE: You need to set a default renderer before using this
void drawCharacter(char c);
// draw a complete string using the default renderer
// NOTE: You need to set a default renderer before using this
void drawString(const char* str);

#endif // RENDERER_H_
