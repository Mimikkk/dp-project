#include "packet.hpp"

#include <memory>
#include "timestamp.hpp"
#include "../process.hpp"
#include "../../utils/console.hpp"

MPI_Datatype mpi_packet_t;

namespace packet {
  fn timestamp() -> i32 {
    return timestamp::clock.value();
  }

  fn send(i32 tag, i32 destination, Packet packet) -> void {
    packet.source = process::Rank;
    packet.timestamp = timestamp::tick();
    packet.tag = tag;

    MPI_Send(&packet, 1, mpi_packet_t, destination, tag, MPI_COMM_WORLD);
  }

  fn receive(i32 tag, i32 source, uptr<MPI_Status> status) -> Packet {
    Packet packet;
    MPI_Recv(&packet, 1, mpi_packet_t, source, tag, MPI_COMM_WORLD, status.get());
    packet.timestamp = timestamp::resolve_conflict(packet.timestamp);
    return packet;
  }

  fn register_mpi_binding() -> void {
    i32 blocklengths[] = {1, 1, 1, 1};
    MPI_Datatype types[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint offsets[] = {
      offsetof(Packet, source),
      offsetof(Packet, timestamp),
      offsetof(Packet, data),
      offsetof(Packet, tag)
    };

    MPI_Type_create_struct(4, blocklengths, offsets, types, &mpi_packet_t);
    MPI_Type_commit(&mpi_packet_t);
  }

  fn free_mpi_binding() -> void {
    MPI_Type_free(&mpi_packet_t);
  }
}
