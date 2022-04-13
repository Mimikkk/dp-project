#pragma once
#include "../imports.hpp"

namespace process {
  inline i32 Size = 1;
  inline i32 Rank = 0;
  inline i32 Poets = 0;
  inline i32 Volunteers = 0;
  inline pthread_t communication_thread;

  extern fn initialize(i32 argc, byte **argv) -> void;
  extern fn finalize() -> void;

  inline fn is_volunteer(i32 rank) { return rank >= Poets and rank < Volunteers; }
  inline fn is_poet(i32 rank) { return rank < Poets; }
}
