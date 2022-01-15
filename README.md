# README

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
