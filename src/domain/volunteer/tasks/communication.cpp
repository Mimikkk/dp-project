#include "communication.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"

namespace volunteer {
  fn communication_task(void *pointer) -> void * {
    loop {
      let packet = packet::receive(poet::action::RequireRoomService);
      packet::send(action::InformAboutCleanedRoom, packet.source);
    }

    return pointer;
  }
}
