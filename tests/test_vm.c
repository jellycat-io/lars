#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "chunk.h"
#include "vm.h"

void test_interpret_with_constant(void) {
  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 3.14, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);
}

void test_interpret_with_multiple_constants(void) {
  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 42, 123);
  writeConstant(&chunk, 3.14, 123);
  writeConstant(&chunk, 1664, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);
}

void test_interpret_with_invalid_instruction(void) {
  Chunk chunk;
  initChunk(&chunk);

  writeChunk(&chunk, 0xff, 123); // Invalid instruction
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_RUNTIME_ERROR);
}

void test_interpret_with_empty_chunk(void) {
  Chunk chunk;
  initChunk(&chunk);

  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);
}

void test_interpret_with_debug_trace(void) {
#ifdef DEBUG_TRACE_EXECUTION
  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 3.14, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);
#endif
}