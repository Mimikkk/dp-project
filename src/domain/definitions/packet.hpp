#pragma once
#include "../../imports.hpp"

namespace packet {
  extern fn timestamp() -> i32;

  typedef struct {
    i32 timestamp;
    i32 source;
    i32 data;
  } Packet;

  extern fn register_mpi_binding() -> void;
  extern fn free_mpi_binding() -> void;

  extern fn send(i32 tag, i32 destination, uptr<Packet> packet = nullptr) -> void;
  extern fn receive(i32 tag, i32 source = MPI_ANY_SOURCE, uptr<MPI_Status> status = nullptr) -> Packet;
}
