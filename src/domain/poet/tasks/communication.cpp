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

      state::raw()->update([&](var state) {
        if (state::get() == state::Member or not rnd::use(join_invite_distribution)) {
          packet::send(packet.source, action::ResponseInvite, false);
        } else {
          *state = state::Member;
          packet::send(packet.source, action::ResponseInvite, true);
        }
      });
    }
  }
}
