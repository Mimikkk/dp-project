#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../state.hpp"
#include "../action.hpp"
#include "../../../utils/random.hpp"
#include "../common.hpp"


namespace poet {
  fn communication_task(void *pointer) -> void * {
    loop {
      let packet = packet::receive(action::RequestInvite);
      packet::send(action::ResponseInvite, packet.source, packet::Packet(false));
    }
  }
}
