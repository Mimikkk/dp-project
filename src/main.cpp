#include <domain/process.hpp>
#include <shared/utils/console.hpp>
#include "shared/sharedtime.hpp"
#include <shared/utils/random.hpp>

fn main(i32 argc, char **argv) -> i32 {
  using namespace process;

  initialize(argc, argv);

  sharedtime::run([&](let time) {
    if (rnd::boolean()) {
      console::info("I want to create a libacja!");
    } else {
      console::info("I'll wont create a libacja. its dumb");
    }
  });

  finalize(ExitCode::Success);
}
