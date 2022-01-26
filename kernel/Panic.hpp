#ifndef PANIC_HPP
#define PANIC_HPP

#include "Common.hpp"

// panic function
// if panicscreen is true then panic message will be printed on
// a crated panic screen with full red background and panic message at the top
//
// NOTE that after calling this function, caller registers wont be set back to nromal state
// so panic must only be called in absolute panic state
void PRINTF_API(1, 2) INTERRUPT_CALLEE_API Panic(const char* fmtstr, ...);

#endif // PANIC_HPP
