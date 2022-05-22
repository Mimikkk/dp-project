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
      let packet = packet::receive();

      switch (state::get()) {
        case state::ClubOwner:
          if (packet.tag == action::RequestInvite) {
            packet::send(action::ResponseInvite, packet.source, packet::Packet(false));
          }
          break;
        case state::Otherwise:
          if (not poet::IsMember and packet.tag == action::RequestInvite) {
            if (rnd::use(invitation_distribution)) {
              packet::send(action::ResponseInvite, packet.source, packet::Packet(true));
            } else {
              packet::send(action::ResponseInvite, packet.source, packet::Packet(false));
            }
          }
      }
    }

    return pointer;
  }
}
