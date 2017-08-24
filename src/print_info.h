#ifndef PRINT_INFO_H
#define PRINT_INFO_H

// Need to include diskmbr.h for parititon_info struct and MBR information location constants
#include "diskmbr.h"

void print_disk(disk_info* disk);

void print_partitions(partition_info* partitions, int num_partitions);

int print_info(char* device, disk_info* disk, partition_info* partitions, int num_partitions);

#endif // PRINT_INFO_H
