#include <diskmbr.h>
#include <print_info.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_print_info() {
    char device[] = "test device";
    disk_info disk = {0};
    partition_info* partitions = malloc(NUM_PARTITIONS * sizeof(*partitions));
    memset(partitions, 0, NUM_PARTITIONS * sizeof(*partitions));

    // Error and return -1 if device is null
    assert(print_info(NULL, &disk, partitions, NUM_PARTITIONS) == -1);
    // Error and return -1 if disk is null
    assert(print_info(device, NULL, partitions, NUM_PARTITIONS) == -1);
    // Error and return -1 if partitions is null
    assert(print_info(device, &disk, NULL, NUM_PARTITIONS) == -1);
    // Error and return -1 if num partitions < NUM_PARTITIONS
    assert(print_info(device, &disk, partitions, 0) == -1);

    // Pass - will only print device name, but that's still a successful run
    assert(print_info(device, &disk, partitions, NUM_PARTITIONS) == 0);
}

int main(int argc, char** argv) {
    test_print_info();
    return 0;
}
