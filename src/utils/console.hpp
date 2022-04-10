#pragma once
#include "../imports.hpp"
#include "../main.hpp"
#include "color.hpp"
#include "../domain/definitions/packet.hpp"

namespace console {
  using namespace color;

  namespace {
    #define printfln(fmt)               \
      do {                              \
      va_list arguments;                \
      va_start(arguments, format);      \
      vfprintf(stdout, fmt, arguments); \
      va_end(arguments);                \
      } while (false)

    fn name() {
      return str("%sRank %s%03d", Gray, dynamic((Rank + 1) % Size).get(), Rank);
    }

    fn time() {
      return str("%sAt %s%016lu", Gray, Blue, packet::timestamp());
    }

    fn nfo() {
      return str("%s: %s %s: %s %s:", Yellow, name().get(), Yellow, time().get(), Yellow);
    }
  }

  inline fn log(const char *format, ...) {
    printfln(str("Log   %s%s %s%s.%s\n", nfo().get(), Cyan, format, Yellow, Reset).get());
  }
  inline fn info(const char *format, ...) {
    printfln(str("%sInfo  %s%s %s%s.%s\n", Gray, nfo().get(), Gray, format, Yellow, Reset).get());
  }
  inline fn event(const char *format, ...) {
    printfln(str("%sEvent %s%s %s%s.%s\n", Green, nfo().get(), Green, format, Yellow, Reset).get());
  }
  inline fn error(const char *format, ...) {
    printfln(str("%sError %s%s %s%s.%s\n", Red, nfo().get(), Red, format, Yellow, Reset).get());
  }
}
