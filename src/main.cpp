#include <domain/process.hpp>
#include <shared/utils/console.hpp>
#include "shared/clock.hpp"

fn main(int argc, char **argv) -> int {
  using namespace process;

  initialize(argc, argv);

  console::log("Hello.., world!");
  console::info("Hello.., world!");
  console::error("Hello.., world!");
  console::event("Hello.., world!");

  sharedtime::run([](let time) {
      console::log("Time: %lu", time);
  });

  finalize(ExitCode::Success);
}
