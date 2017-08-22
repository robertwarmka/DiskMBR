#include "read_mbr.h"
// Need diskmbr.h for MBR_SIZE
#include "diskmbr.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

// Opens the device passed in, reads the MBR into the passed in buffer
int read_mbr(const char* device, char* buf, int buf_size) {
    int fd;
    ssize_t read_amt, bytes_returned;

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

    return read_amt;
}
