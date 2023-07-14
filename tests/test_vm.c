#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "chunk.h"
#include "common.h"
#include "vm.h"

void test_vm_interpret_with_constant(void) {
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  writeConstant(&chunk, 3.14, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);

  freeVM();
  freeChunk(&chunk);
}

void test_vm_interpret_with_multiple_constants(void) {
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  writeConstant(&chunk, 42, 123);
  writeConstant(&chunk, 3.14, 123);
  writeConstant(&chunk, 1664, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);

  freeVM();
  freeChunk(&chunk);
}

void test_vm_interpret_with_invalid_instruction(void) {
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  writeChunk(&chunk, 0xff, 123); // Invalid instruction
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_RUNTIME_ERROR);

  freeVM();
  freeChunk(&chunk);
}

void test_vm_interpret_with_empty_chunk(void) {
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);

  freeVM();
  freeChunk(&chunk);
}

void test_vm_interpret_with_debug_trace(void) {
#ifdef DEBUG_TRACE_EXECUTION
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  writeConstant(&chunk, 3.14, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  InterpretResult result = interpret(&chunk);

  CU_ASSERT_EQUAL(result, INTERPRET_OK);

  freeVM();
  freeChunk(&chunk);
#endif
}

void test_vm_stack_push_pop(void) {
  initVM();

  push(true);
  push(3.14);

  CU_ASSERT_EQUAL(pop(), 3.14);
  CU_ASSERT_EQUAL(pop(), true);

  freeVM();
}

void test_vm_stack_overflow() {
  initVM();
  Chunk chunk;
  initChunk(&chunk);

  // Push more values than the stack can hold
  for (int i = 0; i < STACK_MAX; i++) {
    push(i);
  }

  // The next push should trigger a stack overflow
  CU_ASSERT_EQUAL(interpret(&chunk), INTERPRET_RUNTIME_ERROR);

  freeVM();
  freeChunk(&chunk);
}

void test_vm_stack_underflow(void) {
  initVM();

  // Attempt to pop from an empty stack
  CU_ASSERT_EQUAL(interpret(NULL), INTERPRET_RUNTIME_ERROR);

  freeVM();
}