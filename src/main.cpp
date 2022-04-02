#include <domain/process.hpp>
#include <shared/utils/console.hpp>
#include "shared/sharedtime.hpp"

fn main(int argc, char **argv) -> int {
  using namespace process;

  initialize(argc, argv);

  console::log("Hello.., world!");
  console::info("Hello.., world!");
  console::error("Hello.., world!");
  console::event("Hello.., world!");

  sharedtime::run([](let time) {
      console::info("I want to create a club!");
      console::log("Time: %lu", time);
  });

  finalize(ExitCode::Success);
}
