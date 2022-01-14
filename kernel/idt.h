 /**
 *@file idt.h
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/11/22
 *@brief interrupt descriptor table
 **/

#ifndef IDT_H_
#define IDT_H_

#include <stdint.h>

#define IDT_TYPE_ATTR_InterruptGate 0b10001110
#define IDT_TYPE_ATTR_CallGate 0b10001100
#define IDT_TYPE_ATTR_TrapGate 0b10001111

// idt entry structure
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

// set offset for an idt entry
void set_idt_entry_offset(idt_entry_t* idt_entry, uint64_t offset);
// get offset from an idt entry
uint64_t get_idt_entry_offset(idt_entry_t* idt_entry);

// idtr register structure
typedef struct {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed)) idt_pointer_t;

// you know what this does!
void initialize_interrupt_descriptor_table();

#endif // IDT_H_
