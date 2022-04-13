#pragma once
#include "../imports.hpp"
#include "../domain/process.hpp"
#include "color.hpp"
#include "../domain/definitions/packet.hpp"

namespace console {
  extern fn log(const char *format, ...) -> void;
  extern fn info(const char *format, ...) -> void;
  extern fn event(const char *format, ...) -> void;
  extern fn error(const char *format, ...) -> void;
}
