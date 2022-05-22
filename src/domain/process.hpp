#pragma once
#include "../imports.hpp"
#include "../utils/random.hpp"

namespace process {
  inline i32 Size = 1;
  inline i32 Rank = 0;
  inline i32 Poets = 0;
  inline i32 Volunteers = 0;
  inline pthread_t communication_thread;

  extern fn initialize(i32 argc, byte **argv) -> void;
  extern fn finalize() -> void;

  inline fn is_me(i32 rank) { return rank == Rank; }
  inline fn is_volunteer(i32 rank) { return rank >= Poets and rank < Volunteers; }
  inline fn is_poet(i32 rank) { return rank < Poets; }

  inline fn foreach_poet_except_me(fn action) {
    for (var poet = 0; poet < Poets; ++poet) {
      if (is_me(poet)) continue;
      action(poet);
    }
  }
  inline fn foreach_volunteer_except_me(fn action) {
    for (var volunteer = Poets; volunteer < Poets + Volunteers; ++volunteer) {
      if (is_me(volunteer)) continue;
      action(volunteer);
    }
  }

  inline fn random_poet() {
    static let uniform = rnd::create_i_uniform(0, Poets - 1);
    return rnd::use(uniform);
  }
  inline fn random_volunteer() {
    static let uniform = rnd::create_i_uniform(Poets, Poets + Volunteers - 1);
    return rnd::use(uniform);
  }


  inline fn sleep(f64 seconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds((i64) (seconds * 1000.0 / 1)));
  }
}
