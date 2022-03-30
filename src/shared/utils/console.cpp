#include <shared/utils/console.hpp>
#include <shared/utils/common.hpp>
#include <shared/utils/color.hpp>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

#define printfln(fmt) \
  va_list arguments; \
  va_start(arguments, format); \
  vfprintf(stdout, fmt, arguments); \
  va_end(arguments); \
  free((void *) fmt) \

namespace console {
    using namespace color;

    inline void log(const char *const format, ...) {
      printfln(str("%s%s%s.%s\n", Reset, format, Yellow, Reset));
    }
    inline void info(const char *const format, ...) {
      printfln(str("%sInfo%s:%s %s%s.%s\n", Gray, Yellow, Gray, format, Yellow, Reset));
    }
    inline void error(const char *const format, ...) {
      printfln(str("%sError%s:%s %s%s.%s\n", Red, Yellow, Reset, format, Yellow, Reset));
    }
    inline void event(const char *const format, ...) {
      printfln(str("%sEvent%s:%s %s%s.%s\n", Green, Yellow, Reset, format, Yellow, Reset));
    }
}

