/**
 *@file Printf.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/22/2022
 *@brief provides kprintf
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "Printf.hpp"

#include "Renderer/FontRenderer.hpp"
#include "String.hpp"
#include <cstdint>
#include <cstdarg>
#include <cwctype>

static char kprintf_buff[0x400] = {0};

__attribute__((format(printf, 1, 2))) int Printf(const char* fmtstr, ...){
    va_list vl;
    int i = 0, finalstrsz = 0;
    va_start(vl, fmtstr);
    while(fmtstr && fmtstr[i]){
        if(fmtstr[i] == '%'){
            i++;
            switch(fmtstr[i]){
            // print character
            case 'c': {
                kprintf_buff[finalstrsz] = static_cast<char>(va_arg(vl, int));
                finalstrsz++;
                break;
            }

            // print integer
            case 'd':{
                const char* tmp = utostr(va_arg(vl, int));
                strcpy(&kprintf_buff[finalstrsz], tmp);
                finalstrsz += strlen(tmp);
                break;
            }

            case 'x':{
                const char* tmp = utohexstr(va_arg(vl, uint32_t));
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
