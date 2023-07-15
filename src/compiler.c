#include "compiler.h"
#include "common.h"
#include "logger.h"
#include "scanner.h"
#include <stdio.h>

typedef struct {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
} Parser;

Parser parser;
Chunk *compilingChunk = NULL;

static void errorAt(Token *token, const char *message) {
  // Avoid cascading errors.
  if (parser.panicMode)
    return;
  parser.panicMode = true;

  logError("[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    logError(" at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing
  } else {
    logError(" at '%.s'", token->length, token->start);
  }

  logError(": %s\n", message);
  parser.hadError = true;
}

static void error(const char *message) { errorAt(&parser.previous, message); }

static void errorAtCurrent(const char *message) {
  errorAt(&parser.current, message);
}

static void advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanToken();
    if (parser.current.type != TOKEN_ERROR)
      break;

    errorAtCurrent(parser.current.start);
  }
};

static void consume(TokenType type, const char *message) {
  if (parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

static Chunk *currentChunk() { return compilingChunk; }

static void emitByte(uint8_t byte) {
  writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, u_int8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

static void emitReturn() { emitByte(OP_RETURN); }

static void endCompiler() { emitReturn(); }

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