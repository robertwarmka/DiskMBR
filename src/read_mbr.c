#include "read_mbr.h"
// Need diskmbr.h for MBR_SIZE
#include "diskmbr.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

// Opens the device passed in, gets the device size, reads the MBR into the passed in buffer, and closes the device
off_t read_mbr(const char* device, char* buf, int buf_size) {
    int fd;
    ssize_t read_amt, bytes_returned;
    off_t device_size;

    if(device == NULL) {
        fprintf(stderr, "ERROR: device is null. Please pass in a valid string for the device\n");
        return -1;
    }
    if(buf == NULL) {
        fprintf(stderr, "ERROR: buf character array is null. Please pass in a valid buffer\n");
        return -1;
    }
    if(buf_size < MBR_SIZE) {
        fprintf(stderr, "ERROR: Buffer size is too small\n");
        return -1;
    }

    if((fd = open(device, O_RDONLY)) == -1) {
        perror("ERROR: open failed with status");
        if(errno == EACCES) {
            fprintf(stderr, "Do you have the appropriate permissions/are you superuser?\n");
        }
        return -1;
    }

    // Get device size
    if((device_size = lseek(fd, 0, SEEK_END)) == -1) {
        perror("ERROR: lseek failed with status");
        return -1;
    }

    if(device_size < MBR_SIZE) {
        fprintf(stderr, "ERROR: device size is too small to be an MBR\n");
        return -1;
    }

    // Reset device to byte 0
    if(lseek(fd, 0, SEEK_SET) == -1) {
        perror("ERROR: lseek failed with status");
        return -1;
    }

    read_amt = 0;
    while(read_amt < MBR_SIZE) {
        bytes_returned = read(fd, buf + read_amt, MBR_SIZE - read_amt);
        if(bytes_returned == -1) {
            perror("ERROR: read failed with status");
            return -1;
        }
        read_amt += bytes_returned;
    }
   
    if(close(fd) == -1) {
        perror("ERROR: close failed with status");
    }

    return device_size;
}
