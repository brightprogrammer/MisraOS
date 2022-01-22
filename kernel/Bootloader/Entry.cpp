#include "BootInfo.hpp"
#include "Util.hpp"

// declare kenrel entry here and define it in KernelEntry.cpp
void KernelEntry();

// this is the actual kernel entry point
// but we keep this as a bootloader part to seprarate bootloader from kernel
// in hopes that this will support modular design apporach
// this is just an approach to not make this bootloader a viral component of this kernel
// we can remove this anytime approach
extern "C" [[noreturn]] void Entry(stivale2_struct* tagList){
    // initialize this variable just to init boot info
    // this will load all things required by kernel to this struct
    // and then kernel can access parts from this
    BootInfo bootInfo(tagList);

    // handover control to kernel
    KernelEntry();

    // hang if the kernel returns
    InfiniteHalt();
}
