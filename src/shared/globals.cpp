#include <shared/globals.hpp>

bool is_poet(i32 rank) { return rank < Poets; }
bool is_volunteer(i32 rank) { return rank < Poets + Volunteers; }

void tick() {
  if (Clock == UINT8_MAX) {
    Clock = 0;
  } else {
    Clock++;
  }
}
void finalize(i32 errorcode) {
  if (errorcode != 0) {
    printf("Error code: %d\n", errorcode);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  MPI_Finalize();
  exit(1);
}

void initialize(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  if (argc != 3) {
    printf("Usage: %s <poet count> <volunteer count>\n", argv[0]);
    finalize(1);
  }

  Poets = (i32) strtol(argv[1], nullptr, 10);
  Volunteers = (i32) strtol(argv[2], nullptr, 10);

  if (Size != Poets + Volunteers) {
    printf("%d %d\n", Poets, Volunteers);
    printf("Number of poets and volunteers is not equal to the number of processes.\n");
    var difference = Poets + Volunteers - Size;
    printf("Too %s by %d processes.\n", difference > 0 ? "many" : "few", abs(difference));
    finalize(1);
  }

  if (Poets < 0) {
    printf("Poet count must be non-negative.\n");
    finalize(1);
  }
  
  if (Volunteers < 0) {
    printf("Volunteer count must be non-negative.\n");
    finalize(1);
  }
}
