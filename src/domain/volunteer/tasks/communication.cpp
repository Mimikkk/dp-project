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
          console::event("Nie mam sprzątanka pora na spanko");
          break;
      }
    }
  }
}
