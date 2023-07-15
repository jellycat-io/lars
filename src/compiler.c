#include "common.h"
#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif
#include "compiler.h"
#include "logger.h"
#include "scanner.h"
#include "value.h"
#include <stdio.h>

// Parser struct to track the current and previous tokens
typedef struct {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
} Parser;

// Precedence levels for operators
typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT, // =
  PREC_OR,         // or
  PREC_AND,        // and
  PREC_EQUALITY,   // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM,       // + -
  PREC_FACTOR,     // * /
  PREC_UNARY,      // ! -
  PREC_CALL,       // . ()
  PREC_PRIMARY
} Precedence;

// Function pointer type for parser functions
typedef void (*ParseFn)();

// Rule for parsing expressions
typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRule;

// Global parser and chunk
Parser parser;
Chunk *compilingChunk = NULL;

// Error handling functions
static void errorAt(Token *token, const char *message) {
  // Avoid cascading errors.
  if (parser.panicMode)
    return;
  parser.panicMode = true;

  fprintf(stderr, "%s", ANSI_COLOR_RED);
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  fprintf(stderr, "%s", ANSI_COLOR_RESET);
  parser.hadError = true;
}

static void error(const char *message) { errorAt(&parser.previous, message); }

static void errorAtCurrent(const char *message) {
  errorAt(&parser.current, message);
}

// Advance to the next token
static void advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanToken();
    if (parser.current.type != TOKEN_ERROR)
      break;

    errorAtCurrent(parser.current.start);
  }
};

// Consume the expected token type, or report an error
static void consume(TokenType type, const char *message) {
  if (parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

// Get the current chunk being compiled
static Chunk *currentChunk() { return compilingChunk; }

// Emit a single bytecode instruction
static void emitByte(uint8_t byte) {
  writeChunk(currentChunk(), byte, parser.previous.line);
}

// Emit two bytecode instructions
static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

// Emit a return instruction
static void emitReturn() { emitByte(OP_RETURN); }

// Create a constant value and return its index
static uint8_t makeConstant(Value value) {
  int constant = addConstant(currentChunk(), value);
  return constant;
}

// Emit a constant value instruction
static void emitConstant(Value value) {
  if (getConstantCount(compilingChunk) <= UINT8_MAX) {
    emitBytes(OP_CONSTANT, makeConstant(value));
  } else if (getConstantCount(compilingChunk) <= UINT16_MAX) {
    emitBytes(OP_CONSTANT_16, makeConstant(value));
  } else if (getConstantCount(compilingChunk) <= UINT24_MAX) {
    emitBytes(OP_CONSTANT_24, makeConstant(value));
  } else {
    error("Too many constants in one chunk.");
  }
}

// Finalize the compiler by emitting the return instruction
static void endCompiler() {
  emitReturn();
#ifdef DEBUG_PRINT_CODE
  if (!parser.hadError) {
    disassembleChunk(currentChunk(), "code");
  }
#endif
}

// Parse an expression based on precedence
static void parsePrecedence(Precedence precedence);
static ParseRule *getRule(TokenType type);

// Top-level expression parsing function
static void expression() { parsePrecedence(PREC_ASSIGNMENT); }

// Binary operator parsing function
static void binary() {
  TokenType operatorType = parser.previous.type;
  ParseRule *rule = getRule(operatorType);
  parsePrecedence((Precedence)(rule->precedence + 1));

  switch (operatorType) {
  case TOKEN_PLUS:
    emitByte(OP_ADD);
    break;
  case TOKEN_MINUS:
    emitByte(OP_SUBSTRACT);
    break;
  case TOKEN_STAR:
    emitByte(OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    emitByte(OP_DIVIDE);
    break;
  default:
    return;
  }
}

// Grouping expression parsing function
static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

// Numeric literal parsing function
static void number() {
  double value = strtod(parser.previous.start, NULL);
  emitConstant(value);
}

// Unary operator parsing function
static void unary() {
  TokenType operatorType = parser.previous.type;

  // Compile the operand.
  parsePrecedence(PREC_UNARY);

  // Emit the operator instruction.
  switch (operatorType) {
  case TOKEN_MINUS:
    emitByte(OP_NEGATE);
    break;
  default:
    return; // Unreachable.
  }
}

// Table of parse rules for different token types
ParseRule rules[] = {[TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
                     [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
                     [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
                     [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
                     [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
                     [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
                     [TOKEN_MINUS] = {unary, binary, PREC_TERM},
                     [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
                     [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
                     [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
                     [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
                     [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
                     [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
                     [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
                     [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
                     [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
                     [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
                     [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
                     [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
                     [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
                     [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
                     [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
                     [TOKEN_AND] = {NULL, NULL, PREC_NONE},
                     [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
                     [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
                     [TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
                     [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
                     [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
                     [TOKEN_IF] = {NULL, NULL, PREC_NONE},
                     [TOKEN_THEN] = {NULL, NULL, PREC_NONE},
                     [TOKEN_DO] = {NULL, NULL, PREC_NONE},
                     [TOKEN_NIL] = {NULL, NULL, PREC_NONE},
                     [TOKEN_OR] = {NULL, NULL, PREC_NONE},
                     [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
                     [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
                     [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
                     [TOKEN_SELF] = {NULL, NULL, PREC_NONE},
                     [TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
                     [TOKEN_LET] = {NULL, NULL, PREC_NONE},
                     [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
                     [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
                     [TOKEN_EOF] = {NULL, NULL, PREC_NONE}};

// Get the parse rule for a given token type
static ParseRule *getRule(TokenType type) { return &rules[type]; }

// Parse an expression based on precedence
static void parsePrecedence(Precedence precedence) {
  advance();
  ParseFn prefixRule = getRule(parser.previous.type)->prefix;
  if (prefixRule == NULL) {
    error("Expect expression.");
    return;
  }

  prefixRule();

  while (precedence <= getRule(parser.current.type)->precedence) {
    advance();
    ParseFn infixRule = getRule(parser.previous.type)->infix;
    infixRule();
  }
}

// Compile the source code into bytecode
bool compile(const char *source, Chunk *chunk) {
  initScanner(source);
  compilingChunk = chunk;
  parser.hadError = false;
  parser.panicMode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
  endCompiler();
  return !parser.hadError;
}
