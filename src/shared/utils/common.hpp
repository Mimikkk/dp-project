#pragma once
#include <shared/imports.hpp>
#include <stdnoreturn.h>

#define var auto
#define let const var
#define loop for(;;)
#define fn auto

inline fn str(const char *format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  var size = vsnprintf(nullptr, 0, format, size_check);

  uptr<char> buffer{new char[size + 1]};
  vsprintf(buffer.get(), format, arguments);
  va_end(arguments);
  return buffer;
}
