#include "tokenizer.h"

int tokenize(FILE *fp) {
  char ch;
  while ((ch = fgetc(fp)) != EOF) {
    printf("%c", ch);
  }

  return 0;
}