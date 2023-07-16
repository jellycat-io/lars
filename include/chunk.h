#ifndef lars_chunk_h
#define lars_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_CONSTANT_16,
  OP_CONSTANT_24,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_ADD,
  OP_SUBSTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NOT,
  OP_NEGATE,
  OP_RETURN,
} Opcode;

typedef struct {
  int line;
  int startInstruction;
  int count;
} LineEncoding;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  LineEncoding *lines;
  int lineCount;
  ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int getConstantCount(Chunk *chunk);
Value getTopStackConstant(Chunk *chunk);
int addConstant(Chunk *chunk, Value value);
void writeConstant(Chunk *chunk, Value value, int line);
int getLine(Chunk *chunk, int instruction);

#endif
