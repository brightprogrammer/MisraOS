/**
 *@file Framebuffer.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief Framebuffer handling class
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

    // clear framebuffer with given color
    // last clear color will be stored and
    static void Clear(uint32_t clear);

    // cached clear color
    static inline uint32_t clearColor = 0x00000000;

    // physical address of framebuffer
    static inline uint32_t *address = nullptr;

    // width and height of framebuffer
    static inline uint32_t width = 0;
    static inline uint32_t height = 0;
    static inline uint32_t pitch = 0;
};

#endif // FRAMEBUFFER_HPP
