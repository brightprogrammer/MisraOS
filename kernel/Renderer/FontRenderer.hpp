 /**
 *@file FontRenderer.hpp
 *@author Siddharth Mishra
 *@date 01/09/2022
 *@brief Do some font drawing on screen (framebuffer)
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
