#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

typedef enum TokenType {
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_DOUBLE_QUOTE,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_EQ,
  TOKEN_LESSER,
  TOKEN_LESSER_EQ,
  TOKEN_GREATER,
  TOKEN_GREATER_EQ,
} TokenType;

typedef struct TokenPos {
  size_t ln;
  size_t col;
} TokenPos;

typedef struct Token {
  TokenType type;
  size_t from;
  size_t to;
  TokenPos pos;
} Token;

typedef struct Tokenizer {
  char *source;
  Token *tokens;
  size_t size;
  size_t capacity;
  size_t start;
  TokenPos pos;
} Tokenizer;

void tokenize(Tokenizer *t);
void tokenizer_init(Tokenizer *t, char *source);
void tokenizer_free(Tokenizer *t);
void tokenizer_debug(Tokenizer *t);

#endif
