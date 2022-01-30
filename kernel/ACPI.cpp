/**
 *@file ACPI.cpp
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

#include "ACPI.hpp"
#include "Utils/String.hpp"
#include "Printf.hpp"
#include "Bootloader/BootInfo.hpp"
#include "VirtualMemoryManager.hpp"

// validate checksum based on revision number
bool RSDPDescriptor::ValidateChecksum(){
    if(rev == 1){
        uint8_t* mem = reinterpret_cast<uint8_t*>(&v1);
        uint64_t validate = 0;
        for(uint8_t i = 0; i < sizeof(RSDPDescriptorV1); i++){
            validate += mem[i];
        }

        return (validate & 0xff) == 0;
    }else{
        uint8_t* mem = reinterpret_cast<uint8_t*>(&v1);
        uint64_t validate1 = 0;
        for(uint8_t i = 0; i < sizeof(RSDPDescriptorV1); i++){
            validate1 += mem[i];
        }

        mem = reinterpret_cast<uint8_t*>(&v2);
        uint64_t validate2 = 0;
        for(uint8_t i = 0; i < sizeof(RSDPDescriptorV2); i++){
            validate2 += mem[i];
        }

        return ((validate2 & 0xff) == 0) && ((validate1 & 0xff) == 0);
    }
}


// // validate signature, checksum etc...
// bool RSDPDescriptor::ValidateHeader(){
//     if(rev == 1){
//         return (strcmp(v1.signature, "RSDP PTR ") == 0) && ValidateChecksum();
//     }else{
//         return (strcmp(v2.signature, "RSDP PTR ") == 0) && ValidateChecksum();
//     }
// }

RSDPDescriptor::RSDPDescriptor(){
    if(BootInfo::GetRSDPAddress() != 0){
        RSDPDescriptorV1 *rsdp1 = reinterpret_cast<RSDPDescriptorV1*>(BootInfo::GetRSDPAddress());
        Printf("[+] ACPI Revision : %i\n", rsdp1->revision);
        if(ValidateChecksum()) Printf("[+] RSDP checksum is valid\n");
        else Printf("[!] RSDP checksum is invalid\n");

        rev = rsdp1->revision;
        if(rsdp1->revision == 0){
            v1 = *rsdp1;
        }else{
            v2 = *reinterpret_cast<RSDPDescriptorV2*>(rsdp1);
        }
    }else{
        Printf("[!] This system doesn't support ACPI\n");
    }
}

uint64_t RSDPDescriptor::GetSDTAddress(){
    if(rev == 0){
        return static_cast<uint64_t>(v1.rsdtAddress) + MEM_PHYS_OFFSET;
    }else{
        return v2.xsdtAddress + MEM_PHYS_OFFSET;
    }
}
