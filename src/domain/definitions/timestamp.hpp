#pragma once
#include "../../imports.hpp"
#include "Resource.hpp"

namespace timestamp {
  inline Resource clock(0);

  inline fn current() {
    return clock.value();
  }

  inline fn tick() {
    clock.update([](var value) { ++(*value); });
    return clock.value();
  }

  inline fn resolve_conflict(i32 other) {
    clock.update([&](var value) { *value = std::max(*value, other) + 1; });
    return clock.value();
  }
}
