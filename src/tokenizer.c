#include "tokenizer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tokenizer_init(Tokenizer *t, char *source) {
  t->source = source;
  t->tokens = NULL;
  t->size = 0;
  t->capacity = 0;
  t->start = 0;
  t->pos.ln = 1;
  t->pos.col = 1;
}

void tokenizer_free(Tokenizer *t) {
  free(t->tokens);
  t->source = NULL;
  t->tokens = NULL;
  t->size = 0;
  t->capacity = 0;
}

static char *token_type_label[] = {
  [TOKEN_IDENTIFIER]="TOKEN_IDENTIFIER",
  [TOKEN_NUMBER]="TOKEN_NUMBER",
  [TOKEN_LEFT_BRACE]="TOKEN_LEFT_BRACE",
  [TOKEN_RIGHT_BRACE]="TOKEN_RIGHT_BRACE",
  [TOKEN_DOUBLE_QUOTE]="TOKEN_DOUBLE_QUOTE",
  [TOKEN_PLUS]="TOKEN_PLUS",
  [TOKEN_MINUS]="TOKEN_MINUS",
  [TOKEN_STAR]="TOKEN_STAR",
  [TOKEN_SLASH]="TOKEN_SLASH",
  [TOKEN_EQ]="TOKEN_EQ",
  [TOKEN_LESSER]="TOKEN_LESSER",
  [TOKEN_LESSER_EQ]="TOKEN_LESSER_EQ",
  [TOKEN_GREATER]="TOKEN_GREATER",
  [TOKEN_GREATER_EQ]="TOKEN_GREATER_EQ",
};

static void print_literal(Tokenizer *t, Token tok) {
  printf("\"");
  for (size_t i = tok.from; i < tok.to; i++) {
    printf("%c", t->source[i]);
  }
  printf("\"");
}

void tokenizer_debug(Tokenizer *t) {
  for (size_t i = 0; i < t->size; i++) {
    Token tok = t->tokens[i];
    printf("range[%ld:%ld]\tpos[%ld:%ld]\t %-20s\t", tok.from, tok.to,
           tok.pos.ln, tok.pos.col, token_type_label[tok.type]);
    print_literal(t, tok);
    printf("\n");
  }
}

static void tokenizer_push(Tokenizer *t, Token tok) {
  if (t->size >= t->capacity) {
    size_t new_cap = (t->capacity == 0) ? 4 : t->capacity * 2;
    Token *new_tok = realloc(t->tokens, new_cap * sizeof(Token));
    if (!new_tok) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }

    t->tokens = new_tok;
    t->capacity = new_cap;
  }

  t->tokens[t->size++] = tok;
}

static bool is_numeric(char c) { return c >= '0' && c <= '9'; }

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static void tokenize_number(Tokenizer *t) {
  size_t start = t->start;
  char c = t->source[t->start];
  TokenPos pos = t->pos;
  while (is_numeric(c)) {
    t->start++;
    t->pos.col++;
    c = t->source[t->start];
  }
  if (c == '.') {
    t->start++;
    t->pos.col++;
    c = t->source[t->start];
    while (is_numeric(c)) {
      t->start++;
      t->pos.col++;
      c = t->source[t->start];
    }
  }
  tokenizer_push(t, (Token){TOKEN_NUMBER, start, t->start, pos});
}

static void tokenize_identifier(Tokenizer *t) {
  size_t start = t->start;
  char c = t->source[t->start];
  TokenPos pos = t->pos;
  while (is_alpha(c) || is_numeric(c)) {
    t->pos.col++;
    t->start++;
    c = t->source[t->start];
  }
  tokenizer_push(t, (Token){TOKEN_IDENTIFIER, start, t->start, pos});
}

void tokenize(Tokenizer *t) {
  size_t len = strlen(t->source);
  while (t->start < len) {
    char c = t->source[t->start];
    switch (c) {
    case '(': {
      tokenizer_push(t,
                     (Token){TOKEN_LEFT_BRACE, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case ')': {
      tokenizer_push(
          t, (Token){TOKEN_RIGHT_BRACE, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case '+': {
      tokenizer_push(t, (Token){TOKEN_PLUS, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case '-': {
      tokenizer_push(t, (Token){TOKEN_MINUS, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case '*': {
      tokenizer_push(t, (Token){TOKEN_STAR, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case '/': {
      tokenizer_push(t, (Token){TOKEN_SLASH, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case '=': {
      tokenizer_push(t, (Token){TOKEN_EQ, t->start, t->start + 1, t->pos});
      t->pos.col++;
      t->start++;
      break;
    }
    case '<': {
      if (t->source[t->start + 1] == '=') {
        tokenizer_push(t, (Token){TOKEN_LESSER_EQ, t->start, t->start + 2, t->pos});
        t->pos.col += 2;
        t->start += 2;
      } else {
        tokenizer_push(t, (Token){TOKEN_LESSER, t->start, t->start + 1, t->pos});
        t->pos.col++;
        t->start++;
      }
      break;
    }
    case '>': {
      if (t->source[t->start + 1] == '=') {
        tokenizer_push(t, (Token){TOKEN_GREATER_EQ, t->start, t->start + 2, t->pos});
        t->pos.col += 2;
        t->start += 2;
      } else {
        tokenizer_push(t, (Token){TOKEN_GREATER, t->start, t->start + 1, t->pos});
        t->pos.col++;
        t->start++;
      }
      break;
    }
    case '\n':
      t->pos.col = 1;
      t->pos.ln++;
      t->start++;
      break;
    case ' ':
    case '\t':
      t->pos.col++;
      t->start++;
      break;
    default:
      if (is_numeric(c)) {
        tokenize_number(t);
      } else if (is_alpha(c)) {
        tokenize_identifier(t);
      } else {
        fprintf(stderr, "Invalid token at %ld:%ld\n", t->pos.ln, t->pos.col);
        exit(EXIT_FAILURE);
      }
    }
  }
}
