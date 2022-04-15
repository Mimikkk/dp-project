#include "state.hpp"

namespace poet::state {
  Resource resource(Finish);

  fn get() -> State { return resource.value(); }
  fn change(State next) -> void { resource.update([&](var state) { *state = next; }); }
}
