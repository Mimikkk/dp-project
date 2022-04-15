#include "master.hpp"
#include "../state.hpp"

namespace volunteer {
  fn master_task() -> void {
    while (state::get() != state::Finish);
  }
}
