#ifndef GET_PARTITIONS_H
#define GET_PARTITIONS_H

// Need to include diskmbr.h for parititon_info struct and MBR information location constants
#include "diskmbr.h"
// errno.h is included in this header so that anything that includes this header will have errno
// if something goes wrong in populate_partition_info
#include <errno.h>

// This function checks if the MBR has the right ending magic bytes (0x55AA)
//
// Returns: 0 on success, -1 on failure
int valid_mbr(char* buf, int buf_size);

// This function will create an unsigned short out of two chars (bytes)
unsigned short get_short(unsigned char upper, unsigned char lower);

// This function will create an unsigned int out of four chars (bytes)
unsigned int get_int(unsigned char uu, unsigned char mu, unsigned char ml, unsigned char ll);

// Pulls the cylinder info out of the 2 bytes that contain it in the CHS byte triple
unsigned int get_cylinder(unsigned char lower, unsigned char upper);

// Pulls the head info out of the 1 byte that contains it in the CHS byte triple
unsigned int get_head(unsigned char data);

// Pulls the sector info out of the 1 byte that contains it in the CHS byte triple
unsigned int get_sector(unsigned char data);

// This function will populate the disk info struct passed in.
//
// The parameters are as follows:
// 1. buf: The character buffer that holds the MBR data
//
// 2. disk: The disk_info struct pointer that holds the disk_info struct
//
// Return: This function will return 0 on success, and -1 on error, with errno set to the
// appropriate value
int populate_disk_info(char* buf, disk_info* disk);

// This function will populate the partition info struct passed in.
//
// The parameters are as follows:
// 1. buf: The character buffer that holds the MBR data
//
// 2. partitions: The partition_info struct pointer that is an array of one or more
//    partition_info structs. These structs are what will be populated in this function
//
// 3. size: The size of the paritions array (how many partition_info structs the array
//    contains)
//
// Return: This function will return 0 on success, and -1 on error, with errno set to the
// appropriate value
int populate_partition_info(char* buf, partition_info* partitions, int num_partitions);

#endif // GET_PARTITIONS_H
