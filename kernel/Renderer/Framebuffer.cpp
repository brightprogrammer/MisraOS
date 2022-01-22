#include "Framebuffer.hpp"

// constructor
Framebuffer::Framebuffer(stivale2_struct_tag_framebuffer* framebuffer_tag){
    address = reinterpret_cast<uint32_t*>(framebuffer_tag->framebuffer_addr);
    width = framebuffer_tag->framebuffer_width;
    height = framebuffer_tag->framebuffer_height;
    pitch = framebuffer_tag->framebuffer_pitch;
}

// define operator
uint32_t& Framebuffer::operator()(uint32_t r, uint32_t c){
    return address[r * width + c];
}
