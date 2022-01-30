/**
 *@file Interrupts.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/26/2022
 *@brief Defines interrupt handlers
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

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define PICMASTER_COMMAND 0x20
#define PICMASTER_DATA 0x21
#define PICSLAVE_COMMAND 0xa0
#define PICSLAVE_DATA 0xa1
#define PIC_EOI 0x20

// reference : https://www.eeeguide.com/8259-programmable-interrupt-controller/
// ICWs are Initialization Command Words
// They are given to PIC (Programmable Interrupt Controller) during initialization
// ICW_1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#include <cstdint>
#include "Common.hpp"

// Reference : https://wiki.osdev.org/Exceptions

// this is made as defined in intel architecture manual
struct InterruptFrame {
    uint64_t rip;
    uint16_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint16_t ss;
} PACKED_STRUCT;

// default exception handlers
INTERRUPT_API void DefaultInterruptHandlerNoError(InterruptFrame* frame);
INTERRUPT_API void DefaultInterruptHandlerWithError(InterruptFrame* frame, uint64_t errcode);

// double fault = 0x08
// the processor always pushes 0 as errorcode
INTERRUPT_API void DoubleFaultHandler(InterruptFrame* frame, uint64_t errorcode);
// general protection fault = 0x0d
INTERRUPT_API void GeneralProtectionFaultHandler(InterruptFrame* frame, uint64_t errorcode);
// page fault handler = 0x0e
INTERRUPT_API void PageFaultHandler(InterruptFrame* frame, uint64_t errocode);
// keyboard interrupt handler
INTERRUPT_API void KeyboardInterruptHandler(InterruptFrame* frame);

// remap pic chip so that our interrupts don't collide with
// pic chip's interrupts
void RemapPIC();

#endif // INTERRUPTS_H_
