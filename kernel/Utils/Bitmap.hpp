/**
 *@file Bitmap.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/14/2022
 *@brief Provides an interface to store 8 booleans in 1 byte.
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

#ifndef UTILS_BITMAP_HPP
#define UTILS_BITMAP_HPP

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// bitmap helps you store 8 bools in 1 byte
struct Bitmap{
    // default constructor
    Bitmap() = default;

    // Create a new bitmap of given size and given buffer
    // Size is the size of buffer and not number of bits
    Bitmap(size_t size, uint8_t* buffer);

    // Get value of bit at given index.
    // idx is the index of bit
    bool operator [] (size_t idx);

    // Set value of bit to true at given index
    void SetBit(size_t idx);
    // Set value of bit to false at given index
    void UnsetBit(size_t idx);

    // size of buffer and not the number of booleans required
    size_t size;
    uint8_t *buffer;
};

#endif // UTILS_BITMAP_HPP
