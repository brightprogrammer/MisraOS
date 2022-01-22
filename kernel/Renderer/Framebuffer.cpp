/**
 *@file Framebuffer.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief Framebuffer helper class
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#include "Framebuffer.hpp"
#include "../Bootloader/BootInfo.hpp"

// default constructor uses boot info
Framebuffer::Framebuffer(){
    address = reinterpret_cast<uint32_t*>(BootInfo::GetFramebufferAddress());
    width = BootInfo::GetFramebufferWidth();
    height = BootInfo::GetFramebufferHeight();
    pitch = BootInfo::GetFramebufferPitch();
}

// define operator
uint32_t& Framebuffer::operator()(uint32_t r, uint32_t c){
    return address[r * width + c];
}
