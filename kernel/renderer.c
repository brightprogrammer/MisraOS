/**
 *@file renderer.c
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some drawing on screen (framebuffer)
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "renderer.h"
#include "string.h"

// default font renderer
FontRenderer *defaultFontRenderer;

// create font renderer using default information
FontRenderer createFontRenderer(Framebuffer framebuffer){
    FontRenderer fontRenderer = {
        .framebuffer = framebuffer,
        .font = {
            .bitmap = getFontBitmapArray(),
            .font_width = FONT_WIDTH,
            .font_height = FONT_HEIGHT
        },
        .foregroundColour = 0xff2997ff,
        .backgroundColour = 0x00000000,
        .xpos = 0,
        .ypos = 0
    };

    return fontRenderer;
}

// default font renderer
void setDefaultFontRenderer(FontRenderer* fontRenderer){
    defaultFontRenderer = fontRenderer;
}

// get font renderer
FontRenderer* getDefaultFontRenderer(){
    return defaultFontRenderer;
}

// draw string at position in fontRenderer
void rendererDrawCharacter(FontRenderer *fontRenderer, char c){
    uint8_t font_width = fontRenderer->font.font_width;
    uint8_t font_height = fontRenderer->font.font_height;

    // get bitmap for required character
    // NOTE : This assumes that width of font is 8 pixels
    uint8_t* font_bitmap = fontRenderer->font.bitmap + c * font_height;

    for(uint32_t i = 0; i < font_height; i++){
        uint8_t row_bitmap = font_bitmap[i];
        for(uint32_t j = 0; j < font_width; j++){
            // calculate write address
            uint32_t write_addr = (fontRenderer->xpos + j) + (fontRenderer->ypos + i) * fontRenderer->framebuffer.width;

            // if bit is set then fill foreground colour else fill background colour
            // 8 - j because of endianness of pc when compiling. Bits are stored in little endian format
            if(row_bitmap & (1 << (8 - j))){
                fontRenderer->framebuffer.address[write_addr] = fontRenderer->foregroundColour;
            }else{
                fontRenderer->framebuffer.address[write_addr] = fontRenderer->backgroundColour;
            }
        }
    }

}

// draw string at position contained in fontRenderer
void rendererDrawString(FontRenderer* fontRenderer, const char* str){
    uint32_t strsz = strlen(str);
    for(uint32_t i = 0; i < strsz; i++){
        if(str[i] == '\n'){
            fontRenderer->xpos = 0;
            fontRenderer->ypos += fontRenderer->font.font_height;
            continue;
        }

        if(str[i] == '\t'){
            fontRenderer->xpos += TAB_SIZE * fontRenderer->font.font_width;
            continue;
        }

        // wrap text
        if(fontRenderer->xpos >= fontRenderer->framebuffer.width){
            fontRenderer->xpos = 0;
            fontRenderer->ypos += fontRenderer->font.font_height;
        }

        if(fontRenderer->ypos >= fontRenderer->framebuffer.height){
            fontRenderer->ypos = 0;
            fontRenderer->xpos = 0;
        }

        // draw character
        rendererDrawCharacter(fontRenderer, str[i]);

        // keep increasing position
        fontRenderer->xpos += fontRenderer->font.font_width;
    }
}

// draw a single character on framebuffer at given position
void drawCharacter(char c){
    rendererDrawCharacter(defaultFontRenderer, c);
}

// draw a string at given position
void drawString(const char* str){
    rendererDrawString(defaultFontRenderer, str);
}
