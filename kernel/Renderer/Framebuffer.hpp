/**
 *@file Framebuffer.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief Framebuffer helper class
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <cstdint>

// basic framebuffer information
// a framebuffer is where renderer draws things
struct Framebuffer{
    // default framebuffer constructor
    // uses the framebuffer information provided in bootinfo
    Framebuffer();

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
