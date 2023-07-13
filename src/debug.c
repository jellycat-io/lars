#include "debug.h"
#include <stdio.h>

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

int getLineForInstruction(Chunk *chunk, int instruction) {
  int lineStartIndex = 0;
  while (lineStartIndex + 1 < chunk->lineCount &&
         chunk->lines[lineStartIndex + 1].startInstruction <= instruction) {
    lineStartIndex++;
  }
  return chunk->lines[lineStartIndex].line;
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);
  int line = getLineForInstruction(chunk, offset);

  if (offset > 0 && line == getLineForInstruction(chunk, offset - 1)) {
    printf("   | ");
  } else {
    printf("%4d ", line);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset;
  }
}

void printValue(Value value) { printf("%g", value); }
