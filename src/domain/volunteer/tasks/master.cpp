#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../../utils/random.hpp"

namespace volunteer {
  [[noreturn]] fn master_task() -> void {
    let cleaning_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));

    loop {
      console::info("Awaiting room service...");
      let packet = packet::receive(poet::action::RequestRoomService);

      console::info("Cleaning...");
      process::sleep(rnd::use(cleaning_distribution));

      packet::send(packet.source, action::ResponseRoomServiced);
      console::info("Room serviced!");
    }
  }
}
