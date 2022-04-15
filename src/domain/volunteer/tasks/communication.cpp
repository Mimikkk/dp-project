#include "communication.hpp"
#include "../../../utils/console.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../poet/action.hpp"

namespace volunteer {
  fn communication_task(void *pointer) -> void * {
    loop {
      switch (state::get()) {
        case state::Finish:
          console::event("Kończę prace naura");
          return pointer;
        case state::Cleaning:
          process::sleep(0.5);
          state::change(state::Idle);
          break;
        case state::Idle:
          console::info("Oczekuję paczki");
          var packet = packet::receive();
          console::info("Otrzymałem paczkę");
          console::log("Received tag: %x", packet.tag);
          state::change(state::Finish);
          break;
      }
    }
  }
}
