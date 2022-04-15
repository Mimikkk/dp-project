#pragma once
#include "../../imports.hpp"

namespace volunteer::state {
  enum State { Idle, Finish };

  extern fn get() -> State;
  extern fn change(State next) -> void;
}
