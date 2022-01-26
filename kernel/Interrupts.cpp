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
#include "Panic.hpp"
#include "IO.hpp"

// without errcode
INTERRUPT_API void DefaultExceptionHandlerNoError(InterruptFrame* frame){
    Panic("REACHED DEFAULT EXCEPTION HANDLER!\n");

    // print information
    Panic(//"\tSTACK SEGMENT (SS) : %x\n"
          "\tCODE SEGMENT (CS) : %x\n"
          "\tINSTRUCTION POINTER (RIP) : %lx\n"
          "\tFLAGS REGISTER (RFLAGS) : %lx\n",
  //        "\tSTACK POINTER (RSP) : %lx\n",
          // frame->ss,
          frame->cs, frame->rip, frame->rflags // frame->rsp
        );
}

// with an errcode
INTERRUPT_API void DefaultExceptionHandlerWithError(InterruptFrame* frame, uint64_t errcode){
    Panic("REACHED DEFAULT EXCEPTION HANDLER!\n");

    // print information
    Panic(//"\tSTACK SEGMENT (SS) : %x\n"
          "\tCODE SEGMENT (CS) : %x\n"
          "\tINSTRUCTION POINTER (RIP) : %lx\n"
          "\tFLAGS REGISTER (RFLAGS) : %lx\n"
//          "\tSTACK POINTER (RSP) : %lx\n"
          "\tERROR CODE : %lu\n",
          // frame->ss,
          frame->cs, frame->rip, frame->rflags, // frame->rflags,
          errcode);
}

// without errcode
INTERRUPT_API void DefaultInterruptHandlerNoError(InterruptFrame* frame){
    Panic("REACHED DEFAULT INTERRUPT HANDLER!\n");

    // print information
    Panic(//"\tSTACK SEGMENT (SS) : %x\n"
          "\tCODE SEGMENT (CS) : %x\n"
          "\tINSTRUCTION POINTER (RIP) : %lx\n"
          "\tFLAGS REGISTER (RFLAGS) : %lx\n",
//          "\tSTACK POINTER (RSP) : %lx\n",
          // frame->ss,
          frame->cs, frame->rip, frame->rflags // frame->rsp
        );
}

// with an errcode
INTERRUPT_API void DefaultInterruptHandlerWithError(InterruptFrame* frame, uint64_t errcode){
    Panic("REACHED DEFAULT INTERRUPT HANDLER!\n");

    // print information
    Panic(//"\tSTACK SEGMENT (SS) : %x\n"
          "\tCODE SEGMENT (CS) : %x\n"
          "\tINSTRUCTION POINTER (RIP) : %lx\n"
          "\tFLAGS REGISTER (RFLAGS) : %lx\n"
          //"\tSTACK POINTER (RSP) : %lx\n"
          "\tERROR CODE : %lu\n",
          // frame->ss,
          frame->cs, frame->rip, frame->rflags, // frame->rsp,
          errcode);
}


// 0x08
INTERRUPT_API void DoubleFaultHandler(InterruptFrame* frame){
    Panic("Caught #DOUBLE_FAULT, halting execution...\n");

    // double fault can't be recovered from
    while(true) asm("hlt");
}

// 0x0d
INTERRUPT_API void GeneralProtectionFaultHandler(InterruptFrame* frame){
    Panic("Caught #GENERAL_PROTECTION_FAULT, halting execution...\n");
    while(true) asm("hlt");
}

// 0x0e
INTERRUPT_API void PageFaultHandler(InterruptFrame* frame){
    Panic("Caught #PAGE_FAULT, halting execution...\n");
    while(true) asm("hlt");
}

INTERRUPT_API void KeyboardInterruptHandler(InterruptFrame* frame){
    // 0x60 is the port at which ps2 keyboard is located
    uint8_t scancode = PortInByte(0x60);

}

// remap pic
void RemapPIC(){
    uint8_t bitmask_master, bitmask_slave;

    bitmask_master = PortInByte(PICMASTER_DATA);
    PortIOWait();
    bitmask_slave = PortInByte(PICSLAVE_DATA);
    PortIOWait();

    PortOutByte(PICMASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
    PortIOWait();
    PortOutByte(PICSLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
    PortIOWait();

    // map at 0x20 and 0x28 offsets in IDT
    // descriptors at these offsets will be used in IDT
    // whenever there's a PIC interrupt
    PortOutByte(PICMASTER_DATA, 0x20);
    PortIOWait();
    PortOutByte(PICSLAVE_DATA, 0x28);
    PortIOWait();

    // tell pic chips about their existence
    PortOutByte(PICMASTER_DATA, 4);
    PortIOWait();
    PortOutByte(PICSLAVE_DATA, 2);
    PortIOWait();

    // tell them to operate in 8086 mode now
    PortOutByte(PICMASTER_DATA, ICW4_8086);
    PortIOWait();
    PortOutByte(PICSLAVE_DATA, ICW4_8086);
    PortIOWait();

    PortOutByte(PICMASTER_DATA, bitmask_master);
    PortIOWait();
    PortOutByte(PICSLAVE_DATA, bitmask_slave);
}
