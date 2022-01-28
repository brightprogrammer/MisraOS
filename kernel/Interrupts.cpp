/**
 *@file Interrupts.cpp
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

#include <cstdint>

#include "Interrupts.hpp"
#include "Keyboard.hpp"
#include "Panic.hpp"
#include "IO.hpp"


// without errcode
INTERRUPT_API void DefaultInterruptHandlerNoError(InterruptFrame* frame){
    PanicPrintf("REACHED DEFAULT INTERRUPT HANDLER!\n");

    // print information
    PanicPrintf("\tINSTRUCTION POINTER (RIP) : 0x%lx\n"
          "\tFLAGS REGISTER (RFLAGS) : 0x%lx\n"
          "\tCODE SEGMENT (CS) : 0x%x\n",
          frame->rip, frame->rflags, frame->cs);
}

// with an errcode
INTERRUPT_API void DefaultInterruptHandlerWithError(InterruptFrame* frame, uint64_t errcode){
    PanicPrintf("REACHED DEFAULT INTERRUPT HANDLER!\n");

    // print information
    PanicPrintf("\tINSTRUCTION POINTER (RIP) : 0x%lx\n"
          "\tCODE SEGMENT (CS) : 0x%x\n"
          "\tFLAGS REGISTER (RFLAGS) : 0x%lx\n"
          "\tSTACK POINTER (RSP) : 0x%lx\n"
          "\tSTACK SEGMENT (SS) : 0x%x\n"
          "\tERROR CODE : %lu\n",
          frame->rip, frame->cs, frame->rflags, frame->rsp, frame->ss, errcode);
}

// 0x08
INTERRUPT_API void DoubleFaultHandler(InterruptFrame* frame, uint64_t errorcode){
    PanicPrintf("Caught #DOUBLE_FAULT\n");

    PanicPrintf("\tINSTRUCTION POINTER (RIP) : 0x%lx\n"
          "\tCODE SEGMENT (CS) : 0x%x\n"
          "\tFLAGS REGISTER (RFLAGS) : 0x%lx\n"
          "\tSTACK POINTER (RSP) : 0x%lx\n"
          "\tSTACK SEGMENT (SS) : 0x%x\n"
          "\tERROR CODE : %lu\n",
          frame->rip, frame->cs, frame->rflags, frame->rsp, frame->ss, errorcode);

    // double fault can't be recovered from
    while(true) asm("hlt");
}

// 0x0d
INTERRUPT_API void GeneralProtectionFaultHandler(InterruptFrame* frame, uint64_t errorcode){
    PanicPrintf("Caught #GENERAL_PROTECTION_FAULT\n");

    PanicPrintf("\tINSTRUCTION POINTER (RIP) : 0x%lx\n"
          "\tCODE SEGMENT (CS) : 0x%x\n"
          "\tFLAGS REGISTER (RFLAGS) : 0x%lx\n"
          "\tSTACK POINTER (RSP) : 0x%lx\n"
          "\tSTACK SEGMENT (SS) : 0x%x\n"
          "\tERROR CODE : %lu\n",
          frame->rip, frame->cs, frame->rflags, frame->rsp, frame->ss, errorcode);

    while(true) asm("hlt");
}

// 0x0e
INTERRUPT_API void PageFaultHandler(InterruptFrame* frame, uint64_t errorcode){
    PanicPrintf("Caught #PAGE_FAULT\n");

    PanicPrintf("\tINSTRUCTION POINTER (RIP) : 0x%lx\n"
          "\tCODE SEGMENT (CS) : 0x%x\n"
          "\tFLAGS REGISTER (RFLAGS) : 0x%lx\n"
          "\tSTACK POINTER (RSP) : 0x%lx\n"
          "\tSTACK SEGMENT (SS) : 0x%x\n"
          "\tERROR CODE : %lu\n",
          frame->rip, frame->cs, frame->rflags, frame->rsp, frame->ss, errorcode);

    while(true) asm("hlt");
}

INTERRUPT_CALLEE_API void EndMasterPIC(){
    PortWriteByte(PICMASTER_COMMAND, PIC_EOI);
}

INTERRUPT_CALLEE_API void EndSlavePIC(){
    PortWriteByte(PICMASTER_COMMAND, PIC_EOI);
    PortWriteByte(PICSLAVE_COMMAND, PIC_EOI);
}

INTERRUPT_API void KeyboardInterruptHandler(InterruptFrame* frame){
    // 0x60 is the port at which ps2 keyboard is located
    uint8_t scancode = PortReadByte(0x60);
    HandleKeyboardEvent(scancode);
    EndMasterPIC();
}

// remap pic
void RemapPIC(){
    uint8_t bitmask_master, bitmask_slave;

    bitmask_master = PortReadByte(PICMASTER_DATA);
    PortIOWait();
    bitmask_slave = PortReadByte(PICSLAVE_DATA);
    PortIOWait();

    PortWriteByte(PICMASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
    PortIOWait();
    PortWriteByte(PICSLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
    PortIOWait();

    // map at 0x20 and 0x28 offsets in IDT
    // descriptors at these offsets will be used in IDT
    // whenever there's a PIC interrupt
    PortWriteByte(PICMASTER_DATA, 0x20);
    PortIOWait();
    PortWriteByte(PICSLAVE_DATA, 0x28);
    PortIOWait();

    // tell pic chips about their existence
    PortWriteByte(PICMASTER_DATA, 0b00000100);
    PortIOWait();
    PortWriteByte(PICSLAVE_DATA, 0b00000010);
    PortIOWait();

    // tell them to operate in 8086 mode now
    PortWriteByte(PICMASTER_DATA, ICW4_8086);
    PortIOWait();
    PortWriteByte(PICSLAVE_DATA, ICW4_8086);
    PortIOWait();

    PortWriteByte(PICMASTER_DATA, bitmask_master);
    PortIOWait();
    PortWriteByte(PICSLAVE_DATA, bitmask_slave);

    PortWriteByte(PICMASTER_DATA, 0b11111101);
    PortWriteByte(PICSLAVE_DATA, 0b11111111);

    // sets the interrupt flag in rflags/eflags register
    asm volatile ("sti");
}
