#include "console.hpp"
#include "color.hpp"
#include "../domain/definitions/timestamp.hpp"
#include "../domain/process.hpp"

namespace console {
  namespace {
    using namespace color;
    using namespace process;
    using namespace timestamp;

    #define printfln(fmt)               \
    do {                              \
    va_list arguments;                \
    va_start(arguments, format);      \
    vfprintf(stdout, fmt, arguments); \
    va_end(arguments);                \
    } while (false)

    fn name() {
      let job = is_poet(Rank) ? str("%s%-9s", Silver, "Poet") : str("%s%s", Magenta, "Volunteer");
      return str("%sRank %s %s%03d", Gray, job.get(), dynamic((Rank + 1) % Size).get(), Rank);
    }

    fn time() {
      return str("%sAt %s%08lu", Gray, Blue, current());
    }

    fn nfo() {
      return str("%s: %s %s: %s %s:", Yellow, name().get(), Yellow, time().get(), Yellow);
    }
  }

  fn log(const char *format, ...) -> void {
    printfln(str("Log   %s%s %s%s.%s\n", nfo().get(), Cyan, format, Yellow, Reset).get());
  }
  fn info(const char *format, ...) -> void {
    printfln(str("%sInfo  %s%s %s%s.%s\n", Gray, nfo().get(), Gray, format, Yellow, Reset).get());
  }
  fn event(const char *format, ...) -> void {
    printfln(str("%sEvent %s%s %s%s.%s\n", Green, nfo().get(), Green, format, Yellow, Reset).get());
  }
  fn error(const char *format, ...) -> void {
    printfln(str("%sError %s%s %s%s.%s\n", Red, nfo().get(), Red, format, Yellow, Reset).get());
  }
}
