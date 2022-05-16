#include "communication.hpp"
#include "../../definitions/packet.hpp"
#include "../action.hpp"
#include "../../process.hpp"
#include "../../volunteer/action.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"

namespace poet {
  fn communication_task(void *pointer) -> void * {
    let sleep_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));

    loop {  
      console::info("Requesting room service...");
      packet::send(action::RequestRoomService, process::random_volunteer());

      packet::receive(volunteer::action::ResponseRoomServiced);
      console::info("Requested room serviced!");
  
      console::info("sleeping...");
      process::sleep(rnd::use(sleep_distribution));
    }

    return pointer;
  }
}
