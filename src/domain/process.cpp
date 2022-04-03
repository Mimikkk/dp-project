#include <shared/imports.hpp>
#include <domain/process.hpp>
#include <shared/utils/console.hpp>
#include <domain/volunteer.hpp>
#include <domain/poet.hpp>

namespace process {
  fn is_poet(i32 rank) -> bool {
    return rank < poet::Count;
  }
  fn is_volunteer(i32 rank) -> bool {
    return rank < poet::Count + volunteer::Count;
  }
  fn cooldown() -> void {
    while (--Cooldown) sharedtime::tick();
  }

  fn initialize(int argc, char **argv) -> void {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Size);

    if (argc != 3) {
      printf("Usage: %s <poet Count> <volunteer Count>\n", argv[0]);
      finalize(Failure);
    }

    poet::Count = (i32) strtol(argv[1], nullptr, 10);
    volunteer::Count = (i32) strtol(argv[2], nullptr, 10);

    if (Size != poet::Count + volunteer::Count) {
      var difference = poet::Count + volunteer::Count - Size;

      console::error("Number of poets and volunteers is not equal to the number of processes.\n");
      console::error("Too %s by %d processes.\n", difference > 0 ? "many" : "few", abs(difference));
      finalize(Failure);
    }

    if (poet::Count < 0) {
      console::error("Number of poets must be a positive integer.\n");
      finalize(Failure);
    }

    if (volunteer::Count < 0) {
      console::error("Volunteer Count must be a positive integer.\n");
      finalize(Failure);
    }
  }
  fn finalize(ExitCode code) -> void {
    if (code != Success) {
      console::error("Error code: %d\n", code);
      MPI_Abort(MPI_COMM_WORLD, code);
    }

    MPI_Finalize();
    exit(code);
  }
}

