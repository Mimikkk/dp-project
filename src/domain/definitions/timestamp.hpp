#pragma once
#include "../../imports.hpp"
#include "Resource.hpp"

namespace timestamp {
  Resource clock(0);

  fn tick() {
    clock.update([](var value) { ++(*value); });
    return clock.value();
  }
  
  fn max(i32 other) {
    clock.update([&](var value) { *value = std::max(*value, other) + 1; });
    return clock.value();
  }
}
