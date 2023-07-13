#include <stdlib.h>

#include "memory.h"

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
  // If newSize is 0, free the memory and return NULL
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  // Reallocate the memory block to newSize
  void *result = realloc(pointer, newSize);
  if (result == NULL)
    exit(1); // Exit the program if reallocation fails

  return result;
}
