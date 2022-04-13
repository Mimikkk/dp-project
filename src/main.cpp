#include "imports.hpp"
#include "domain/tasks/volunteer/communication.hpp"
#include "domain/tasks/poet/master.hpp"
#include "domain/tasks/volunteer/master.hpp"
#include "domain/tasks/poet/communication.hpp"
#include "domain/process.hpp"

fn main(i32 argc, char **argv) -> i32 {
  using namespace process;
  initialize(argc, argv);
  
  if (is_poet(Rank)) {
    using namespace poet;
    master_task();
    pthread_create(&communication_thread, nullptr, communication_task, nullptr);
  } else {
    using namespace volunteer;
    pthread_create(&communication_thread, nullptr, communication_task, nullptr);
    master_task();
  }

  finalize();
}
