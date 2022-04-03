#pragma once
#include "imports.hpp"
#include "shared/utils/common.hpp"
#include "sharedtime.hpp"

namespace process {
  enum ExitCode { Success = 0, Failure = 1 };

  inline i32 Rank;
  inline i32 Size;
  inline MPI_Status Status;

  extern fn finalize(ExitCode code) -> void;
  extern fn initialize(i32 argc, char **argv) -> void;
  extern fn is_poet(i32 rank) -> bool;
  extern fn is_volunteer(i32 rank) -> bool;

  [[noreturn]] inline fn run(fn logic) { loop { logic(); }}

  using sharedtime::timestamp;
  inline fn receive(i32 tag) -> timestamp {
    sharedtime::tick();
    var time = 0lu;
    MPI_Recv(&time, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &process::Status);
    return time;
  }

  inline fn send(i32 tag, i32 dest) -> timestamp {
    var time = sharedtime::tick();
    MPI_Send(&time, 1, MPI_UNSIGNED_LONG, dest, tag, MPI_COMM_WORLD);
    return time;
  }
}
