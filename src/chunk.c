#include "chunk.h"
#include "memory.h"
#include <stdlib.h>

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->lineCount = 0;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineEncoding, chunk->lines, chunk->capacity);
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
        GROW_ARRAY(LineEncoding, chunk->lines, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;

  // Check if chunk is on a new line
  if (chunk->lineCount == 0 ||
      chunk->lines[chunk->lineCount - 1].line != line) {
    LineEncoding lineEncoding = {line, chunk->count, 1};
    chunk->lines[chunk->lineCount] = lineEncoding;
    chunk->lineCount++;
  } else {
    chunk->lines[chunk->lineCount - 1].count++;
  }
  chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

int getLine(Chunk *chunk, int instruction) {
  LineEncoding *line = NULL;

  for (int i = 0; i < chunk->lineCount; i++) {
    if (chunk->lines[i].startInstruction <= instruction &&
        instruction <
            chunk->lines[i].startInstruction + chunk->lines[i].count) {
      line = &chunk->lines[i];
      break;
    }
  }

  if (line != NULL) {
    return line->line;
  } else {
    return -1;
  }
}