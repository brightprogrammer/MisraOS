/**
 *@file Entry.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/22/2022
 *@brief Contains THE ACTUAL KERNEL ENTRY
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

#include "BootInfo.hpp"
#include "Util.hpp"

#include "../Renderer/FontRenderer.hpp"

// declare kenrel entry here and define it in KernelEntry.cpp
void KernelEntry();

// this is the ACTUAL KERNEL ENTRY POINT
// but we keep this as a bootloader part to seprarate bootloader from kernel
// in hopes that this will support modular design apporach
// this is just an approach to not make this bootloader a viral component of this kernel
// we can remove this anytime approach
extern "C" [[noreturn]] void Entry(stivale2_struct* tagList){
    // initialize this variable just to init boot info
    // this will load all things required by kernel to this struct
    // and then kernel can access parts from this
    BootInfo bootInfo(tagList);

    // setup font renderer before calling kernel entry
    CreateDefaultFontRenderer();

    // handover control to kernel
    KernelEntry();

    // hang if the kernel returns
    while(true){
        asm volatile("hlt");
    }
}
