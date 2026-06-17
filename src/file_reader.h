#ifndef FILE_READER_H
#define FILE_READER_H

#include <stddef.h>

char *read_file(const char *path, size_t *out_size);

#endif
