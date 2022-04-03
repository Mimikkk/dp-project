#include <domain/volunteer.hpp>
#include "shared/process.hpp"
#include <domain/poet.hpp>

using namespace process;
fn main(i32 argc, char **argv) -> i32 {
  initialize(argc, argv);

  if (process::is_poet(Rank)) poet::logic();
  else volunteer::logic();

  finalize(Success);
}
