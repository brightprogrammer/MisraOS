#include "Panic.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Colors.hpp"
#include "Renderer/FontRenderer.hpp"
#include "Utils/String.hpp"
#include <cstdint>
#include <cstdarg>
#include <cwctype>
#include "Printf.hpp"

uint32_t oldbgcolor = 0;
uint32_t oldfgcolor = 0;

void PRINTF_API(1, 2) INTERRUPT_CALLEE_API PanicPrintf(const char* fmtstr, ...){
    // shorter name "r"
    FontRenderer& r = GetDefaultFontRenderer();

    // cache colors
    oldbgcolor = r.backgroundColour;
    oldfgcolor = r.foregroundColour;

    // change to panic colors
    r.backgroundColour = PANIC_BGCOLOR;
    r.foregroundColour = PANIC_FGCOLOR;

    // print
    va_list vl;
    va_start(vl, fmtstr);
    Printf(fmtstr, vl);
    va_end(vl);

    // restore colors
    r.backgroundColour = oldbgcolor;
    r.foregroundColour = oldfgcolor;
}

// normal print without formatting
void INTERRUPT_CALLEE_API PanicPuts(const char* str){
    // shorter name "r"
    FontRenderer& r = GetDefaultFontRenderer();

    // cache colors
    oldbgcolor = r.backgroundColour;
    oldfgcolor = r.foregroundColour;

    // change colors to panic colros
    r.backgroundColour = PANIC_BGCOLOR;
    r.foregroundColour = PANIC_FGCOLOR;

    // draw string
    DrawString(str);

    // restore colors
    r.backgroundColour = oldbgcolor;
    r.foregroundColour = oldfgcolor;
}
