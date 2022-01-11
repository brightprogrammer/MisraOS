 /**
 *@file renderer.h
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some drawing on screen (framebuffer)
 **/

#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdint.h>
#include "font.h"

// basic framebuffer information
// a framebuffer is where renderer draws things
typedef struct {
    uint32_t *address;
    uint32_t width;
    uint32_t height;
} framebuffer_t;

// font renderer is responsible for drawing fonts on framebuffer
typedef struct {
    framebuffer_t framebuffer;
    font_t font;
    uint32_t foreground_colour;
    uint32_t background_colour;
    uint32_t xpos;
    uint32_t ypos;
} font_renderer_t;

// create a font renderer with normal settings
font_renderer_t create_font_renderer(framebuffer_t framebuffer);
// setter for default font renderer
void set_default_font_renderer(font_renderer_t* font_renderer);
// getter for default font renderer
font_renderer_t* get_default_font_renderer();

// draw character using the data in font_renderer
void renderer_draw_character(font_renderer_t* font_renderer, char c);
// draw string using the data in font_renderer
void renderer_draw_string(font_renderer_t* font_renderer, const char* str);

// draw a character c using the default renderer
// NOTE: You need to set a default renderer before using this
void draw_character(char c);
// draw a complete string using the default renderer
// NOTE: You need to set a default renderer before using this
void draw_string(const char* str);


#endif // RENDERER_H_
