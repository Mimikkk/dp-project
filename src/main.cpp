#include <domain/process.hpp>
#include <shared/utils/console.hpp>
#include "shared/clock.hpp"

int main(int argc, char **argv) {
  using namespace process;

  initialize(argc, argv);

  console::log("Hello.., world!");
  console::info("Hello.., world!");
  console::error("Hello.., world!");
  console::event("Hello.., world!");

  sharedtime::run([](var time) {
      console::log("Time: %lu", time);
  });

  finalize(ExitCode::Success);
}
