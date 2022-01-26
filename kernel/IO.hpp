#ifndef IO_HPP
#define IO_HPP

#include <cstdint>

// all functions that are going to be called within any interrupt handler
// must have this attribute
#define INTERRUPT_CALLEE_API __attribute__((no_caller_saved_registers))

// put a byte on to a I/O bus
// port selects the device on the I/O bus to talk to
// value is the value that we want to give to the device
INTERRUPT_CALLEE_API void PortOutByte(uint16_t port, uint8_t value);

// get byte from port
INTERRUPT_CALLEE_API uint8_t PortInByte(uint16_t port);

// wait for small time
// on older machines, i/o ports are slow
INTERRUPT_CALLEE_API void PortIOWait();

#endif // IO_HPP
