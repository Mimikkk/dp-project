#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"

namespace volunteer {
  fn master_task() -> void {
    console::event("wrr gdzie sprzątanko...?");
    process::sleep(0.5);
    console::event("Wysłałem coś");
    packet::send(100, process::random_volunteer());
  }
}
