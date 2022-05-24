#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../state.hpp"
#include "../action.hpp"
#include "../../../utils/random.hpp"
#include "../common.hpp"

namespace poet {
  [[noreturn]] fn communication_task(void *pointer) -> void * {
    loop {
      if (state::get() == state::Member) {
        let packet = packet::receive(action::RequestInvite);
        packet::send(action::ResponseInvite, packet.source, false);
      }
    }
  }
}
