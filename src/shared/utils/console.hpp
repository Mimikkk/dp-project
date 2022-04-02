#pragma once
#include <shared/utils/console.hpp>
#include <shared/utils/common.hpp>
#include <shared/utils/color.hpp>
#include <shared/clock.hpp>

namespace console {
    using namespace color;

    namespace {
        #define printfln(fmt) \
          do {                \
          va_list arguments;                \
          va_start(arguments, format);      \
          vfprintf(stdout, fmt, arguments); \
          va_end(arguments);                \
          } while (false)

        fn name() -> uptr<char> {
          return str("%sAn %s%-9s %sranked %s%03d",
                     Gray, Blue, process::is_poet(process::Rank) ? "poet" : "volunteer", Gray, Blue, process::Rank);
        }

        fn time() -> uptr<char> {
          return str("%sAt %s%016lu", Gray, Blue, sharedtime::get());
        }

        fn nfo() -> uptr<char> {
          return str("%s: %s %s: %s %s:", Yellow, name().get(), Yellow, time().get(), Yellow);
        }
    }

    inline fn log(const char *format, ...) {
      printfln(str("Log   %s %s %s%s.%s\n", nfo().get(), Cyan, format, Yellow, Reset).get());
    }
    inline fn info(const char *format, ...) {
      printfln(str("%sInfo  %s %s %s%s.%s\n", Gray, nfo().get(), Gray, format, Yellow, Reset).get());
    }
    inline fn event(const char *format, ...) {
      printfln(str("%sEvent %s %s %s%s.%s\n", Green, nfo().get(), Green, format, Yellow, Reset).get());
    }
    inline fn error(const char *format, ...) {
      printfln(str("%sError %s %s %s%s.%s\n", Red, nfo().get(), Red, format, Yellow, Reset).get());
    }
}
