#ifndef DISKMBR_H
#define DISKMBR_H

/*
 *
 * Author - Robert Warmka
 *
 * This program's purpose is to read the MBR (Master Boot Record) of the given device,
 * determine the type of MBR, and print out the relevant information.
 *
 */

// Include config.h - generated by autotools for version-specific
#include <config.h>

// Define Constants
// Size of MBR (amount to read from disk)
#define MBR_SIZE 512

// Starting location of magic bytes
#define MAGIC_BYTE_LOC 510

// MBR magic bytes (0x55AA). Ensure these exist (last 2 bytes - bytes 510 & 511 of MBR)
#define MAGIC_BYTES 0x55AA

// Number of partitions to read from the MBR
#define NUM_PARTITIONS 4

// For all MBR types except for AST/NEC MS-DOS & SpeedStor, the first partition entry is at 446
// and even in AST/NEC/SpeedStor, the primary partition entries are still bytes 446-509, but
// they are laid out 4, 3, 2, 1, instead of the traditional 1, 2, 3, 4. Maybe handle their
// special parition case in the future (to not display them backwards), but right now, just
// display that backwards if that MBR is ever encountered
#define PARTITION_LOC 446

// Each partition is guaranteed to be 16 bytes in size
#define PARTITION_SIZE 16

// Define partition info offsets
#define DRIVE_STATUS_OFFSET 0
#define CHS_START_OFFSET 1
#define PARTITION_TYPE_OFFSET 4
#define CHS_END_OFFSET 5
#define LBA_START_OFFSET 8
#define LBA_COUNT_OFFSET 12

// If we have a modern boot MBR, byte 440 will begin the disk signature
#define DISK_SIGNATURE_LOCATION 440

// To check if bytes 440-443 are a real signature (and not boot code), check if bytes 444-445
// are either 0x0000 or 0x5A5A, meaning not copy protected or copy protected
#define DISK_SIGNATURE_VERIFICATION 444
#define NOT_COPY_PROTECTED 0x0000
#define COPY_PROTECTED 0x5A5A

/// Structs
typedef struct {
    // Drive status
    unsigned char status;
    // Partition type
    unsigned char partition_type;
    // Starting CHS (cylinder, head, sector)
    unsigned int start_cylinder;
    unsigned int start_head;
    unsigned int start_sector;
    // Ending CHS
    unsigned int end_cylinder;
    unsigned int end_head;
    unsigned int end_sector;
    // Starting LBA
    unsigned long long start_lba;
    // Number of sectors
    unsigned long long sector_count;
    // If this partition_info struct holds valid data, or if it's unused (all 0)
    int valid;
} partition_info;

#endif // DISKMBR_H
