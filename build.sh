#!/usr/bin/env bash

# create sys_root directory structure
# root of our operating system
mkdir -pv sys_root
# contains bootloader related files
mkdir -pv sys_root/boot

# remove previous kernel
rm sys_root/boot/kernel

# build os
mkdir -pv build
cd build
# take any changes in account
cmake ..
# build
make -j8
# install will keep the build files in build directory only
# we will need to copy those files from here to sys_root
make install
cd ..

# remove previous misra.hdd
rm misra.hdd

# Create an empty zeroed out 64MB misra.hdd file.
# block size is 1 MB (read/write bytes at once)
dd if=/dev/zero bs=1M count=0 seek=64 of=misra.hdd

# Create a GUID partition table.
# -s is --script : this never prompts for user intervention
parted -s misra.hdd mklabel gpt

# Create an EFI System Partition (ESP) that spans the whole disk.
parted -s misra.hdd mkpart ESP fat32 2048s 100%
# set ESP flag "on" for partition 1 on misra.hdd
parted -s misra.hdd set 1 esp on

# Build limine-install.
# make -C limine

# Install the Limine BIOS stages onto misra
limine/build/bin/limine-install misra.hdd

# Mount the loopback device.
# The loop device is a block device that maps its data blocks
# not to a physical device such as a hard disk or optical disk
# drive, but to the blocks of a regular file in a filesystem
# or to another block device
#
# partscan forces the kernel to scan the partition table for newly
# created loop device.
# find the first unused loop device and if a file argument is present
# (in our case misra.hdd) use it as loop device
# show the name of the assigned loop device (--find [file] must be present)
USED_LOOPBACK=$(sudo losetup --partscan --find misra.hdd --show)

# Format the ESP partition (partition 1 : p1) as FAT32.
sudo mkfs.fat -F 32 ${USED_LOOPBACK}p1

# Create directory for mounting misra.hdd
mkdir -p disk_root
# mount hdd so that we can directly copy/paste files into it
sudo mount ${USED_LOOPBACK}p1 disk_root

# copy kernel
cp -v build/kernel/kernel sys_root/boot/kernel
# copy bootloader related file
mkdir -pv sys_root/boot/EFI
mkdir -pv sys_root/boot/EFI/BOOT
cp -v limine/build/bin/BOOTX64.EFI sys_root/boot/EFI/BOOT/
# copy limine config
cp -v limine.cfg limine/build/bin/limine.sys sys_root/boot/

# Copy the relevant files over
sudo cp -v sys_root/* disk_root/ -r

# Sync system cache and unmount partition and loopback device.
sync
sudo umount disk_root
sudo losetup -d ${USED_LOOPBACK}
