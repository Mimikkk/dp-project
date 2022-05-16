#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../action.hpp"
#include "../../process.hpp"
#include "../../volunteer/action.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"

namespace poet {
  fn communication_task(void *pointer) -> void * {
    loop {
      console::info("Requesting room service...");
      packet::send(action::RequestRoomService, process::random_volunteer());

      packet::receive(volunteer::action::ResponseRoomServiced);
      console::info("Requested room serviced!");

      console::info("sleeping...");
      process::sleep(rnd::real(0.8, 2.0));
    }

    return pointer;
  }
}
