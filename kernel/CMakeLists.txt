# set assembly compile options
# set(CMAKE_ASM_FLAGS "${CFLAGS}")


# set sources
set(KERNEL_SRCS "KernelEntry.cpp" "Renderer/Framebuffer.cpp" "Renderer/FontRenderer.cpp" "Renderer/Font.cpp" "String.cpp"
    "GDT.cpp" "Utils/Bitmap.cpp" "Bootloader/Util.cpp"
    "PhysicalMemoryManager.cpp" "VirtualMemoryManager.cpp" "Printf.cpp" "Bootloader/Entry.cpp" "Bootloader/BootInfo.cpp")

# make kernel as executable
add_executable(kernel ${KERNEL_SRCS})

# set compile options
target_compile_options(kernel PRIVATE   -Wall -Wextra -O0 -g
                                        -ffreestanding
                                        -fno-stack-protector
                                        -fno-pic -fpie
                                        #-mgeneral-regs-only
                                        -fno-exceptions
                                        -mno-red-zone)
# set linker options
target_link_options(kernel PRIVATE  -fno-pic -fpie
                                    # this must be a comma separated list
                                    -Wl,-static,-pie,-ztext--no-dynamic-linker
                                    -static-pie
                                    -nostdlib
                                    -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
                                    -z max-page-size=0x1000)

# set_target_properties(kernel PROPERTIES PUBLIC_HEADER "stivale2.h")

# add install rules for this include dir
# install(TARGETS kernel PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/kernel)
