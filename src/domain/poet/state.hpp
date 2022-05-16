#pragma once
#include "../../imports.hpp"
#include "../definitions/Resource.hpp"

namespace poet::state {
  enum State { Idle };

  extern fn get() -> State;
  extern fn change(State next) -> void;
}
