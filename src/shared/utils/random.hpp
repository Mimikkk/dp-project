#pragma once
#include <shared/imports.hpp>
#include <domain/resource.hpp>

namespace rnd {
    namespace {
        inline std::mt19937 generator() {
          static std::random_device device;
          static std::mt19937 generator(device());
          return generator;
        }
    }

    inline fn create_uniform(i32 min, i32 max) {
      return std::uniform_int_distribution<i32>(min, max);
    }

    inline fn use(std::uniform_int_distribution<i32> distribution) {
      static let gen = generator();
      return distribution.operator()(gen);
    }
}