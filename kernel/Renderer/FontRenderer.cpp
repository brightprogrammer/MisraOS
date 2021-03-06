/**
 *@file Renderer.cpp
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Renders font on the screen (or framebuffer)
 *@copyright BSD 3-Clause License

 Copyright (c) 2022, Siddharth Mishra
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "FontRenderer.hpp"
#include "../Utils/String.hpp"

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
static uint32_t lastLineSize = 0;
void FontRenderer::DrawCharacter(char c){
    uint8_t width = font.width;
    uint8_t height = font.height;

    // get bitmap for required character
    uint8_t* font_bitmap = font.bitmap.buffer + c * height;

    if(c == '\n'){
        lastLineSize = xpos;
        xpos = 0;
        ypos += height;
        return;
    } else if(c == '\t'){
        xpos += TAB_SIZE * width;
        return;
    } else if(c == '\b'){
        if(xpos != 0){
            xpos -= width;
        }else{
            xpos = lastLineSize;
            if(ypos) ypos -= height;
        }
    }

    // wrap text
    if(xpos >= framebuffer.width){
        lastLineSize = framebuffer.width;
        xpos = 0;
        ypos += font.height;
    }

    if(ypos >= framebuffer.height){
        ypos = 0;
        xpos = 0;
    }

    for(uint32_t i = 0; i < height; i++){
        uint8_t row_bitmap = font_bitmap[i];
        for(uint32_t j = 0; j < width; j++){
            // calculate write address
            uint32_t write_addr = (xpos + j) + (ypos + i) * framebuffer.width;

            if(c == '\b'){
                framebuffer.address[write_addr] = backgroundColour;
            }else {
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

    if(c != '\b'){
        // keep increasing position
        xpos += font.width;
    }
}

// draw string at position contained in fontRenderer
void FontRenderer::DrawString(const char* str){
    uint32_t strsz = strlen(str);
    for(uint32_t i = 0; i < strsz; i++){
        // draw character
        DrawCharacter(str[i]);
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
