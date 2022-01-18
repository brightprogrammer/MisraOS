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
FontRenderer *defaultFontRenderer;

// create font renderer using default information
FontRenderer::FontRenderer(const Framebuffer& framebuffer) noexcept :
    framebuffer(framebuffer){
        font = {
            .bitmap = GetFontBitmap(),
            .font_width = FONT_WIDTH,
            .font_height = FONT_HEIGHT
        };

        foregroundColour = 0xff2997ff;
        backgroundColour = 0x00000000;
        xpos = 0;
        ypos = 0;
}

// create renderer with given data
FontRenderer::FontRenderer(const Framebuffer& framebuffer, const Font& font) noexcept :
    framebuffer(framebuffer), font(font){

    foregroundColour = 0xff2997ff;
    backgroundColour = 0x00000000;

    xpos = 0;
    ypos = 0;
}

// default font renderer
void SetDefaultFontRenderer(FontRenderer& fontRenderer){
    defaultFontRenderer = &fontRenderer;
}

// get font renderer
FontRenderer* GetDefaultFontRenderer(){
    return defaultFontRenderer;
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
