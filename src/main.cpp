#include <domain/process.hpp>
#include <shared/utils/console.hpp>
#include "shared/sharedtime.hpp"
#include "domain/poet.hpp"

fn main(i32 argc, char **argv) -> i32 {
  using namespace process;

  initialize(argc, argv);

  console::log("Hello.., world!");
  console::info("Hello.., world!");
  console::error("Hello.., world!");
  console::event("Hello.., world!");

  sharedtime::run([](let time) {
      console::info("I want to create a club!");
      console::info("I'll pick %d %d %d", poet::pick_resource(), poet::pick_resource(), poet::pick_resource());
  });

  finalize(ExitCode::Success);
}
