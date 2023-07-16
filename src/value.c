#include "value.h"
#include "logger.h"
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

void printValue(Value value) {
  switch (value.type) {
  case VAL_BOOL:
    logWithColor(ANSI_COLOR_GREEN, "%s", AS_BOOL(value) ? "true" : "false");
    break;
  case VAL_NIL:
    logWithColor(ANSI_COLOR_GREEN, "%s", "nil");
    break;
  case VAL_NUMBER:
    logWithColor(ANSI_COLOR_GREEN, "%g", AS_NUMBER(value));
    break;
  }
}
