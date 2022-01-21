#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "../Bootloader/stivale2.h"

// basic framebuffer information
// a framebuffer is where renderer draws things
struct Framebuffer{
    // Constructor needs the framebuffer tag returned from bootloader
    // in the kernel entry. Framebuffer tag is requested in Bootloader/Utils.cpp
    Framebuffer(stivale2_struct_tag_framebuffer* framebuffer_tag);

    // access a pixel in r-th row and c-th column
    uint32_t& operator () (uint32_t r, uint32_t c);

    // physical address of framebuffer
    uint32_t *address;

    // width and height of framebuffer
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
};

#endif // FRAMEBUFFER_HPP
