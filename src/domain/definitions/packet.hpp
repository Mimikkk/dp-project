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

  extern fn send(Packet *packet, i32 destination, i32 tag) -> void;
  extern fn receive(i32 source, i32 tag, MPI_Status *status) -> Packet;
}
