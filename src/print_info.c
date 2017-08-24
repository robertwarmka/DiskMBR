#include "print_info.h"
#include <stdio.h>

// A list of all partition types - as found on Wikipedia.
const char* const partition_types[] = {
    "Empty", // 0x00
    "FAT12", // 0x01
    "XENIX root", // 0x02
    "XENIX usr", // 0x03
    "FAT16", // 0x04
    "Extended", //0x05
    "FAT16B", // 0x06
    "NTFS/HPFS/exFAT", // 0x07
    "Multi-drive Partition", // 0x08
    "AIX", // 0x09
    "OS/2 Boot Manager", // 0x0A
    "FAT32 - CHS Addressing", // 0x0B
    "FAT32 - LBA Addressing", // 0x0C
    "Unused", // 0x0D
    "FAT16B - LBA Addressing", // 0x0E
    "Extended Partition - LBA Addressing", // 0x0F
    "OPUS", // 0x10
    "Hidden FAT12", // 0x11
    "Service FS", // 0x12
    "Unused", // 0x13
    "Hidden FAT16", // 0x14
    "Hidden Extended Partition", // 0x15
    "Hidden FAT16B", // 0x16
    "Hidden NTFS", // 0x17
    "AST Hibernation", // 0x18
    "Willowtech Photon coS", // 0x19
    "Unused", // 0x1A
    "Hidden FAT32", // 0x1B
    "Hidden FAT32 - LBA Addressing", // 0x1C
    "Unused", // 0x1D
    "Hidden FAT16 - LBA Addressing", // 0x1E
    "Hidden Extended - LBA Addressing", // 0x1F
    "Windows Mobile Update", // 0x20
    "HP Volume Expansion", // 0x21
    "Oxygen Extended Partition Table", // 0x22
    "Windows Mobile Boot", // 0x23
    "Logical FAT12/16", // 0x24
    "Windows Mobile IMGFS", // 0x25
    "Reserved", // 0x26
    "Windows Recovery Partition", // 0x27
    "Unused", // 0x28
    "Unused", // 0x29
    "AtheOS FS", // 0x2A
    "SyllableOS", // 0x2B
    "Unused", // 0x2C
    "Unused", // 0x2D
    "Unused", // 0x2E
    "Unused", // 0x2F
    "Reserved", // 0x31
    "NOS FS", // 0x32
    "Reserved", // 0x33
    "Reserved", // 0x34
    "IBM JFS", // 0x35
    "Reserved", // 0x36
    "Unused", // 0x37
    "THEOS 3.2", // 0x38
    "Bell Labs Plan 9", // 0x39
    "THEOS 4", // 0x3A
    "THEOS 4 Extended", // 0x3B
    "PartitonMagic PqRP", // 0x3C
    "PartitionMagic Hidden NetWare", // 0x3D
    "Unused", // 0x3E
    "Unused", // 0x3F
    "VentureCom Venix", // 0x40
    "Old Linux", // 0x41
    "Old Linux Swap", // 0x42
    "Old Linux Native", // 0x43
    "GoBack", // 0x44
    "Priam", // 0x45
    "EUMEL/ELAN", // 0x46
    "EUMEL/ELAN", // 0x47
    "EUMEL/ELAN", // 0x48
    "Unused", // 0x49
    "AdaOS", // 0x4A
    "Unused", // 0x4B
    "Aos FS", // 0x4C
    "Primary QNX POSIX", // 0x4D
    "Secondary QNX POSIX", // 0x4E
    "Tertiary QNX POSIX", // 0x4F
    "LynxOS", // 0x50
    "RW Partition - Disk Manager", // 0x51
    "CP/M-80", // 0x52
    "Auxiliary 3 - Disk Manager", // 0x53
    "Dynamic Drive Overlay - Disk Manager", // 0x54
    "EZ-Drive", // 0x55
    "AT&T MS-DOS", // 0x56
    "VNDI", // 0x57
    "Unused", // 0x58
    "Unused", // 0x59
    "Unused", // 0x5A
    "Unused", // 0x5B
    "Priam EDisk", // 0x5C
    "Unused", // 0x5D
    "Unused", // 0x5E
    "Unused", // 0x5F
    "Unused", // 0x60
    "SpeedStor", // 0x61
    "Unused", // 0x62
    "AT&T FS", // 0x63
    "NetWare FS 286", // 0x64
    "NetWare FS 386", // 0x65
    "NetWare FS 386", // 0x66
    "Wolf Mountain", // 0x67
    "NetWare", // 0x68
    "NetWare 5", // 0x69
    "Unused", // 0x6A
    "Unused", // 0x6B
    "Unused", // 0x6C
    "Unused", // 0x6D
    "Unused", // 0x6E
    "Unused", // 0x6F
    "DiskSecure Multiboot", // 0x70
    "Reserved", // 0x71
    "V7/x86", // 0x72
    "Reserved", // 0x73
    "Reserved", // 0x74
    "IBM PC/IX", // 0x75
    "Reserved", // 0x76
    "Novell", // 0x77
    "XOSL Bootloader", // 0x78
    "APTI FAT16 - CHS Addressing", // 0x79
    "APTI FAT16 - LBA Addressing", // 0x7A
    "APTI FAT16B - CHS Addressing", // 0x7B
    "APTI FAT32 - LBA Addressing", // 0x7C
    "APTI FAT32 - CHS Addressing", // 0x7D
    "PrimoCache - Level 2", // 0x7E
    "Reserved", // 0x7F
    "MINIX", // 0x80
    "MINIX", // 0x81
    "Solaris x86", // 0x82
    "Linux", // 0x83
    "Hibernation", // 0x84
    "Linux Extended", // 0x85
    "Linux RAID", // 0x86
    "Fault-tolerant NTFS", // 0x87
    "Linux plaintext Partition Table", // 0x88
    "Unused", // 0x89
    "Linux Kernel Image", // 0x8A
    "Fault-tolerant FAT32", // 0x8B
    "Fault-tolerant FAT32", // 0x8C
    "FreeDOS - Hidden FAT32", // 0x8D
    "Linux LVM", // 0x8E
    "Unused", // 0x8F
    "FreeDOS - Hidden FAT32", // 0x90
    "FreeDOS - Hidden Extended", // 0x91
    "FreeDOS - Hidden FAT16B", // 0x92
    "Hidden Linux FS", // 0x93
    "Amoeba Bad Block Table", // 0x94
    "EXOPC", // 0x95
    "ISO-9660 FS", // 0x96
    "FreeDOS - Hidden FAT32", // 0x97
    "FreeDOS - Hidden FAT32", // 0x98
    "Early Unix", // 0x99
    "FreeDOS - Hidden FAT16", // 0x9A
    "FreeDOS - Hidden Extended - LBA Addressing", // 0x9B
    "Unused", // 0x9C
    "Unused", // 0x9D
    "ForthOS", // 0x9E
    "BSD/OS 3.0+", // 0x9F
    "HP Diagnostic Partition", // 0xA0
    "HP Volume Expansion", // 0xA1
    "HPS ARM Preloader", // 0xA2
    "HP Volume Expansion", // 0xA3
    "HP Volume Expansion", // 0xA4
    "BSD Slice", // 0xA5
    "OpenBSD Slice", // 0xA6
    "NeXT FS", // 0xA7
    "Mac OS X UFS", // 0xA8
    "NetBSD Slice", // 0xA9
    "Olivetti MS-DOS", // 0xAA
    "Mac OS X Boot", // 0xAB
    "Unused", // 0xAC
    "RISC OS", // 0xAD
    "ShagOS", // 0xAE
    "Mac OS X - HFS(+)", // 0xAF
    "Boot-Start Dummy Partition", // 0xB0
    "QNX Neutrino", // 0xB1
    "QNX Neutrino", // 0xB2
    "QNX Neutrino", // 0xB3
    "HP Volume Expansion", // 0xB4
    "Unused", // 0xB5
    "HP Volume Expansion", // 0xB6
    "BSDI FS", // 0xB7
    "BSDI Swap", // 0xB8
    "Unused", // 0xB9
    "Unused", // 0xBA
    "BootWizard", // 0xBB
    "Windows NT 4 Server", // 0xBC
    "BonnyDOS", // 0xBD
    "Solaris 8 Boot", // 0xBE
    "Solaris x86", // 0xBF
    "DR-DOS", // 0xC0
    "DR-DOS", // 0xC1
    "Power Boot - Hidden Linux - Native", // 0xC2
    "Power Boot - Hidden Linux - Swap", // 0xC3
    "DR-DOS", // 0xC4
    "DR-DOS", // 0xC5
    "Windows NT 4 Server", // 0xC6
    "Windows NT 4 Server", // 0xC7
    "DR-DOS Reserved", // 0xC8
    "DR-DOS Reserved", // 0xC9
    "DR-DOS Reserved", // 0xCA
    "DR-DOS", // 0xCB
    "DR-DOS", // 0xCC
    "CTOS - Memory Dump", // 0xCD
    "DR-DOS", // 0xCE
    "DR-DOS", // 0xCF
    "Multiuser DOS", // 0xD0
    "Multiuser DOS", // 0xD1
    "Unused", // 0xD2
    "Unused", // 0xD3
    "Multiuser DOS", // 0xD4
    "Multiuser DOS", // 0xD5
    "Multiuser DOS", // 0xD6
    "Unused", // 0xD7
    "CP/M-86", //0xD8
    "Unused", // 0xD9
    "Shielded Disk", // 0xDA
    "Dell DRMK", // 0xDB
    "Unused", // 0xDC
    "CTOS - Hidden Memory Dump", // 0xDD
    "Dell - Hidden FS", // 0xDE
    "DG/UX", // 0xDF
    "ST AVFS", // 0xE0
    "SpeedStor", // 0xE1
    "SpeedStor", // 0xE2
    "SpeedStor", // 0xE3
    "SpeedStor", // 0xE4
    "Tandy MS-DOS", // 0xE5
    "SpeedStor", // 0xE6
    "Unused", // 0xE7
    "LUKS", // 0xE8
    "Unused", // 0xE9
    "Unused", // 0xEA
    "BeOS", // 0xEB
    "SkyOS", // 0xEC
    "HP EFI", // 0xED
    "Microsoft EFI", // 0xEE
    "Intel EFI", // 0xEF
    "PA-RISC Linux", // 0xF0
    "SpeedStor", // 0xF1
    "Sperry IT/Unisys MS-DOS", // 0xF2
    "SpeedStor", // 0xF3
    "SpeedStor", // 0xF4
    "Prologue", // 0xF5
    "SpeedStor", // 0xF6
    "EFAT", // 0xF7
    "Unused", // 0xF8
    "Linux Persistent Cache", // 0xF9
    "x86 Emulator", // 0xFA
    "VMware VMFS", // 0xFB
    "VMware Swap", // 0xFC
    "FreeDOS", // 0xFD
    "Windows NT", // 0xFE
    "XENIX Bad Block Table" // 0xFF
};

