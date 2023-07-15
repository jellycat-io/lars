#define CUNIT_VERBOSE

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>

void test_chunk_initChunk(void) {
  Chunk chunk;
  initChunk(&chunk);

  // Check that chunk is properly initialized
  CU_ASSERT_EQUAL(chunk.count, 0);
  CU_ASSERT_EQUAL(chunk.capacity, 0);
  CU_ASSERT_PTR_NULL(chunk.code);
  CU_ASSERT_PTR_NULL(chunk.lines);
  CU_ASSERT_EQUAL(chunk.lineCount, 0);
  CU_ASSERT_TRUE(chunk.constants.count == 0);

  freeChunk(&chunk);
}

void test_chunk_writeChunk(void) {
  Chunk chunk;
  initChunk(&chunk);

  // Write three bytes to the chunk
  writeChunk(&chunk, 0x01, 123);
  writeChunk(&chunk, 0x02, 123);
  writeChunk(&chunk, 0x03, 124);

  // Check that chunk has the correct state after writing
  CU_ASSERT_EQUAL(chunk.count, 3);
  CU_ASSERT_EQUAL(chunk.capacity, 8);
  CU_ASSERT_EQUAL(chunk.lineCount, 2);
  CU_ASSERT_PTR_NOT_NULL(chunk.code);
  CU_ASSERT_PTR_NOT_NULL(chunk.lines);

  freeChunk(&chunk);
}

void test_chunk_addConstant(void) {
  Chunk chunk;
  initChunk(&chunk);

  // Add two constants to the chunk
  int index1 = addConstant(&chunk, 3.14);
  int index2 = addConstant(&chunk, true);

  // Check that constants are added correctly
  CU_ASSERT_EQUAL(index1, 0);
  CU_ASSERT_EQUAL(index2, 1);
  CU_ASSERT_EQUAL(chunk.constants.count, 2);

  freeChunk(&chunk);
}

void test_chunk_with_less_than_256_unique_constants(void) {
  Chunk chunk;
  initChunk(&chunk);

  // Add 255 unique constants to the chunk
  for (int i = 0; i < 255; i++) {
    writeConstant(&chunk, i, 1);
  }

  // Check that the count of chunk's bytes is equal to 255 * 2 bytes
  CU_ASSERT_EQUAL(chunk.count, 510);

  // Check that all opcodes in the chunk are OP_CONSTANT
  for (int i = 0; i < chunk.count; i += 2) {
    CU_ASSERT_EQUAL(chunk.code[i], OP_CONSTANT);
  }

  freeChunk(&chunk);
}

void test_chunk_with_more_than_256_unique_constants(void) {
  Chunk chunk;
  initChunk(&chunk);

  // Add 300 unique constants to the chunk
  for (int i = 0; i < 300; i++) {
    writeConstant(&chunk, i, 1);
  }

  // Check that the count of chunk's bytes is 256 * 2 (OP_CONSTANT + 1-byte
  // constant index) + 44 * 3 (OP_CONSTANT_16 + 2-byte constant index) = 644
  // entries

  CU_ASSERT_EQUAL(chunk.count, 644);

  // Check that the first 255 * 2 entries in the chunk use OP_CONSTANT
  for (int i = 0; i < 256 * 2; i += 2) {
    CU_ASSERT_EQUAL(chunk.code[i], OP_CONSTANT);
  }

  // Check that the remaining entries in the chunk use OP_CONSTANT_16
  for (int i = 256 * 2; i < chunk.count; i += 3) {
    CU_ASSERT_EQUAL(chunk.code[i], OP_CONSTANT_16);
  }

  freeChunk(&chunk);
}
