#include "get_partitions.h"
#include <stdlib.h>
#include <stdio.h>

/*
#define MBR_SIZE 512
#define MAGIC_BYTES 0x55AA
#define PARTITION_LOC 446
#define PARTITION_SIZE 16

#define DISK_SIG_LOC 440
#define DISK_SIG_SIZE 4

#define COPY_PROT_LOC 444
#define COPY_PROT_SIZE 2

#define NOT_COPY_PROTECTED 0x0000
#define COPY_PROTECTED 0x5A5A

#define DRIVE_STATUS_OFFSET 0
#define CHS_START_OFFSET 1
#define PARTITION_TYPE_OFFSET 4
#define CHS_END_OFFSET 5
#define LBA_START_OFFSET 8
#define LBA_COUNT_OFFSET 12

typedef struct {
    // Drive status
    unsigned char status;
    // Partition type
    unsigned char partition_type,
    // Starting CHS (cylinder, head, sector)
    unsigned int start_cylinder,
    unsigned int start_head,
    unsigned int start_sector,
    // Ending CHS
    unsigned int end_cylinder,
    unsigned int end_head,
    unsigned int end_sector,
    // Starting LBA
    unsigned long long start_lba,
    // Number of sectors
    unsigned long long sector_count
    // If this partition_info struct holds valid data, or if it's unused (all 0)
    int valid;
} partition_info;
*/

unsigned short get_short(unsigned char upper, unsigned char lower) {
    // Set ret to upper, then shift upper up 8 bits, bit-wise OR with lower, and set that all as ret
    unsigned short ret = (upper << 8) | lower;
    return ret;
}

unsigned int get_int(unsigned char uu, unsigned char mu, unsigned char ml, unsigned char ll) {
    // 
    unsigned int ret = (uu << 24) | (mu << 16) | (ml << 8) | ll;
    return ret;
}

int valid_mbr(char* buf, int buf_size) {
    if(buf_size < MBR_SIZE) {
        fprintf(stderr, "ERROR: Buffer size not large enough to check for a valid MBR\n");
        return -1;
    }
    unsigned short magic_bytes = get_short(buf[MAGIC_BYTE_LOC], buf[MAGIC_BYTE_LOC + 1]);
    if(magic_bytes != MAGIC_BYTES) {
        return -1;
    }
    return 0;
}
/*
typedef struct {
    // 4 byte disk signature, if one exists
    unsigned int disk_signature;
    // 2 bytes to specify if the disk is copy protected or not. If it is copy protected,
    // the bytes take the value 0x5A5A. If not, the value is 0x0000. All other values are invalid
    unsigned short copy_protected;
    // If any bytes for the signature or copy protection are non-zero, this struct will be valid,
    // Otherwise if everything is 0, or the copy protection != (0x0000 || 0x5A5A), then the struct
    // is invalid
    int valid;
} disk_info;
#define DISK_SIG_LOC 440
#define DISK_SIG_SIZE 4
#define COPY_PROT_LOC 444
#define COPY_PROT_SIZE 2
*/
int populate_disk_info(char* buf, disk_info* disk) {
    int valid_loop;
    int disk_signature_end = DISK_SIG_LOC + DISK_SIG_SIZE + COPY_PROT_SIZE;

    if(buf == NULL) {
        fprintf(stderr, "ERROR: buf character array is null. Please pass in a valid buffer\n");
        return -1;
    }

    if(disk == NULL) {
        fprintf(stderr, "ERROR: disk_info struct pointer is null. Please pass in a valid struct pointer\n");
        return -1;
    }

    // Populate the members of disk_info with the values from the appropriate locations in the buffer
    // These will also be used later to determine the validity of the data
    disk->disk_signature = get_int(buf[DISK_SIG_LOC + 3], buf[DISK_SIG_LOC + 2],
                                   buf[DISK_SIG_LOC + 1], buf[DISK_SIG_LOC]);

    // NOTE: This can be written to accomodate for little endian or not, because the only two valid
    // values for copy_protected are 0x0000 and 0x5A5A, it doesn't matter what order the bytes are
    // loaded in as, because both bytes will hold the same value, so endianness doesn't matter here
    disk->copy_protected = get_short(buf[COPY_PROT_LOC + 1], buf[COPY_PROT_LOC]);

    // Check to make sure we have valid disk data
    //
    // First, the disk signature must be non-zero. I don't think a disk signature can be non-zero
    // Second, make sure the copy protection is either 0x0000 or 0x5A5A. All other values are invalid
    //
    // So if both of these are true, we are valid, otherwise we are not.
    if(disk->disk_signature != 0 && (disk->copy_protected == NOT_COPY_PROTECTED || disk->copy_protected == COPY_PROTECTED)) {
        disk->valid = 1;
    } else {
        disk->valid = 0;
    }
    // The one case that can slip through is if a legacy
    // MBR uses at least up to byte 440, up to byte 443 for boot code, but leaves bytes 444-445
    // as 0. This then leaves the signature location non-zero, but it leaves the copy protection
    // location 0 (a valid value), which will trick this function into thinking the drive has a
    // valid signature. An interesting side effect if it ever comes up.
    return 0;
}

