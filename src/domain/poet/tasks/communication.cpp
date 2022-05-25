#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../state.hpp"
#include "../action.hpp"
#include "../../../utils/random.hpp"

namespace poet {
  [[noreturn]] fn communication_task(void *pointer) -> void * {
    let join_invite_distribution = rnd::create_b_uniform(0.5);

    loop {
      let packet = packet::receive(action::RequestInvite);

      if (state::get() == state::Member) {
        packet::send(packet.source, action::ResponseInvite, false);
      } else {
        let decision = rnd::use(join_invite_distribution);
        if (decision) state::change(state::Member);
        packet::send(packet.source, action::ResponseInvite, decision);
      }
    }
  }
}
