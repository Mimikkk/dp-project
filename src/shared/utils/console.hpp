#pragma once

namespace console {
    extern void log(const char *format, ...);
    extern void info(const char *format, ...);
    extern void event(const char *format, ...);
    extern void error(const char *format, ...);
}
