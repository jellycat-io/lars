#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "vm.h"

void test_compiler_number_literal(void) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  const char *source = "42";
  bool result = compile(source, &chunk);

  CU_ASSERT_TRUE(result);
  CU_ASSERT_EQUAL(chunk.count, 3);

  freeChunk(&chunk);
  freeVM();
}

void test_compiler_binary_expression(void) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  const char *source = "1 + 2 * 3";
  bool result = compile(source, &chunk);

  CU_ASSERT_TRUE(result);
  CU_ASSERT_EQUAL(chunk.count, 9);

  freeChunk(&chunk);

  freeVM();
}

void test_compiler_invalid_expression(void) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  const char *source = "1 +";
  bool result = compile(source, &chunk);

  CU_ASSERT_FALSE(result);

  freeChunk(&chunk);

  freeVM();
}