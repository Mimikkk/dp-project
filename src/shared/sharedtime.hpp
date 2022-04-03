#pragma once
#include <shared/imports.hpp>
#include <domain/process.hpp>

namespace sharedtime {
  typedef usize timestamp;

  namespace {
    inline fn handle_overflow() {}
    inline timestamp stamp = 0;
  }
  inline fn tick() -> timestamp {
    if (++stamp == 0) handle_overflow();
    return stamp;
  }

  inline fn get() -> timestamp { return stamp; }

  inline fn receive(i32 tag) -> timestamp {
    tick();
    var time = 0lu;
    MPI_Recv(&time, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &process::Status);
    return time;
  }

  inline fn send(i32 tag, i32 dest) -> timestamp {
    var time = tick();
    MPI_Send(&time, 1, MPI_UNSIGNED_LONG, dest, tag, MPI_COMM_WORLD);
    return time;
  }
}
