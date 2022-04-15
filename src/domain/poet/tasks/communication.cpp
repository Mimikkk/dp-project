#include "communication.hpp"
#include "../../../utils/console.hpp"

namespace poet {
  fn communication_task(void *pointer) -> void * {
    console::event("I wrote this poem just to show it");
    
    return nullptr;
  }
}
