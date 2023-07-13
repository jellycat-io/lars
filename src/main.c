#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>

int main() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 3.14, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
