/**
 *@file kernel.c
 *@author Siddharth Mishra (brightprogrammer)
 *@date 01/08/2022
 *@brief Contains kernel entry point code and init code.
 **/

#include "kernel.h"
#include "renderer.h"
#include "string.h"
#include "gdt.h"

// we cannot dynamically allocate anything now
// so will initialize stack as an array in .bss and tell stivale
// where our stack is
static uint8_t stack[8192]; // 8kb stack

// we need a framebuffer from stivale on bootup so we
// need to tell stivale that we need a framebuffer instead of
// CGA-compatible text mode.
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        // which type of tag is this
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        // this must be a pointer address, NULL in this case
        .next = NULLADDR
    },
    // set all framebuffer specifics to 0 and let bootloader decide
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    // The entry_point member is used to specify an alternative entry
    // point that the bootloader should jump to instead of the executable's
    // ELF entry point. We do not care about that so we leave it zeroed.
    .entry_point = NULLADDR,
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (uintptr_t)stack + sizeof(stack),
    // Bit 1, if set, causes the bootloader to return to us pointers in the
    // higher half, which we likely want since this is a higher half kernel.
    // Bit 2, if set, tells the bootloader to enable protected memory ranges,
    // that is, to respect the ELF PHDR mandated permissions for the executable's
    // segments.
    // Bit 3, if set, enables fully virtual kernel mappings, which we want as
    // they allow the bootloader to pick whichever *physical* memory address is
    // available to load the kernel, rather than relying on us telling it where
    // to load it.
    // Bit 4 disables a deprecated feature and should always be set.
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    // This header structure is the root of the linked list of header tags and
    // points to the first one in the linked list.
    .tags = (uintptr_t)&framebuffer_hdr_tag
};


// We will now write a helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL) {
            return NULL;
        }

        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id) {
            return current_tag;
        }

        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void *)current_tag->next;
    }
}

void hang(){
    for(;;){
        asm("hlt");
    }
}

// The following will be our kernel's entry point.
void _start(struct stivale2_struct *stivale2_struct) {
  // Let's get the terminal structure tag from the bootloader.
    struct stivale2_struct_tag_framebuffer *framebuffer_tag;
    framebuffer_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    // Check if the tag was actually found.
    if (framebuffer_tag == NULL) {
        hang();
    }

    // get framebuffer details
    framebuffer_t framebuffer;
    framebuffer.address = (uint32_t*)framebuffer_tag->framebuffer_addr;
    framebuffer.width = framebuffer_tag->framebuffer_width;
    framebuffer.height = framebuffer_tag->framebuffer_height;

    // prepare renderer
    font_renderer_t font_renderer;
    // create font renderer for this framebuffer
    font_renderer = create_font_renderer(framebuffer);
    // set default font renderer
    set_default_font_renderer(&font_renderer);

    // draw this string onto the screen
    draw_string("Misra OS | Copyright Siddharth Mishra (c) 2022 | All Rights Reserved\n\n");

    // get the memmap tag given to kernel by the bootloader
    struct stivale2_struct_tag_memmap *memmap_tag;
    memmap_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    // check if tag is valid
    if(memmap_tag == NULL){
        font_renderer.foreground_colour = 0xffff0000;
        draw_string("[-] Failed to get memory map.");

        hang();
    }else{
        draw_string("[+] Got the memory map.\n");
    }


    // load gdt
    draw_string("[+] Initializing Global Descriptor Table...\n");
    initialize_global_descriptor_table();
    draw_string("[+] Initializing Global Descriptor Table... Complete!\n");

    // We're done, just hang...
    for (;;) {
        asm ("hlt");
    }
}
