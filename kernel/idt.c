#include "idt.h"

static idt_pointer_t idt_pointer;

void set_idt_entry_offset(idt_entry_t *idt_entry, uint64_t offset){
    idt_entry->offset_low = (uint16_t)(offset & 0xffff);
    idt_entry->offset_middle = (uint16_t)(offset & 0xffff0000);
    idt_entry->offset_high = (uint32_t)(offset & 0xffffffff00000000);
}

uint64_t get_idt_entry_offset(idt_entry_t* idt_entry){
    uint64_t offset = 0;
    offset = ((uint64_t)idt_entry->offset_high << 32) | ((uint64_t)idt_entry->offset_middle << 16) | ((uint64_t)idt_entry->offset_low);

    return offset;
}

void initialize_interrupt_descriptor_table(){
    idt_pointer.limit = 0x0fff;
    idt_pointer.offset = (uint64_t)0;
}
