#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "dsa.h"
#include <stdbool.h>

typedef struct SExprAtom {
  char type;
  union {
    TokenType symbol;
    char *number;
    char *str;
    bool boolean;
  } data;
} SExprAtom;

typedef union SExprData {
  struct SExprAtom atom;
  List list;
} SExprData;

typedef struct SExpr {
  char type;
  SExprData data;
} SEXpr;

typedef struct Parser {
  Tokenizer *tokenizer;
  size_t current;
} Parser;

void parser_init(Parser *parser, Tokenizer *tokenizer);
void parse(Parser *parser);

#endif
