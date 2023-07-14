#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>

int main() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  for (int i = 0; i < 300; i++) {
    writeConstant(&chunk, i, i);
  }
  writeChunk(&chunk, OP_RETURN, 300);

  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
