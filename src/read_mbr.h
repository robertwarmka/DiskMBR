#ifndef READ_MBR_H
#define READ_MBR_H

#include <sys/types.h>

off_t read_mbr(const char* device, char* buf, int buf_size);

#endif // READ_MBR_H
