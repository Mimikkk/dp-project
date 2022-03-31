#pragma once
#include <shared/imports.hpp>

#define var auto
#define let const var
#define loop for(;;)

inline char *str(const char *format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  var size = vsnprintf(nullptr, 0, format, size_check);

  var buffer = (char *) malloc(size + 1);
  vsprintf(buffer, format, arguments);
  va_end(arguments);
  return buffer;
}
