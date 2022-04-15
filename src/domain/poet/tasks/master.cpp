#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../action.hpp"
#include "../state.hpp"

namespace poet {
  fn master_task() -> void {
    using namespace process;
    console::event("My name is Philip");

    packet::send(action::RequestRoomService, random_volunteer());

    state::change(state::Finish);
  }
}