unsigned int get_cylinder(unsigned char lower, unsigned char upper) {
    // Cylinder contained in the second and third bytes of the CHS triple, with the third byte containing bits 0-7,
    // and the second byte containing bits 8 & 9. Thus, bit-wise AND the second byte with 0xC0 (11000000b), set
    // that equal to the return int, then shift the int right 8 bits and bit-wise OR it with the lower 8 bits
    unsigned int ret = upper & 0xC0;
    // Only bit-shift 2 bits right, because the upper bits are already at bits 6 & 7 before the shift
    ret = (ret << 2) | lower;
    return ret;
}

unsigned int get_head(unsigned char data) {
    // Head contained solely to the first byte of the CHS byte triple, so just cast to unsigned int
    return (unsigned int)data;
}

unsigned int get_sector(unsigned char data) {
    // Sector contained solely in the lower 6 bits of the second byte of the CHS byte triple,
    // so just bit-wise AND it with 0x3F (00111111b) to get the sector count
    return (unsigned int)(data & 0x3F);
}

int populate_partition_info(char* buf, partition_info* partitions, int num_partitions) {
    // Set the offset to PARTITION_LOC, increment by PARTITION_SIZE
    int offset = PARTITION_LOC;
    int count, valid_loop;
    //unsigned int cylinder, head, sector;

    for(count = 0; count < num_partitions; ++count) {
        for(valid_loop = offset; valid_loop < offset + PARTITION_SIZE; ++valid_loop) {
            if(buf[valid_loop] != 0) {
                // If we find any byte in this partition that isn't 0, then the partition is marked as valid
                // If all bytes are 0, then this partition is unused and contains nothing important, so skip it
                partitions[count].valid = 1;
                break;
            }
        }
        // Populate the partition fields if the partition is valid, otherwise there's no data there (all zeros)
        if(partitions[count].valid) {
            partitions[count].status = buf[offset + DRIVE_STATUS_OFFSET];
    
            partitions[count].start_cylinder = get_cylinder(buf[offset + CHS_START_OFFSET + 2], buf[offset + CHS_START_OFFSET + 1]);
            partitions[count].start_head = get_head(buf[offset + CHS_START_OFFSET]);
            partitions[count].start_sector = get_sector(buf[offset + CHS_START_OFFSET + 1]);
    
            partitions[count].partition_type = buf[offset + PARTITION_TYPE_OFFSET];
    
            partitions[count].end_cylinder = get_cylinder(buf[offset + CHS_END_OFFSET + 2], buf[offset + CHS_END_OFFSET + 1]);
            partitions[count].end_head = get_head(buf[offset + CHS_END_OFFSET]);
            partitions[count].end_sector = get_sector(buf[offset + CHS_END_OFFSET + 1]);
    
            partitions[count].start_lba = get_int(buf[offset + LBA_START_OFFSET + 3], buf[offset + LBA_START_OFFSET + 2],
                                                    buf[offset + LBA_START_OFFSET + 1], buf[offset + LBA_START_OFFSET]);
    
            partitions[count].sector_count = get_int(buf[offset + LBA_COUNT_OFFSET + 3], buf[offset + LBA_COUNT_OFFSET + 2],
                                                    buf[offset + LBA_COUNT_OFFSET + 1], buf[offset + LBA_COUNT_OFFSET]);
        }

        // Increment partition offset
        offset += PARTITION_SIZE;
    }

    return 0;
}
