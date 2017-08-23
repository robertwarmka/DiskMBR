#include "get_partitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_valid_mbr() {
    char buf[MBR_SIZE] = {0};
    buf[510] = 0x55;
    buf[511] = 0xAA;
    assert(valid_mbr(buf, MBR_SIZE) == 0);
}

void test_populate_partition_info() {
    char buf[512] = {0};
    partition_info* partitions = malloc(NUM_PARTITIONS * sizeof(*partitions));
    memset(partitions, 0, NUM_PARTITIONS * sizeof(*partitions));

    assert(populate_partition_info(buf, partitions, NUM_PARTITIONS) == 0);

    free(partitions);
}

int main(int argc, char** argv) {
    test_valid_mbr();
    test_populate_partition_info();

    return 0;
}
