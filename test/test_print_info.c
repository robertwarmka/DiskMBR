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

    assert(print_info(device, NULL, partitions, NUM_PARTITIONS) == -1);
    assert(print_info(device, &disk, NULL, NUM_PARTITIONS) == -1);
    assert(print_info(device, &disk, partitions, 0) == -1);

    assert(print_info(device, &disk, partitions, NUM_PARTITIONS) == 0);
}

int main(int argc, char** argv) {
    test_print_info();

    return 0;
}
