#include "Interrupts.hpp"
#include "Printf.hpp"

__attribute__((interrupt)) void PageFaultHandler(struct InterruptFrame* frame){
    Printf("Catched a #PAGE_FAULT\n Halting execution...\n");
    // halt execution
    while(true) asm volatile ("hlt");
}
