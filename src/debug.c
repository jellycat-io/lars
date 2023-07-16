#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <stdio.h>
#include <string.h>

static int simpleInstruction(const char *name, int offset) {
  // Print the name of a simple instruction
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  int constant = chunk->code[offset + 1];

  // Handle OP_CONSTANT_16 and OP_CONSTANT_24
  if (strcmp(name, "OP_CONSTANT_16") == 0) {
    // Retrieve the constant value from the 2-byte index
    constant += chunk->code[offset + 2] << 8;
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 3;
  } else if (strcmp(name, "OP_CONSTANT_24") == 0) {
    // Retrieve the constant value from the 3-byte index
    constant += chunk->code[offset + 2] << 8;
    constant += chunk->code[offset + 3] << 16;
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
  }

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

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);
  int line = getLine(chunk, offset);

  if (offset > 0 && line == getLine(chunk, offset - 1)) {
    printf("   | ");
  } else {
    printf("%4d ", line);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_CONSTANT_24:
    return constantInstruction("OP_CONSTANT_24", chunk, offset);
  case OP_NIL:
    return simpleInstruction("OP_NIL", offset);
  case OP_TRUE:
    return simpleInstruction("OP_TRUE", offset);
  case OP_FALSE:
    return simpleInstruction("OP_FALSE", offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBSTRACT:
    return simpleInstruction("OP_SUBSTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_NOT:
    return simpleInstruction("OP_NOT", offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset;
  }
}
