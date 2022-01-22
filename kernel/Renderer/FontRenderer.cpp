/**
 *@file Renderer.cpp
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Renders font on the screen (or framebuffer)
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "FontRenderer.hpp"
#include "../String.hpp"

// default font renderer
static FontRenderer DefaultFontRenderer;

void CreateDefaultFontRenderer(){
    DefaultFontRenderer = FontRenderer();
}

// default font renderer
void SetDefaultFontRenderer(FontRenderer& fontRenderer){
    DefaultFontRenderer = fontRenderer;
}

FontRenderer& GetDefaultFontRenderer(){
    return DefaultFontRenderer;
}

FontRenderer::FontRenderer(){
    framebuffer = Framebuffer();
    font = Font();
}

// draw string at position in fontRenderer
void FontRenderer::DrawCharacter(char c){
    uint8_t font_width = font.font_width;
    uint8_t font_height = font.font_height;

    // get bitmap for required character
    // NOTE : This assumes that width of font is 8 pixels
    uint8_t* font_bitmap = font.bitmap.buffer + c * font_height;

    for(uint32_t i = 0; i < font_height; i++){
        uint8_t row_bitmap = font_bitmap[i];
        for(uint32_t j = 0; j < font_width; j++){
            // calculate write address
            uint32_t write_addr = (xpos + j) + (ypos + i) * framebuffer.width;

            // if bit is set then fill foreground colour else fill background colour
            // 8 - j because of endianness of pc when compiling. Bits are stored in little endian format
            if(row_bitmap & (1 << (8 - j))){
                framebuffer.address[write_addr] = foregroundColour;
            }else{
                framebuffer.address[write_addr] = backgroundColour;
            }
        }
    }
}

// draw string at position contained in fontRenderer
void FontRenderer::DrawString(const char* str){
    uint32_t strsz = strlen(str);
    for(uint32_t i = 0; i < strsz; i++){
        if(str[i] == '\n'){
            xpos = 0;
            ypos += font.font_height;
            continue;
        }

        if(str[i] == '\t'){
            xpos += TAB_SIZE * font.font_width;
            continue;
        }

        // wrap text
        if(xpos >= framebuffer.width){
            xpos = 0;
            ypos += font.font_height;
        }

        if(ypos >= framebuffer.height){
            ypos = 0;
            xpos = 0;
        }

        // draw character
        DrawCharacter(str[i]);

        // keep increasing position
        xpos += font.font_width;
    }
}

// draw character
void DrawCharacter(char c){
    DefaultFontRenderer.DrawCharacter(c);
}

// draw string
void DrawString(const char* str){
    DefaultFontRenderer.DrawString(str);
}
