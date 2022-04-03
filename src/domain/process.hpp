#pragma once
#include <shared/imports.hpp>
#include <shared/utils/common.hpp>
#include <shared/sharedtime.hpp>

namespace process {
  enum ExitCode { Success = 0, Failure = 1 };

  inline i32 Rank;
  inline i32 Size;
  inline i32 Cooldown;
  inline MPI_Status Status;

  fn finalize(ExitCode code) -> void;
  fn initialize(i32 argc, char **argv) -> void;

  extern fn is_poet(i32 rank) -> bool;
  extern fn is_volunteer(i32 rank) -> bool;


  inline fn cooldown() {
    while (process::Cooldown != 0) {
      --process::Cooldown;
      sharedtime::tick();
    }
  }

  inline fn receive(auto &command) {
    cooldown();
    sharedtime::run([](let) {
      var time = 0lu;
      MPI_Recv(&time, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
    });
  }

  inline fn send() {
    cooldown();
    sharedtime::run([](let time
    ) {
      MPI_Send(&time, 1, MPI_UNSIGNED_LONG, Status.MPI_SOURCE, Status.MPI_TAG, MPI_COMM_WORLD);
    });
  }
}
