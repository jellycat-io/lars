#include "value.h"
#include "memory.h"
#include <stdio.h>

void initValueArray(ValueArray *array) {
  // Initialize the value array
  array->values = NULL;
  array->capacity = 0;
  array->count = 0;
}

void freeValueArray(ValueArray *array) {
  // Free the dynamically allocated memory for values array
  FREE_ARRAY(Value, array->values, array->capacity);
  // Reinitialize the value array
  initValueArray(array);
}

void writeValueArray(ValueArray *array, Value value) {
  if (array->capacity < array->count + 1) {
    // If capacity is not enough, grow the array
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values =
        GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
  }

  // Write the value to the array
  array->values[array->count] = value;
  array->count++;
}
