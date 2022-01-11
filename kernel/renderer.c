/**
 *@file renderer.c
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some drawing on screen (framebuffer)
 **/

#include "renderer.h"
#include "string.h"

// default font renderer
font_renderer_t *default_font_renderer;

// create font renderer using default information
font_renderer_t create_font_renderer(framebuffer_t framebuffer){
    font_renderer_t font_renderer = {
        .framebuffer = framebuffer,
        .font = {
            .bitmap = get_font_bitmap_array(),
            .font_width = FONT_WIDTH,
            .font_height = FONT_HEIGHT
        },
        .foreground_colour = 0xff2997ff,
        .background_colour = 0x00000000,
        .xpos = 0,
        .ypos = 0
    };

    return font_renderer;
}

// default font renderer
void set_default_font_renderer(font_renderer_t* font_renderer){
    default_font_renderer = font_renderer;
}

// get font renderer
font_renderer_t* get_default_font_renderer(){
    return default_font_renderer;
}

// draw string at position in font_renderer
void renderer_draw_character(font_renderer_t *font_renderer, char c){
    uint8_t font_width = font_renderer->font.font_width;
    uint8_t font_height = font_renderer->font.font_height;

    // get bitmap for required character
    // NOTE : This assumes that width of font is 8 pixels
    uint8_t* font_bitmap = font_renderer->font.bitmap + c * font_height;

    for(uint32_t i = 0; i < font_height; i++){
        uint8_t row_bitmap = font_bitmap[i];
        for(uint32_t j = 0; j < font_width; j++){
            // calculate write address
            uint32_t write_addr = (font_renderer->xpos + j) + (font_renderer->ypos + i) * font_renderer->framebuffer.width;

            // if bit is set then fill foreground colour else fill background colour
            // 8 - j because of endianness of pc when compiling. Bits are stored in little endian format
            if(row_bitmap & (1 << (8 - j))){
                font_renderer->framebuffer.address[write_addr] = font_renderer->foreground_colour;
            }else{
                font_renderer->framebuffer.address[write_addr] = font_renderer->background_colour;
            }
        }
    }

}

// draw string at position contained in font_renderer
void renderer_draw_string(font_renderer_t* font_renderer, const char* str){
    uint32_t strsz = strlen(str);
    for(uint32_t i = 0; i < strsz; i++){
        if(str[i] == '\n'){
            font_renderer->xpos = 0;
            font_renderer->ypos += font_renderer->font.font_height;
            continue;
        }

        // draw character
        renderer_draw_character(font_renderer, str[i]);

        // keep increasing position
        font_renderer->xpos += font_renderer->font.font_width;

        // wrap text
        if(font_renderer->xpos >= font_renderer->framebuffer.width){
            font_renderer->xpos = 0;
            font_renderer->ypos += font_renderer->font.font_height;
        }

        if(font_renderer->ypos >= font_renderer->framebuffer.height){
            font_renderer->ypos = 0;
        }
    }
}

// draw a single character on framebuffer at given position
void draw_character(char c){
    renderer_draw_character(default_font_renderer, c);
}

// draw a string at given position
void draw_string(const char* str){
    renderer_draw_string(default_font_renderer, str);
}
