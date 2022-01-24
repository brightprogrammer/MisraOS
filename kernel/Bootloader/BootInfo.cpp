/**
 *@file BootInfo.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/22/2022
 *@brief BootInfo class definitions
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

static BootInfo bootInfo = {};

BootInfo::BootInfo(stivale2_struct* stivaleTagList){
    // get framebuffer tag
    stivale2_struct_tag_framebuffer *fb_tag = reinterpret_cast<stivale2_struct_tag_framebuffer*>(StivaleGetTag(stivaleTagList, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID));
    // stivale 2 spec states that bootloader will boot even if the framebuffer wasn't found!
    if (fb_tag == nullptr) {
        InfiniteHalt();
    }


    // fill framebuffer related information
    fbAddr = fb_tag->framebuffer_addr;
    fbWidth = fb_tag->framebuffer_width;
    fbHeight = fb_tag->framebuffer_height;
    fbPitch = fb_tag->framebuffer_pitch;

    // get kernel related information
    stivale2_struct_tag_kernel_base_address *kaddr_tag = reinterpret_cast<stivale2_struct_tag_kernel_base_address*>(StivaleGetTag(stivaleTagList, STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID));
    krnlPhysAddr = kaddr_tag->physical_base_address;
    krnlVirtAddr = kaddr_tag->virtual_base_address;

    // get memmap tag
    memmap_tag = reinterpret_cast<stivale2_struct_tag_memmap*>(StivaleGetTag(stivaleTagList, STIVALE2_STRUCT_TAG_MEMMAP_ID));
}

uint64_t BootInfo::GetFramebufferAddress(){ return fbAddr; }
uint16_t BootInfo::GetFramebufferWidth(){ return fbWidth; }
uint16_t BootInfo::GetFramebufferHeight(){ return fbHeight; }
uint16_t BootInfo::GetFramebufferPitch(){ return fbPitch; }

uint64_t BootInfo::GetKernelPhysicalBase(){ return krnlPhysAddr; }
uint64_t BootInfo::GetKernelVirtualBase(){ return krnlVirtAddr; }

uint64_t BootInfo::GetMemmapCount(){ return memmap_tag->entries; }
MemMapEntry* BootInfo::GetMemmap(){ return memmap_tag->memmap; }
