# README

## Features
- [x] Long Mode (64-bit) OS
- [x] Higher Half Kernel
- [x] Stack Based - Page Frame Allocator (PMM) with O(1) time complexity
- [x] Virtual Memory Manager (VMM)
- [x] Global Descriptor Table (GDT)
- [x] Formatted Printing Support (`Printf(...)`)
- [x] Fonts are rendered to a Framebuffer
- [ ] Interrupt Descriptor Table
- [ ] Keyboard Input
- [ ] Heap

More coming soon...

## Build Instructions
To install build this kernel for the first time, take the following steps : 
- Clone the repo
- Run `git submodule update --init` (or clone recursively in first step itself)
- Uncomment the line `make -C limine` from `build.sh``
- run `build.sh`
- Comment out the `make -C limine` line again (keep this commented for future)

## Run MisraOS
To run MisraOS, you run the `misra.hdd` file present in the project root directory. This is built when you run the build script. Run it using
`qemu-system_x86-64 misra.hdd -m 256`. This will run it with 256 MB of memory.

## License

BSD 3-Clause License

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
