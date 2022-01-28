/**
 *@file Puts.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/28/2022
 *@brief Define simple puts function that prints every text to a new line
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

#include "Puts.hpp"
#include "Renderer/FontRenderer.hpp"

// draw a string without any formatting
void Puts(const char* str){
    DrawString(str);
}

// puts but with a color
void ColorPuts(uint32_t fgcolor, uint32_t bgcolor, const char* str){
    FontRenderer& r = GetDefaultFontRenderer();

    // store old colors
    uint32_t oldbg = r.backgroundColour;
    uint32_t oldfg = r.foregroundColour;

    // set new colors
    r.backgroundColour = bgcolor;
    r.foregroundColour = fgcolor;

    // draw with new colors
    r.DrawString(str);

    // reset to old colors
    r.backgroundColour = oldbg;
    r.foregroundColour = oldfg;
}

// draw a string without any formatting
void PutChar(char c){
    if(!c){
        return;
    }

    DrawCharacter(c);
}

static uint32_t oldbg, oldfg;
// puts but with a color
void ColorPutChar(uint32_t fgcolor, uint32_t bgcolor, char c){
    if(!c){
        return;
    }

    FontRenderer& r = GetDefaultFontRenderer();

    // store old colors
    oldbg = r.backgroundColour;
    oldfg = r.foregroundColour;

    // set new colors
    r.backgroundColour = bgcolor;
    r.foregroundColour = fgcolor;

    // draw with new colors
    r.DrawCharacter(c);

    // reset to old colors
    r.backgroundColour = oldbg;
    r.foregroundColour = oldfg;
}
