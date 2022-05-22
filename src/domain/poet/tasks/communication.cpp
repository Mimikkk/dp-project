#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../state.hpp"
#include "../action.hpp"
#include "../../../utils/random.hpp"
#include "../common.hpp"


namespace poet {
  fn communication_task(void *pointer) -> void * {
    let invitation_distribution = rnd::create_b_uniform(0.5);

    loop {
      let packet = packet::receive(action::RequestInvite);
      let decision = state::get() != state::Member && rnd::use(invitation_distribution);
      packet::send(action::ResponseInvite, packet.source, packet::Packet(decision));
    }
  }
}
