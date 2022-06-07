#include "packet.hpp"

#include <memory>
#include "timestamp.hpp"
#include "../process.hpp"
#include "../../utils/console.hpp"

namespace packet {
  template<>
  fn send<void *>(i32 destination, i32 tag, void *ignored) -> void {
    let stamp = timestamp::tick();
    MPI_Send(&stamp, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn send<i32>(i32 destination, i32 tag, i32 item) -> void {
    const struct { i32 timestamp; i32 item; } data = {timestamp::tick(), item};
    MPI_Send(&data, 2, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn send<bool>(i32 destination, i32 tag, bool item) -> void {
    const struct { i32 timestamp; bool item; } data = {timestamp::tick(), item};
    MPI_Send(&data, 2, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn send<vector<i32>>(i32 destination, i32 tag, vector<i32> items) -> void {
    let size = (i32) (1 + items.size());
    var data = (i32 *) malloc(size);
    data[0] = timestamp::tick();

    for (var i = 0; i < items.size(); ++i) data[i + 1] = items[i];

    MPI_Send(data, size, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn send<vector<bool>>(i32 destination, i32 tag, vector<bool> items) -> void {
    let size = (i32) (1 + items.size());
    var data = (i32 *) malloc(size);
    data[0] = timestamp::tick();

    for (var i = 0; i < items.size(); ++i) data[i + 1] = items[i];

    MPI_Send(data, size, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn send<vector<tuple<i32, i32>>>(i32 destination, i32 tag, vector<tuple<i32, i32>> items) -> void {
    let size = (i32) (1 + 2 * items.size());
    var data = (i32 *) malloc(size);

    data[0] = timestamp::tick();
    for (var i = 0; i < items.size(); ++i) {
      let [first, second] = items[i];
      data[i + 1] = first;
      data[i + 2] = second;
    }

    MPI_Send(data, size, MPI_INT, destination, tag, MPI_COMM_WORLD);
  }

  template<>
  fn receive<void *>(i32 tag, i32 source) -> Packet<void *> {
    MPI_Status status;
    i32 stamp;
    MPI_Recv(&stamp, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

    return {
      timestamp::resolve_conflict(stamp),
      status.MPI_SOURCE,
      status.MPI_TAG,
    };
  }

  template<>
  fn receive<i32>(i32 tag, i32 source) -> Packet<i32> {
    MPI_Status status;

    struct { i32 timestamp; i32 item; } data{};
    MPI_Recv(&data, 2, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

    return {
      timestamp::resolve_conflict(data.timestamp),
      status.MPI_SOURCE,
      status.MPI_TAG,
      data.item,
    };
  }

  template<>
  fn receive<bool>(i32 tag, i32 source) -> Packet<bool> {
    MPI_Status status;
    struct { i32 timestamp; bool item; } data{};
    MPI_Recv(&data, 2, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

    return {
      timestamp::resolve_conflict(data.timestamp),
      status.MPI_SOURCE,
      status.MPI_TAG,
      data.item,
    };
  }

  template<>
  fn receive<vector<i32>>(i32 tag, i32 source) -> Packet<vector<i32>> {
    MPI_Status status;

    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);

    i32 count;
    MPI_Get_count(&status, MPI_INT, &count);

    var data = (i32 *) malloc(count);
    MPI_Recv(data, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);


    var items = vector<i32>(count - 1);
    for (var i = 0; i < count - 1; ++i) items[i] = data[i + 1];

    return {
      timestamp::resolve_conflict(data[0]),
      status.MPI_SOURCE,
      status.MPI_TAG,
      move(items),
    };
  }

  template<>
  fn receive<vector<bool>>(i32 tag, i32 source) -> Packet<vector<bool>> {
    MPI_Status status;

    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);

    i32 count;
    MPI_Get_count(&status, MPI_INT, &count);

    var data = (i32 *) malloc(count);
    MPI_Recv(data, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);


    var items = vector<bool>(count - 1);
    for (var i = 0; i < count - 1; ++i) items[i] = data[i + 1];

    return {
      timestamp::resolve_conflict(data[0]),
      status.MPI_SOURCE,
      status.MPI_TAG,
      move(items),
    };
  }

  template<>
  fn receive<vector<tuple<i32, i32>>>(i32 tag, i32 source) -> Packet<vector<tuple<i32, i32>>> {
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);

    i32 count;
    MPI_Get_count(&status, MPI_INT, &count);

    var data = (i32 *) malloc(count);
    MPI_Recv(data, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

    var items = vector<tuple<i32, i32>>((count - 1) / 2);
    for (var i = 0; i < (count - 1) / 2; ++i) {
      items[i] = std::make_tuple(data[i + 1], data[i + 2]);
    }

    return {
      timestamp::resolve_conflict(data[0]),
      status.MPI_SOURCE,
      status.MPI_TAG,
      move(items),
    };
  }
}
