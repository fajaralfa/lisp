#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *filename = argv[1];

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  tokenize(fp);

  fclose(fp);
  return EXIT_SUCCESS;
}