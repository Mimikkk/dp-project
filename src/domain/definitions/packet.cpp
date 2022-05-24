#include "packet.hpp"

#include <memory>
#include "timestamp.hpp"
#include "../process.hpp"
#include "../../utils/console.hpp"

namespace packet {
  typedef struct sendable_t {
    i32 timestamp;
    i32 source;
    i32 tag;
    void *data;

    static fn next(i32 tag, void *data = nullptr) -> sendable_t {
      return {timestamp::tick(), process::Rank, tag, data};
    }
    static const i32 fields = 3;
  } Sendable;

  fn timestamp() -> i32 {
    return timestamp::clock.value();
  }

  fn send(i32 tag, i32 destination, Sendable sendable, i32 extra = 0) {
    MPI_Send(&sendable, Sendable::fields + extra, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn send<void *>(i32 tag, i32 destination) -> void {
    send(tag, destination, Sendable::next(tag));
  }

  template<>
  fn send<i32>(i32 tag, i32 destination, i32 item)
  -> void {
    send(tag, destination, Sendable::next(tag, reinterpret_cast<void *>(item)), 1);
  }

  template<>
  fn send<vector<i32>>(i32 tag, i32 destination, vector<i32> items)
  -> void {
    send(tag, destination, Sendable::next(tag, items.data()), (i32) items.size());
  }

  template<>
  fn send<vector<tuple<i32, i32>>>(i32 tag, i32 destination, vector<tuple<i32, i32>> pairs)
  -> void {
    var _pairs = (i32 *) malloc(2 * pairs.size());
    for (var i = 0; i < pairs.size(); ++i) {
      _pairs[i + 0] = get<0>(pairs[i]);
      _pairs[i + 1] = get<1>(pairs[i]);
    }

    send(tag, destination, Sendable::next(tag, _pairs), 2 * (i32) pairs.size());
  }

  fn receive(i32 tag, i32 source, unique_ptr<MPI_Status> status, i32 extra) -> Sendable {
    Sendable sendable;
    MPI_Recv(&sendable, 3 + extra, MPI_INT, source, tag, MPI_COMM_WORLD, status.get());
    sendable.timestamp = timestamp::resolve_conflict(sendable.timestamp);
    return sendable;
  }

  template<typename T>
  fn from_sendable_with_data(Sendable sendable, T data = nullptr) {
    Packet<T> packet;
    packet.timestamp = sendable.timestamp;
    packet.tag = sendable.tag;
    packet.source = sendable.source;
    packet.data = data;
    return packet;
  }

  template<>
  fn receive<i32>(i32 tag, i32 source, unique_ptr<MPI_Status> status)
  -> Packet<i32> {
    let sendable = receive(tag, source, std::move(status), 1);

    return from_sendable_with_data(sendable, static_cast<int>(reinterpret_cast<usize>(sendable.data)));
  }

  template<>
  fn receive<vector<i32>>(i32 tag, i32 source, unique_ptr<MPI_Status> status)
  -> Packet<vector<i32>> {
    Packet<vector<i32>> packet;
    MPI_Probe(source, tag, MPI_COMM_WORLD, status.get());

    i32 count;
    MPI_Get_count(status.get(), MPI_INT, &count);

    Sendable sendable;
    MPI_Recv(&sendable, count, MPI_INT, 0, 0, MPI_COMM_WORLD, status.get());

    var items = vector<i32>(count - Sendable::fields);
    var raw = reinterpret_cast<i32 *>(sendable.data);
    for (var i = 0; i < count - Sendable::fields; ++i) items[i] = raw[i];

    return from_sendable_with_data(sendable, std::move(items));
  }

  template<>
  fn receive<vector<tuple<i32, i32>>>(i32 tag, i32 source, unique_ptr<MPI_Status> status)
  -> Packet<vector<tuple<i32, i32>>> {
    Packet<vector<tuple<i32, i32>>> packet;
    MPI_Probe(source, tag, MPI_COMM_WORLD, status.get());

    i32 count;
    MPI_Get_count(status.get(), MPI_INT, &count);

    Sendable sendable;
    MPI_Recv(&sendable, count, MPI_INT, 0, 0, MPI_COMM_WORLD, status.get());

    var items = vector<tuple<i32, i32>>(count - Sendable::fields);
    var raw = reinterpret_cast<i32 *>(sendable.data);
    for (var i = 0; i < count - Sendable::fields; i += 2) {
      get<0>(items[i]) = raw[i + 0];
      get<1>(items[i]) = raw[i + 1];
    }

    return from_sendable_with_data(sendable, std::move(items));
  }

  template<>
  fn receive<void *>(i32 tag, i32 source, unique_ptr<MPI_Status> status) -> Packet<void *> {
    return from_sendable_with_data(receive(tag, source, std::move(status)), nullptr);
  }
}
