#include "chunk.h"
#include "memory.h"
#include <stdlib.h>

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->lineCount = 0; // initialize lineCount
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineStart, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines =
        GROW_ARRAY(LineStart, chunk->lines, oldCapacity, chunk->capacity);
  }

  // Store the bytecode instruction in the code array
  chunk->code[chunk->count] = byte;

  // If the last instruction was from a different line, store the new line
  // information.
  if (chunk->count == 0 || chunk->lines[chunk->lineCount - 1].line != line) {
    LineStart lineStart = {chunk->count, line};
    chunk->lines[chunk->lineCount] = lineStart;
    chunk->lineCount++;
  }

  chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
