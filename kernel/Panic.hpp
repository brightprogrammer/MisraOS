#ifndef PANIC_HPP
#define PANIC_HPP

#include "Common.hpp"
#include "Renderer/Colors.hpp"

// colors used when printing panic message
#define PANIC_FGCOLOR COLOR_BLACK
#define PANIC_BGCOLOR COLOR_RED


// NOTE that after calling this function, caller registers wont be set back to nromal state
// so panic must only be called in absolute panic state
void PRINTF_API(1, 2) INTERRUPT_CALLEE_API PanicPrintf(const char* fmtstr, ...);

// panic puts
void INTERRUPT_CALLEE_API PanicPuts(const char* str);

#endif // PANIC_HPP
