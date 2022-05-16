#pragma once
#include "../../imports.hpp"

namespace volunteer::state {
  enum State { Idle, Cleaning };

  extern fn get() -> State;
  extern fn change(State next) -> void;
}
