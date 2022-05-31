#pragma once
#include "../../imports.hpp"
#include "../definitions/Resource.hpp"

namespace poet::state {
  enum State { Idle, Member };

  extern fn get() -> State;
  extern fn change(State next) -> void;
  extern Resource<State> *raw();
}
