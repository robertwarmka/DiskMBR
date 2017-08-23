// Includes
#include "diskmbr.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "get_partitions.h"
#include "read_mbr.h"

int main(int argc, char** argv) {
    // Variable declaration
    int i, status;
    char buf[MBR_SIZE] = {0};
    disk_info disk = {0};
    partition_info* partitions = malloc(NUM_PARTITIONS * sizeof(partition_info));
    // Set partitions to 0
    memset(partitions, 0, NUM_PARTITIONS * sizeof(partitions));

    // Input parsing
    if(argc < 2) {
        fprintf(stderr, "ERROR - Not enough arguments. Proper use: %s <device>\n", argv[0]);
        return 1;
    }

    // argv[1] contains the device the user wants to open. MBR_SIZE is the size of the buffer
    if(read_mbr(argv[1], buf, MBR_SIZE) == -1) {
        // We errored upon reading the MBR, return. read_mbr should print the error message
        return 1;
    }

    while(i < MBR_SIZE) {
        if(i > 0) {
            printf(":");
        }
        printf("%02hhX", buf[i]);
        ++i;
    }
    printf("\n");

    printf("\n");
    i = 440;
    while(i < 446) {
        printf("%02hhX ", buf[i]);
        ++i;
    }
    printf("\n");

    if(valid_mbr(buf, MBR_SIZE) == -1) {
        fprintf(stderr, "ERROR: MBR not valid. Magic bytes should be: %04hX, but they are: %04hX\n", MAGIC_BYTES, get_short(buf[510], buf[511]));
        return 1;
    }

    // Pass in a pointer to the disk_info struct to keep disk_info on the stack
    status = populate_disk_info(buf, &disk);
    if(status != 0) {
        fprintf(stderr, "ERROR: populate_disk_info returned an error");
        return 1;
    }

    if(disk.valid) {
        printf("%04X\n", disk.disk_signature);
        printf("%02hX\n", disk.copy_protected);
    }

    status = populate_partition_info(buf, partitions, NUM_PARTITIONS);
    if(status != 0) {
        fprintf(stderr, "ERROR: populate_partition_info returned an error");
        return 1;
    }

    // Free head-allocated memory
    free(partitions);

    return 0;
}
