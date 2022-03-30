#include <shared/utils/common.hpp>
#include <shared/imports.hpp>

char *str(const char *const format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  var size = vsnprintf(nullptr, 0, format, size_check);
  
  var buffer = (char *) malloc(size + 1);
  buffer[0] = '\0';
  vsprintf(buffer, format, arguments);
  buffer[size] = '\0';
  va_end(arguments);
  return buffer;
}
