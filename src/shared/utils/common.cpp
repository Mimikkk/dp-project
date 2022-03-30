#include <shared/utils/common.hpp>
#include <shared/imports.hpp>

char *str(const char *const format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  var size = vsnprintf(nullptr, 0, format, size_check);
  var buffer = (char *) calloc(size + 1, sizeof(char));
  vsprintf(buffer, format, arguments);
  va_end(arguments);
  return buffer;
}
