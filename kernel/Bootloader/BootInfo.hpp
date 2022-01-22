#ifndef BOOTINFO_HPP
#define BOOTINFO_HPP

#include "stivale2.h"
#include <cstdint>

// contains boot information needed by the kernel
struct BootInfo {
    BootInfo() = default;
    BootInfo(stivale2_struct* tagList);

    static uint64_t GetFramebufferAddress();
    static uint16_t GetFramebufferPitch();
    static uint16_t GetFramebufferWidth();
    static uint16_t GetFramebufferHeight();

    static uint64_t GetKernelPhysicalBase();
    static uint64_t GetKernelVirtualBase();
private:
    // framebuffer information
    static inline uint64_t fbAddr = 0;
    static inline uint16_t fbPitch = 0;
    static inline uint16_t fbWidth = 0;
    static inline uint16_t fbHeight = 0;

    // kernel information
    static inline uint64_t krnlPhysAddr = 0;
    static inline uint64_t krnlVirtAddr = 0;
};



#endif // BOOTINFO_HPP
