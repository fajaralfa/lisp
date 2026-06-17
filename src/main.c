#include "file_reader.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }
  const char *filename = argv[1];

  Tokenizer tokenizer;

  size_t filesize;
  char *source = read_file(filename, &filesize);
  if (!source) {
    fprintf(stderr, "Failed to read file\n");
    return EXIT_FAILURE;
  }

  tokenizer_init(&tokenizer, source);
  tokenize(&tokenizer);

  tokenizer_debug(&tokenizer);

  tokenizer_free(&tokenizer);
  free(source);

  return EXIT_SUCCESS;
}
