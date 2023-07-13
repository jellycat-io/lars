#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <stdio.h>

int main() {
  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 3.14);
  writeChunk(&chunk, OP_CONSTANT);
  writeChunk(&chunk, constant);
  writeChunk(&chunk, OP_RETURN);

  disassembleChunk(&chunk, "test_chunk");
  freeChunk(&chunk);
  return 0;
}
