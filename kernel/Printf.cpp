/**
 *@file Printf.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/22/2022
 *@brief provides kprintf
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

#include "Printf.hpp"

#include "Renderer/FontRenderer.hpp"
#include "Utils/String.hpp"
#include <cstdint>
#include <cstdarg>
#include <cwctype>

static char kprintf_buff[0x400] = {0};

int PRINTF_API(1, 2) Printf(const char* fmtstr, ...){
    va_list vl;
    int i = 0, finalstrsz = 0;
    va_start(vl, fmtstr);

    bool longEnable = false;

    while(fmtstr && fmtstr[i]){
        // check if any format is present
        if(fmtstr[i] == '%'){
            i++;

            // check for long mode print
            if(fmtstr[i] == 'l'){
                longEnable = true;
                i++;
            }

            switch(fmtstr[i]){
            // print character
            case 'c': {
                kprintf_buff[finalstrsz] = static_cast<char>(va_arg(vl, int));
                finalstrsz++;
                break;
            }

            // print integer
            case 'i':{
                const char* tmp = nullptr;
                if(longEnable){
                    tmp = itostr(va_arg(vl, int64_t));
                    longEnable = false;
                }else{
                    tmp = itostr(va_arg(vl, int32_t));
                }

                strcpy(&kprintf_buff[finalstrsz], tmp);
                finalstrsz += strlen(tmp);
                break;
            }

            // print integer
            case 'u':{
                const char* tmp = nullptr;
                if(longEnable){
                    tmp = utostr(va_arg(vl, uint64_t));
                    longEnable = false;
                }else{
                    tmp = utostr(va_arg(vl, uint32_t));
                }

                strcpy(&kprintf_buff[finalstrsz], tmp);
                finalstrsz += strlen(tmp);
                break;
            }

            case 'x':{
                const char* tmp = nullptr;
                if(longEnable){
                    tmp = utohexstr(va_arg(vl, int64_t));
                    longEnable = false;
                }else{
                    tmp = utohexstr(va_arg(vl, int32_t));
                }

                strcpy(&kprintf_buff[finalstrsz], tmp);
                finalstrsz += strlen(tmp);
                break;
            }

            case 's':{
                char* tmp = va_arg(vl, char*);
                strcat(kprintf_buff, tmp);
                finalstrsz += strlen(tmp);
                break;
            }
            }
        }else{
            kprintf_buff[finalstrsz] = fmtstr[i];
            finalstrsz++;
        }

        i++;
    }
    kprintf_buff[finalstrsz] = 0;

    // print the string now
    DrawString(kprintf_buff);

    va_end(vl);
    return finalstrsz;
}

int PRINTF_API(3, 4) ColorPrintf(uint32_t fgcolor, uint32_t bgcolor, const char* fmtstr, ...){
    FontRenderer& r = GetDefaultFontRenderer();
    const uint32_t oldfgcolor = r.foregroundColour;
    const uint32_t oldbgcolor = r.backgroundColour;

    r.backgroundColour = bgcolor;
    r.foregroundColour = fgcolor;

    va_list vl;
    va_start(vl, fmtstr);
    int res = Printf(fmtstr, vl);
    va_end(vl);

    r.backgroundColour = oldbgcolor;
    r.foregroundColour = oldfgcolor;

    return res;
}
