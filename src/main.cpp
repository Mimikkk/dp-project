#include <memory>

#include "imports.hpp"
#include "domain/poet/tasks/communication.hpp"
#include "domain/poet/tasks/master.hpp"
#include "domain/volunteer/tasks/communication.hpp"
#include "domain/volunteer/tasks/master.hpp"
#include "domain/process.hpp"
#include "domain/definitions/packet.hpp"
#include "utils/console.hpp"

fn main(i32 argc, char **argv) -> i32 {
  using namespace process;
  initialize(argc, argv);

//  if (is_poet(Rank)) {
//    using namespace poet;
//    pthread_create(&communication_thread, nullptr, communication_task, nullptr);
//    master_task();
//  } else {
//    using namespace volunteer;
//    pthread_create(&communication_thread, nullptr, communication_task, nullptr);
//    master_task();
//  }
//

  if (process::Rank == 0) {
    console::log("Sending...");

    console::log("Receiving...");
  }


  finalize();
}
