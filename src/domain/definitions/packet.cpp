#include "packet.hpp"
#include "../../main.hpp"
#include "../../utils/common.hpp"
#include "timestamp.hpp"

MPI_Datatype mpi_packet_t;

namespace packet {
  fn timestamp() -> i32 {
    return timestamp::clock.value();
  }

  fn send(Packet *packet, i32 destination, i32 tag) -> void {
    packet->source = Rank;
    packet->timestamp = timestamp::tick();
    MPI_Send(packet, 1, mpi_packet_t, destination, tag, MPI_COMM_WORLD);
  }

  fn receive(i32 source, i32 tag, MPI_Status *status = nullptr) -> Packet {
    Packet packet;
    MPI_Recv(&packet, 1, mpi_packet_t, source, tag, MPI_COMM_WORLD, status);
    packet.timestamp = timestamp::max(packet.timestamp);

    return packet;
  }

  fn register_mpi_binding() -> void {
    i32 blocklengths[] = {1, 1, 1};
    MPI_Datatype types[] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint offsets[] = {offsetof(Packet, source), offsetof(Packet, timestamp), offsetof(Packet, data)};

    MPI_Type_create_struct(3, blocklengths, offsets, types, &mpi_packet_t);
    MPI_Type_commit(&mpi_packet_t);
  }

  fn free_mpi_binding() -> void {
    MPI_Type_free(&mpi_packet_t);
  }
}
