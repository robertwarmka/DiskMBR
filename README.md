# DiskMBR
[![Build Status](https://travis-ci.org/robertwarmka/DiskMBR.svg?branch=master)](https://travis-ci.org/robertwarmka/DiskMBR)

## Overview
This program can read the contents of your drive's MBR, much like the popular Linux utility, fdisk.

It will print the following list of items (so long as they exist):
1. Disk Signature
2. Copy Protection
3. Disk Size
4. Partition Info (Traditional MBR supports up to 4 partitions)
    1. Partition Status
    2. Partition Type
    3. Starting CHS (Cylinder, Head, Sector) Triple
    4. Ending CHS (Cylinder, Head, Sector) Triple
    5. Starting LBA
    6. Ending LBA
    7. LBA Count

## Build
To build, execute the following commands:
```
autoreconf --install
./configure
make
```

## Test
You can run tests by running the following make command:
```
make && make check
```
If you get errors during testing, make sure you have already executed `make` before `make check`.
The reason for this is that the tests depend on files in the src/ directory, which need to be
built prior to building the tests

## Install
```
make install
```
You will need to add sudo to the previous command if your install directory needs superuser permissions

## Run
To run diskmbr, execute the following command:
```
sudo diskmbr </path/to/your-device>
```
Or if you didn't install it, build with `make`, change directories to src/, and execute:
```
sudo ./diskmbr </path/to/your-device>
```
Don't forget to sudo for all devices that need superuser permissions!

## Clean Up
For a normal object & executable removal, execute `make clean` 
For a more thorough clean that removes Makefiles, execute `make distclean` 
For the most thorough clean, removing cache files as well, execute `make maintainer-clean`
