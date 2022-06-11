#include "communication.hpp"
#include "../state.hpp"
#include "../action.hpp"
#include "../../process.hpp"
#include "../../../utils/random.hpp"
#include "../../definitions/packet.hpp"

[[noreturn]] fn volunteer::communication_task(void *pointer) -> void * {
  let cleaning_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));

  loop {
    state::raw()->wait();
    process::sleep(rnd::use(cleaning_distribution));
    packet::send(process::Rank, action::ResponseServiceEnd, process::Rank);
  }
}
