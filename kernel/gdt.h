#/**
 *@file gdt.h
 *@author Siddharth Mishra
 *@date 01/11/22
 *@brief global descriptor table function definitions
 *@copyright Copyright (c) 2022 Siddharth Mishra CC BY-SA 3.0
 **/

#ifndef GDT_H_
#define GDT_H_

#include <stdint.h>

// struct to represent gdtr
typedef struct{
    uint16_t table_limit;
    uint64_t table_base_address;
} __attribute__((packed)) GDTR;

// implementation of a general segment descriptor
typedef struct {
    // segment limit is divided into 2 parts
    // fist part is bit 0 to 15 in first dword
    // second pat is bit 16 to 19 in second dword
    uint16_t segment_limit_low;

    // base address is divided into 3 parts
    // first part is bit 16 to 31 in first dword
    // second part is bit 0 to 7 in second dword
    // third and final part is from bit 24 to 32 in second dword
    uint16_t base_address_low;
    uint8_t base_address_middle;

    // bit 0 to 3 is for type field
    // bit 4 is S bit (user segment or system segment)
    // bits 5 and 6 make up the DPL (descriptor privilege level) field
    // bit 7 is P bit (present bit)
    uint8_t access_flags;

    // this field contains the higher 4 bits of segment limit (bits 0:3)
    // then there is the AVL Bit (bit 4)
    // then a reserved bit (bit 5)
    // then a D/B bit (bit 6)
    // and at last the Granularity (G) bit (bit 7)
    uint8_t attributes;

    // finally the higher part of base_address
    uint8_t base_address_high;
} __attribute__((packed)) GDTEntry;

typedef struct {
    GDTEntry null;
    GDTEntry kernel_code;
    GDTEntry kernel_data;
    GDTEntry user_code;
    GDTEntry user_data;
} __attribute__((packed)) __attribute__((aligned(0x1000))) GDT;

// initialize global descriptor table
void initGDT();

#endif // GDT_H_
