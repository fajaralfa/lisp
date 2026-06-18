#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

typedef struct Parser {
  Tokenizer *tokenizer;
  size_t current;
} Parser;

void parser_init(Parser *parser, Tokenizer *tokenizer);
void parse(Parser *parser);

#endif
