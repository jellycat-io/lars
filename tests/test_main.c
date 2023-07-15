#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>

// Declare the test functions from test_chunk.c
extern void test_chunk_initChunk(void);
extern void test_chunk_writeChunk(void);
extern void test_chunk_addConstant(void);
extern void test_chunk_with_less_than_256_unique_constants(void);
extern void test_chunk_with_more_than_256_unique_constants(void);
// Declare the test functions from test_compiler.c
extern void test_compiler_number_literal(void);
extern void test_compiler_binary_expression(void);
extern void test_compiler_invalid_expression(void);

int main() {
  /* Initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // Add a suite to the registry for chunk tests
  CU_pSuite pChunkSuite = CU_add_suite("chunk_suite", 0, 0);
  if (NULL == pChunkSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  // Add the tests to the suite
  if ((NULL == CU_add_test(pChunkSuite, "test_chunk_initChunk",
                           test_chunk_initChunk)) ||
      (NULL == CU_add_test(pChunkSuite, "test_chunk_writeChunk",
                           test_chunk_writeChunk)) ||
      (NULL == CU_add_test(pChunkSuite, "test_chunk_addConstant",
                           test_chunk_addConstant)) ||
      (NULL == CU_add_test(pChunkSuite,
                           "test_chunk_with_less_than_256_unique_constants",
                           test_chunk_with_less_than_256_unique_constants)) ||
      (NULL == CU_add_test(pChunkSuite,
                           "test_chunk_with_more_than_256_unique_constants",
                           test_chunk_with_more_than_256_unique_constants))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add a suite to the registry for vm tests
  CU_pSuite pCompilerSuite = CU_add_suite("compiler_suite", 0, 0);
  if (NULL == pCompilerSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  // Add the tests to the suite
  if ((NULL == CU_add_test(pCompilerSuite, "test_compiler_number_literal",
                           test_compiler_number_literal)) ||
      (NULL == CU_add_test(pCompilerSuite, "test_compiler_binary_expression",
                           test_compiler_binary_expression)) ||
      (NULL == CU_add_test(pCompilerSuite, "test_compiler_invalid_expression",
                           test_compiler_invalid_expression))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
