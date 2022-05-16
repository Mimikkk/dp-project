#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../action.hpp"
#include "../../process.hpp"
#include "../../volunteer/action.hpp"

namespace poet {
  fn communication_task(void *pointer) -> void * {
    loop {
      packet::send(action::RequestRoomService, process::random_volunteer());
      packet::receive(volunteer::action::ResponseRoomServiced);
    }

    return pointer;
  }
}
