#include "parser.h"
#include <stdio.h>

void parser_init(Parser *parser, Tokenizer *tokenizer) {
  parser->current = 0;
  parser->tokenizer = tokenizer;
}

static Token next(Parser *parser) {
  return parser->tokenizer->tokens[parser->current++];
}

static Token peek(Parser *parser) {
  return parser->tokenizer->tokens[parser->current];
}

void parse(Parser *parser) {
  Token token = next(parser);
  if (token.type == TOKEN_LEFT_BRACE) {
  }
}
