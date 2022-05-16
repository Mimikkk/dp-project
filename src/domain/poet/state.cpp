#include "state.hpp"

namespace poet::state {
  Resource resource(Idle);

  fn get() -> State { return resource.value(); }
  fn change(State next) -> void { resource.update([&](var state) { *state = next; }); }
}
