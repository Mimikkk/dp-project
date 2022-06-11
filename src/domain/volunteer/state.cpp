#include "state.hpp"
#include "../definitions/Resource.hpp"

namespace volunteer::state {
  Resource resource(Idle);

  fn get() -> State { return resource.value(); }
  fn change(State next) -> void { resource.update([&](var state) { *state = next; }); }
  Resource<State> *raw() { return &resource; }
}
