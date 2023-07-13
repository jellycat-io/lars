#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

void test_chunk_initChunk(void) {
  Chunk chunk;
  initChunk(&chunk);

  CU_ASSERT_EQUAL(chunk.count, 0);
  CU_ASSERT_EQUAL(chunk.capacity, 0);
  CU_ASSERT_PTR_NULL(chunk.code);
  CU_ASSERT_TRUE(chunk.constants.count == 0);

  freeChunk(&chunk);
}

void test_chunk_writeChunk(void) {
  Chunk chunk;
  initChunk(&chunk);

  writeChunk(&chunk, 0x01);
  writeChunk(&chunk, 0x02);
  writeChunk(&chunk, 0x03);

  CU_ASSERT_EQUAL(chunk.count, 3);
  CU_ASSERT_EQUAL(chunk.capacity, 8);
  CU_ASSERT_PTR_NOT_NULL(chunk.code);

  freeChunk(&chunk);
}

void test_chunk_addConstant(void) {
  Chunk chunk;
  initChunk(&chunk);

  int index1 = addConstant(&chunk, 3.14);
  int index2 = addConstant(&chunk, true);

  CU_ASSERT_EQUAL(index1, 0);
  CU_ASSERT_EQUAL(index2, 1);
  CU_ASSERT_EQUAL(chunk.constants.count, 2);

  freeChunk(&chunk);
}