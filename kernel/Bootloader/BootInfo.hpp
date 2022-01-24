/**
 *@file BootInfo.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/22/2022
 *@brief BootInfo class declarations
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

#ifndef BOOTINFO_HPP
#define BOOTINFO_HPP

#include "stivale2.h"
#include <cstdint>

// typedef for shorter name
typedef stivale2_mmap_entry MemMapEntry;

// contains boot information needed by the kernel
struct BootInfo {
    BootInfo() = default;
    BootInfo(stivale2_struct* tagList);

    static uint64_t GetFramebufferAddress();
    static uint16_t GetFramebufferPitch();
    static uint16_t GetFramebufferWidth();
    static uint16_t GetFramebufferHeight();

    static uint64_t GetKernelPhysicalBase();
    static uint64_t GetKernelVirtualBase();

    static uint64_t GetMemmapCount();
    static MemMapEntry* GetMemmap();
private:
    // framebuffer information
    static inline uint64_t fbAddr = 0;
    static inline uint16_t fbPitch = 0;
    static inline uint16_t fbWidth = 0;
    static inline uint16_t fbHeight = 0;

    // kernel information
    static inline uint64_t krnlPhysAddr = 0;
    static inline uint64_t krnlVirtAddr = 0;

    // store memory region related data
    static inline stivale2_struct_tag_memmap* memmap_tag = nullptr;
};

#endif // BOOTINFO_HPP
