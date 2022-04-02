#pragma once
#include <shared/imports.hpp>
#include <domain/resource.hpp>

namespace rnd {
    inline fn generator() -> std::mt19937 {
      static std::random_device device;
      static std::mt19937 gen(device());
      return gen;
    }

    inline fn create_uniform(i32 min, i32 max) {
      return std::uniform_int_distribution<i32>(min, max);
    }

    inline fn use(std::uniform_int_distribution<i32> distribution) {
      static var gen = generator();
      return distribution(gen);
    }
}
