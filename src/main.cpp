#include "imports.hpp"
#include "utils/console.hpp"
#include "domain/definitions/packet.hpp"
#include "domain/tasks/communication.hpp"
#include "domain/tasks/master.hpp"

i32 Size, Rank;
pthread_t communication_thread;

fn check_thread_support(i32 provided) {
  console::info("Thread Support");

  switch (provided) {
    case MPI_THREAD_SINGLE:
      console::error("Brak wsparcia dla wielu wątków");
      MPI_Finalize();
      exit(-1);
    case MPI_THREAD_FUNNELED:
      console::info("Tylko te wątki, ktore wykonały mpi_init_thread mogą wykonać wołania do biblioteki MPI");
      break;
    case MPI_THREAD_SERIALIZED:
      console::info("Tylko jeden watek naraz może wykonać wołania do biblioteki MPI");
      break;
    case MPI_THREAD_MULTIPLE:
      console::info("Pełne wsparcie dla wątków");
      break;
    default:
      break;
  }
}

fn initialize(i32 argc, char **argv) {
  i32 provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  packet::register_mpi_binding();
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  check_thread_support(provided);
  srand(Rank);

  pthread_create(&communication_thread, nullptr, communication_task, nullptr);
}

fn finalize() {
  console::log("czekam na wątek \"komunikacyjny\"");

  pthread_join(communication_thread, nullptr);

  packet::free_mpi_binding();
  MPI_Finalize();
}

fn main(i32 argc, char **argv) -> i32 {
  initialize(argc, argv);

  master_task();

  finalize();
}
