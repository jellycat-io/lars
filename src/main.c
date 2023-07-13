#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <stdio.h>

int main() {
  Chunk chunk;
  initChunk(&chunk);
  writeChunk(&chunk, OP_RETURN);
  disassembleChunk(&chunk, "test_chunk");
  freeChunk(&chunk);
  return 0;
}
