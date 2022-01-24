/**
 *@file Bitmap.cpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/14/2022
 *@brief Definitions for Bitmap class
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

#include "Bitmap.hpp"

// create bitmap
Bitmap::Bitmap(size_t size, uint8_t* buffer)
: size(size), buffer(buffer){}

// set bit at given index
void Bitmap::SetBit(size_t idx){
    // check if given index is in range of bitmap size
    if(size < (idx / 8))
        return;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    // orring with 1 will always set it to 1
    buffer[buf_idx] = buffer[buf_idx] | (1 << bit_offset);
}

// unset bit at given index
void Bitmap::UnsetBit(size_t idx){
    // check if given index is in range of bitmap size
    if(size < (idx / 8))
        return;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    // and it with 0 to set it off
    buffer[buf_idx] = buffer[buf_idx] & (~(1 << bit_offset));
}

// if idx is out of range then false is returned
bool Bitmap::operator[](uint64_t idx){
    // check if index is in range
    if(size < (idx / 8))
        return false;

    // index of byte that contains the bit for given index
    uint64_t buf_idx = idx / 8;
    // offset into that bit
    uint8_t bit_offset = idx % 8;

    return (buffer[buf_idx] >> bit_offset) & 0x1;
}
