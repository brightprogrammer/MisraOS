#include "Panic.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Colors.hpp"
#include "Renderer/FontRenderer.hpp"
#include "Utils/String.hpp"
#include <cstdint>
#include <cstdarg>
#include <cwctype>
#include "Printf.hpp"

uint32_t panicx = 0;
uint32_t panicy = 0;
bool panicmode = false;

void PRINTF_API(1, 2) INTERRUPT_CALLEE_API Panic(const char* fmtstr, ...){
    // clear screen
    if(!panicmode){
        Framebuffer::Clear(COLOR_RED);
        panicmode = true;
    }

    // reset position
    GetDefaultFontRenderer().xpos = panicx;
    GetDefaultFontRenderer().ypos = panicy;

    va_list vl;
    va_start(vl, fmtstr);
    Printf(fmtstr, vl);
    va_end(vl);

    panicx = GetDefaultFontRenderer().xpos;
    panicy = GetDefaultFontRenderer().ypos;
}
