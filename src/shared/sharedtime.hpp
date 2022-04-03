#pragma once
#include <shared/imports.hpp>
#include "process.hpp"

namespace sharedtime {
  typedef usize timestamp;

  namespace {
    inline fn handle_overflow() {}
    inline timestamp stamp = 0;
  }
  inline fn tick() -> timestamp {
    if (++stamp == 0) handle_overflow();
    return stamp;
  }

  inline fn get() -> timestamp { return stamp; }
}