// Print disk info
void print_disk(disk_info* disk) {
    if(disk->valid) {
        // Print disk signature as 8 hex characters
        printf("Disk signature: 0x%08x\n", disk->disk_signature);
        // Always print "Copy protected: " message
        printf("Copy protected: ");
        // Print yes or no depending on if it is copy protected or not
        if(disk->copy_protected) {
            printf("Yes\n"); 
        } else {
            printf("No\n");
        }

        if(disk->size > 0) {
            printf("Size: %gGB (%lu bytes)\n", (double)disk->size / (1024.0*1024.0*1024.0), disk->size);
        }
        printf("\n");
    }
}

// Print partition info
void print_partitions(partition_info* partitions, int num_partitions) {
    int count;
    for(count = 0; count < num_partitions; ++count) {
        if(partitions[count].valid) {
            // Print the partition count (+1 for 1-indexing)
            printf("Partition %d\n", count + 1);

            printf("Status: ");
            if(partitions[count].status) {
                printf("Active\n");
            } else {
                printf("Inactive\n");
            }

            printf("Partition type: %s\n", partition_types[partitions[count].partition_type]);

            printf("Cylinder, Head, Sector\n");
            printf("Start: C: %4d, H: %3d, S: %2d\n", partitions[count].start_cylinder, partitions[count].start_head, partitions[count].start_sector);
            printf("End:   C: %4d, H: %3d, S: %2d\n", partitions[count].end_cylinder, partitions[count].end_head, partitions[count].end_sector);

            printf("Start LBA: %llu - ", partitions[count].start_lba);
            printf("End LBA: %llu - ", partitions[count].start_lba + partitions[count].sector_count);
            printf("LBA Count: %llu ", partitions[count].sector_count);
            printf("\n\n");
        }
    }
}

// Print all disk & partition info
int print_info(char* device, disk_info* disk, partition_info* partitions, int num_partitions) {
    if(device == NULL) {
        fprintf(stderr, "ERROR: device string is null. Please pass in a valid device\n");
        return -1;
    }

    if(disk == NULL) {
        fprintf(stderr, "ERROR: disk_info struct pointer is null. Please pass in a valid struct pointer\n");
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

    printf("Disk: %s\n", device);

    print_disk(disk);
    print_partitions(partitions, num_partitions);

    return 0;
}
