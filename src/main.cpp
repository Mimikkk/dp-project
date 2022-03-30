#include <shared/globals.hpp>

int main(int argc, char **argv) {
  initialize(argc, argv);

  printf("Hello, world!\n");

  MPI_Finalize();
}
