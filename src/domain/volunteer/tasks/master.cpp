#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"

namespace volunteer {
  fn master_task() -> void {
    console::event("wrr gdzie sprzątanko...?");
    process::sleep(0.5);
    console::event("Zmieniam stan na koniec");
    state::change(state::Finish);
  }
}
