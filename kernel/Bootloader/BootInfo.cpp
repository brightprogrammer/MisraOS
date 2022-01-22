#include "BootInfo.hpp"
#include "Util.hpp"

static BootInfo bootInfo = {};

BootInfo::BootInfo(stivale2_struct* stivaleTagList){
    // get framebuffer tag
    stivale2_struct_tag_framebuffer *fb_tag = reinterpret_cast<stivale2_struct_tag_framebuffer*>(StivaleGetTag(stivaleTagList, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID));
    // stivale 2 spec states that bootloader will boot even if the framebuffer wasn't found!
    if (fb_tag == nullptr) {
        InfiniteHalt();
    }


    // fill framebuffer related information
    fbAddr = fb_tag->framebuffer_addr;
    fbWidth = fb_tag->framebuffer_width;
    fbHeight = fb_tag->framebuffer_height;
    fbPitch = fb_tag->framebuffer_pitch;

    // get kernel related information
    stivale2_struct_tag_kernel_base_address *kaddr_tag = reinterpret_cast<stivale2_struct_tag_kernel_base_address*>(StivaleGetTag(stivaleTagList, STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID));
    krnlPhysAddr = kaddr_tag->physical_base_address;
    krnlVirtAddr = kaddr_tag->virtual_base_address;
}

uint64_t BootInfo::GetFramebufferAddress(){ return fbAddr; }
uint16_t BootInfo::GetFramebufferWidth(){ return fbWidth; }
uint16_t BootInfo::GetFramebufferHeight(){ return fbHeight; }
uint16_t BootInfo::GetFramebufferPitch(){ return fbPitch; }

uint64_t BootInfo::GetKernelPhysicalBase(){ return krnlPhysAddr; }
uint64_t BootInfo::GetKernelVirtualBase(){ return krnlVirtAddr; }
