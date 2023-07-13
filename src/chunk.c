#include "chunk.h"
#include "memory.h"
#include <stdlib.h>

void initChunk(Chunk *chunk) {
  // Initialize the chunk's fields
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->lineCount = 0;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  // Free the dynamically allocated memory
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineEncoding, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  // Reinitialize the chunk
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    // If the capacity is not enough, grow the arrays
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines =
        GROW_ARRAY(LineEncoding, chunk->lines, oldCapacity, chunk->capacity);
  }

  // Store the byte in the code array
  chunk->code[chunk->count] = byte;

  // Check if chunk is on a new line
  if (chunk->lineCount == 0 ||
      chunk->lines[chunk->lineCount - 1].line != line) {
    // If it's a new line, store line information
    LineEncoding lineEncoding = {line, chunk->count, 1};
    chunk->lines[chunk->lineCount] = lineEncoding;
    chunk->lineCount++;
  } else {
    // If it's the same line, increment the count for the last line
    chunk->lines[chunk->lineCount - 1].count++;
  }
  chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
  // Add the constant value to the constants array
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
