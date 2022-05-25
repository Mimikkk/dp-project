#include "imports.hpp"
#include "domain/process.hpp"
#include "domain/definitions/packet.hpp"
#include "utils/console.hpp"

fn main(i32 argc, char **argv) -> i32 {
  using namespace process;
  initialize(argc, argv);

//  if (is_poet(Rank)) {
//    using namespace poet;
//    pthread_create(&communication_thread, nullptr, communication_task, nullptr);
//    master_task();
//  } else {
//    using namespace volunteer;
//    pthread_create(&communication_thread, nullptr, communication_task, nullptr);
//    master_task();
//  }
//

  {
    packet::send(0, 0);
    let packet = packet::receive();
    console::log("%d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
  }
  {
    packet::send(0, 0, 1337);
    let packet = packet::receive<i32>();
    console::log("%d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
  }
  {
    packet::send(0, 0, false);
    let packet = packet::receive<bool>();
    console::log("%d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
  }
  {
    packet::send(0, 0, true);
    let packet = packet::receive<bool>();
    console::log("%d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
  }
  {
    packet::send<const vector<i32> &>(0, 0, {1, 2, 3});
    let packet = packet::receive<vector<i32>>();
    console::log("%d %d %d %s", packet.timestamp, packet.source, packet.tag, str(packet.data).get());
  }
  {
    packet::send<const vector<tuple<i32, i32>> &>(0, 0, {{1, 2}, {2, 3}, {3, 4}});
    let packet = packet::receive<vector<tuple<i32, i32>>>();
    console::log("%d %d %d %s", packet.timestamp, packet.source, packet.tag, str(packet.data).get());
  }

  finalize();
}
