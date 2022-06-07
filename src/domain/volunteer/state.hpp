#pragma once
#include "../../imports.hpp"
#include "../definitions/Resource.hpp"

namespace volunteer::state {
  enum State { Idle, Servicing };

  extern fn get() -> State;
  extern fn change(State next) -> void;
  extern Resource<State> *raw();
}
