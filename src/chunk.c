#include "chunk.h"
#include "memory.h"
#include <stdlib.h>

void initChunk(Chunk *chunk) {
  // Initialize the Chunk struct
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->lineCount = 0;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  // Free the dynamically allocated memory for code and lines arrays
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineEncoding, chunk->lines, chunk->capacity);
  // Free the value array
  freeValueArray(&chunk->constants);
  // Reinitialize the Chunk struct
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    // Grow the code and lines arrays using the GROW_ARRAY macro
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines =
        GROW_ARRAY(LineEncoding, chunk->lines, oldCapacity, chunk->capacity);
  }

  // Store the bytecode instruction in the code array
  chunk->code[chunk->count] = byte;

  // Check if chunk is on a new line
  if (chunk->lineCount == 0 ||
      chunk->lines[chunk->lineCount - 1].line != line) {
    // Create a new LineEncoding struct for the new line and store it in the
    // lines array
    LineEncoding lineEncoding = {line, chunk->count, 1};
    chunk->lines[chunk->lineCount] = lineEncoding;
    chunk->lineCount++;
  } else {
    // Increment the count of the current line
    chunk->lines[chunk->lineCount - 1].count++;
  }
  chunk->count++;
}

int getConstantCount(Chunk *chunk) { return chunk->constants.count - 1; }

Value getTopStackConstant(Chunk *chunk) {
  if (chunk->constants.count > 0) {
    // Return the last constant in the constants array
    return chunk->constants.values[chunk->constants.count - 1];
  } else {
    // No constants in the array, return NIL_VAL
    return NIL_VAL;
  }
}

int addConstant(Chunk *chunk, Value value) {
  // Write the value to the constants array and return its index
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

void writeConstant(Chunk *chunk, Value value, int line) {
  int constant = addConstant(chunk, value);

  if (constant <= UINT8_MAX) {
    // If the constant index fits within a byte, write OP_CONSTANT followed by
    // the constant index
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, constant, line);
  } else if (constant <= UINT16_MAX) {
    // If the constant index is larger, write OP_CONSTANT_16 followed by the
    // 2-byte constant index
    writeChunk(chunk, OP_CONSTANT_16, line);
    writeChunk(chunk, constant & 0xff, line);
    writeChunk(chunk, (constant >> 8) & 0xff, line);
  } else {
    // If the constant index is larger, write OP_CONSTANT_24 followed by the
    // 3-byte constant index
    writeChunk(chunk, OP_CONSTANT_24, line);
    writeChunk(chunk, constant & 0xff, line);
    writeChunk(chunk, (constant >> 8) & 0xff, line);
    writeChunk(chunk, (constant >> 16) & 0xff, line);
  }
}

int getLine(Chunk *chunk, int instruction) {
  LineEncoding *line = NULL;

  // Iterate over the line encodings in the lines array
  for (int i = 0; i < chunk->lineCount; i++) {
    if (chunk->lines[i].startInstruction <= instruction &&
        instruction <
            chunk->lines[i].startInstruction + chunk->lines[i].count) {
      // If the instruction index is within the range of a line, store the line
      // encoding and break the loop
      line = &chunk->lines[i];
      break;
    }
  }

  // If a matching line encoding is found, return the line number
  if (line != NULL) {
    return line->line;
  } else {
    // Otherwise, return -1
    return -1;
  }
}
