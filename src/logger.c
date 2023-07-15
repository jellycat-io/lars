#include "logger.h"
#include <stdarg.h>

void logWithColor(const char *color, const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("%s", color);
  vprintf(format, args);
  printf("%s", ANSI_COLOR_RESET);

  va_end(args);
}

void logError(const char *format, ...) {
  va_list args;
  va_start(args, format);

  printf("%s", ANSI_COLOR_RED);
  fprintf(stderr, format, args);
  printf("%s", ANSI_COLOR_RESET);

  va_end(args);
}

char *formatColor(const char *message, const char *color) {
  static char
      formatted[256]; // Assuming a maximum string length of 255 characters
  snprintf(formatted, sizeof(formatted), "%s%s%s", color, message,
           ANSI_COLOR_RESET);
  return formatted;
}