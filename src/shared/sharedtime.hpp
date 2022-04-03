#pragma once
#include <shared/imports.hpp>

namespace sharedtime {
    typedef usize timestamp;

    namespace {
        inline fn handle_overflow() {}
        inline timestamp stamp = 0;

        inline fn tick() -> timestamp {
          if (++stamp == 0) handle_overflow();
          return stamp;
        }
    }

    inline fn get() -> timestamp { return stamp; }

    inline fn run(const std::function<void(timestamp)> &command) {
      while (process::Cooldown != 0) {
        --process::Cooldown;
        tick();
      }
      command(tick());
    }
}
