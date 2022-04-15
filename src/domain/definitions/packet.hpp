#pragma once
#include "../../imports.hpp"

namespace packet {
  extern fn timestamp() -> i32;

  typedef struct packet_t {
    i32 timestamp;
    i32 source;
    i32 data;
    i32 tag;
  } Packet;

  extern fn register_mpi_binding() -> void;
  extern fn free_mpi_binding() -> void;

  extern fn send(i32 tag, i32 destination, Packet packet = Packet()) -> void;
  extern fn receive(i32 tag = MPI_ANY_TAG, i32 source = MPI_ANY_SOURCE, uptr<MPI_Status> status = nullptr) -> Packet;
}
