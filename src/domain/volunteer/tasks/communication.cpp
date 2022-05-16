#include "communication.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../../utils/random.hpp"

namespace volunteer {
  fn communication_task(void *pointer) -> void * {

    loop {
      console::info("Awaiting room service...");
      let packet = packet::receive(poet::action::RequestRoomService);

      console::info("Cleaning...");
      process::sleep(rnd::real(0.8, 2.0));

      packet::send(action::ResponseRoomServiced, packet.source);
      console::info("Room serviced!");
    }

    return pointer;
  }
}
