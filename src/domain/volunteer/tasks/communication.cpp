#include "communication.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"

namespace volunteer {
  fn communication_task(void *pointer) -> void * {
    loop {
      switch (state::get()) {
        case state::Finish:
          console::event("Kończę prace naura");
          return pointer;
        case state::Idle:
          console::event("Oczekuję czegoś");
          let packet = packet::receive(100);
          console::event("Otrzymałem coś");
          console::event("Otrzymałem %d w %d od %d", packet.data, packet.timestamp, packet.source);
          state::change(state::Finish);
          break;
      }
    }
  }
}
