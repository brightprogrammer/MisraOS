# cmake settings
cmake_minimum_required(VERSION 3.5)

# project settings
project(misraos VERSION 0 LANGUAGES C ASM)

# add kernel
add_subdirectory(kernel)
