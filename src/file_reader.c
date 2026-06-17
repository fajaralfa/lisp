#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path, size_t *out_size) {
  FILE *fp = fopen(path, "rb");
  if (!fp)
    return NULL;

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);

  if (size < 0) {
    fclose(fp);
    return NULL;
  }

  char *buf = malloc(size + 1);
  if (!buf) {
    fclose(fp);
    return NULL;
  }

  size_t n = fread(buf, 1, size, fp);
  fclose(fp);

  if (n != (size_t)size) {
    free(buf);
    return NULL;
  }

  buf[size] = '\0';

  if (out_size)
    *out_size = (size_t)size;

  return buf;
}
