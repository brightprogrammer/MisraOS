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

## License
All code written by me (author : Siddharth Mishra) will be licensed under [`CC BY-SA 3.0`](https://creativecommons.org/licenses/by-sa/3.0/). If the built kernel uses my code the the kernel must be licensed under [`CC BY-SA 3.0`](https://creativecommons.org/licenses/by-sa/3.0/) too!
