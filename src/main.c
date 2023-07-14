#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>

int main() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 6, 123);
  writeConstant(&chunk, 7, 123);
  writeChunk(&chunk, OP_MULTIPLY, 123);
  writeConstant(&chunk, 7, 123);
  writeChunk(&chunk, OP_ADD, 123);
  writeConstant(&chunk, 7, 123);
  writeChunk(&chunk, OP_DIVIDE, 123);
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 124);

  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
