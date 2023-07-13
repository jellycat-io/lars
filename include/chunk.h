#ifndef lars_chunk_h
#define lars_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} Opcode;

typedef struct {
  int startInstruction;
  int line;
} LineStart;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  LineStart *lines;
  int lineCount;
  ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);
int getLineForInstruction(Chunk *chunk, int instruction);

#endif