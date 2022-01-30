/**
 *@file ACPI.hpp
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/30/2022
 *@brief Declares ACPI structs
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

#ifndef RSDP_HPP
#define RSDP_HPP

#include "Common.hpp"
#include <cstdint>

// information about root system description pointer
struct RSDPDescriptorV1 {
    char signature[8];
    uint8_t checksum;
    char oemID[6];
    uint8_t revision;
    uint32_t rsdtAddress;
} PACKED_STRUCT;

// new rsdp structure defined in uefi v2
struct RSDPDescriptorV2 : public RSDPDescriptorV1{
    uint32_t length;
    uint64_t xsdtAddress;
    uint8_t extendedChecksum;
    uint8_t reserved[3];
} PACKED_STRUCT;

// NOTE: RSDP checksums can be often wrong
// we will just assume them to be true
// also limine checks that for us before hand
struct RSDPDescriptor{
    RSDPDescriptor();

    // rev == 1 for version 1
    // rev > 1 for revion 2
    uint8_t rev;
    union {
        RSDPDescriptorV1 v1;
        RSDPDescriptorV2 v2;
    };

    bool ValidateChecksum();
    uint64_t GetSDTAddress();
};

// all tables have same header
// System Descriptor Table Header
struct SDTHeader {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemID[6];
    char oemTableID[8];
    uint32_t oemRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} PACKED_STRUCT;

// Memory-mapped Configuration Table Header
struct MCFGHeader : public SDTHeader{
    uint64_t reserved;
} PACKED_STRUCT;

#endif // RSDP_HPP
