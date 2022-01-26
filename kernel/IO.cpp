#include "IO.hpp"

INTERRUPT_CALLEE_API void PortOutByte(uint16_t port, uint8_t value){
    asm volatile ("outb %0, %1"
                  :
                  : "a"(value), "Nd"(port));
}

INTERRUPT_CALLEE_API uint8_t PortInByte(uint16_t port){
    uint8_t ret;
    asm volatile ("inb %1, %0"
                  : "=a"(ret)
                  : "Nd"(port));

    return ret;
}

INTERRUPT_CALLEE_API void PortIOWait(){
    // write something into an unused port so that
    // other ports get time to catch up
    // this will waste a single IO cycle
    PortOutByte(0x80, 0);
}
