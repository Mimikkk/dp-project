#include <shared/imports.hpp>
#include <shared/globals.hpp>
#include <shared/utils/console.hpp>

int main(int argc, char **argv) {
  initialize(argc, argv);
  console::log("Hello.., world!");
  console::info("Hello.., world!");
  console::error("Hello.., world!");
  console::event("Hello.., world!");
  MPI_Finalize();
}
