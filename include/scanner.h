#ifndef lars_scanner_h
#define lars_scanner_h

typedef enum {
  // Single-character tokens.
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_SEMICOLON,
  TOKEN_PERCENT,
  // One or two character tokens.
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_MINUS,
  TOKEN_MINUS_EQUAL,
  TOKEN_PLUS,
  TOKEN_PLUS_EQUAL,
  TOKEN_SLASH,
  TOKEN_SLASH_EQUAL,
  TOKEN_STAR,
  TOKEN_STAR_EQUAL,
  // Literals.
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,
  // Keywords.
  TOKEN_AND,
  TOKEN_CLASS,
  TOKEN_DO,
  TOKEN_ELSE,
  TOKEN_FALSE,
  TOKEN_FOR,
  TOKEN_FUN,
  TOKEN_IF,
  TOKEN_LET,
  TOKEN_NIL,
  TOKEN_OR,
  TOKEN_PRINT,
  TOKEN_RETURN,
  TOKEN_SELF,
  TOKEN_SUPER,
  TOKEN_SWITCH,
  TOKEN_THEN,
  TOKEN_TRUE,
  TOKEN_WHILE,
  // Special tokens.
  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
} Token;

void initScanner(const char *source);
Token scanToken();

#endif