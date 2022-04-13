#include "process.hpp"
#include "../utils/console.hpp"
#include "definitions/packet.hpp"

namespace process {
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

  fn initialize(i32 argc, byte **argv) -> void {
    i32 provided;

    Poets = (i32) strtol(argv[1], nullptr, 10);
    Volunteers = (i32) strtol(argv[2], nullptr, 10);

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    packet::register_mpi_binding();

    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);

    if (argc != 3) {
      console::error(
        "Użycie programu: mpirun -np <Poet + Volunteer Counts> -oversubscribe ./bin/application <Poet Count> <Volunteer Count>");
      MPI_Finalize();
      exit(-1);
    }

    try {
      Poets = (i32) strtol(argv[1], nullptr, 10);
      Volunteers = (i32) strtol(argv[2], nullptr, 10);

      if (Poets + Volunteers != Size) {
        throw std::domain_error("Liczba procesów musi być równa sumie Poetów i Wolontariuszy");
      }
    } catch (std::domain_error &e) {
      console::error(e.what());
      MPI_Finalize();
      exit(-1);
    } catch (...) {
      console::error("Błąd podczas inicjalizacji liczby poetów i wolontariuszy");
      MPI_Finalize();
      exit(-1);
    }

    check_thread_support(provided);
    srand(Rank);
  }

  fn finalize() -> void {
    console::log("Oczekuję na zakończenie wątku komunikacyjnego");
    pthread_join(communication_thread, nullptr);

    packet::free_mpi_binding();
    MPI_Finalize();
  }
}
