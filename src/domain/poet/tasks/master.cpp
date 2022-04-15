#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../volunteer/action.hpp"

namespace poet {
  fn master_task() -> void {
    using namespace process;

    loop {
      console::event("Informuję o potrzebie sprzątania jakiegoś wolontariusza.");
      packet::send(action::RequestRoomService, random_volunteer());

      console::event("Oczekuję informacji, że bedzie sprzątane...");
      packet::receive(volunteer::action::RoomCleaning);
      console::event("Będzie sprzatane, mogę kończyć!");
    };
  }
}
