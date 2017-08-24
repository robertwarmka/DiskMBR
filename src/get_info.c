#include "get_info.h"
#include <stdlib.h>
#include <stdio.h>

unsigned short get_short(unsigned char upper, unsigned char lower) {
    // Set ret to upper, then shift upper up 8 bits, bit-wise OR with lower, and set that all as ret
    unsigned short ret = (upper << 8) | lower;
    return ret;
}

unsigned int get_int(unsigned char uu, unsigned char mu, unsigned char ml, unsigned char ll) {
    // bit-shift the appropriate bytes to the appropriate int locations, and set ret equal to the result
    unsigned int ret = (uu << 24) | (mu << 16) | (ml << 8) | ll;
    return ret;
}

int valid_mbr(char* buf, int buf_size) {
    if(buf == NULL) {
        fprintf(stderr, "ERROR: buf character array is null. Please pass in a valid buffer\n");
        return -1;
    }
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

    // INTERESTING NOTE: This can be written to accomodate for little endian or not, because the only two valid
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

    if(buf == NULL) {
        fprintf(stderr, "ERROR: buf character array is null. Please pass in a valid buffer\n");
        return -1;
    }

    if(partitions == NULL) {
        fprintf(stderr, "ERROR: partition_info struct pointer is null. Please pass in a valid struct pointer\n");
        return -1;
    }

    if(num_partitions < NUM_PARTITIONS) {
        fprintf(stderr, "ERROR: number of partitions passed in is too small. Need at least %d partitions, %d passed in\n", NUM_PARTITIONS, num_partitions);
        return -1;
    }

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
