#include <diskmbr.h>
#include <read_mbr.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_read_mbr() {
    char device[] = "test";
    char buf[MBR_SIZE] = {0};

    // Error and return -1 if device is null
    assert(read_mbr(NULL, buf, MBR_SIZE) == -1);
    // Error and return -1 if buf is null
    assert(read_mbr(device, NULL, MBR_SIZE) == -1);
    // Error and return -1 if buf_size < MBR_SIZE
    assert(read_mbr(device, buf, 0) == -1);

    // Will fail because we don't have permission to execute, otherwise if we do it will fail because it
    // won't find a device called "test". A proper test would need sudo permissions to get a successful
    // run on this call if the device were a real device
    assert(read_mbr(device, buf, MBR_SIZE) == -1);
}

int main(int argc, char** argv) {
    test_read_mbr();

    return 0;
}
