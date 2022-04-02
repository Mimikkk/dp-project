#pragma once
#include <shared/utils/console.hpp>
#include <shared/utils/common.hpp>
#include <shared/utils/color.hpp>
#include <shared/clock.hpp>

namespace console {
    using namespace color;

    namespace {
        #define printfln(fmt) \
          va_list arguments; \
          va_start(arguments, format); \
          vfprintf(stdout, fmt, arguments); \
          va_end(arguments); \
          free((void *) (fmt))

        char *stamp() {
          return str("%s %03d%s : %sAt %s%lu%s",
                     Magenta, process::Rank, Yellow, Reset, Magenta, sharedtime::get(), Reset);
        }
    }

    inline void log(const char *format, ...) {
      printfln(str("Log   %s %s:%s %s%s.%s\n", stamp(), Yellow, Cyan, format, Yellow, Reset));
    }
    inline void info(const char *format, ...) {
      printfln(str("%sInfo  %s %s:%s %s%s.%s\n", Gray, stamp(), Yellow, Gray, format, Yellow, Reset));
    }
    inline void event(const char *format, ...) {
      printfln(str("%sError %s %s:%s %s%s.%s\n", Red, stamp(), Yellow, Reset, format, Yellow, Reset));
    }
    inline void error(const char *format, ...) {
      printfln(str("%sEvent %s %s:%s %s%s.%s\n", Green, stamp(), Yellow, Reset, format, Yellow, Reset));
    }
}
