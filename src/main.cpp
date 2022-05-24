#include <memory>

#include "imports.hpp"
#include "domain/poet/tasks/communication.hpp"
#include "domain/poet/tasks/master.hpp"
#include "domain/volunteer/tasks/communication.hpp"
#include "domain/volunteer/tasks/master.hpp"
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

  if (process::Rank == 0) {
    console::log("nullptr: %d", nullptr);

    console::log("Sending...");
    packet::send<i32>(0, 0, 1);
    packet::send<i32>(0, 0, 2);
    packet::send<i32>(0, 0, 3);
    packet::send<i32>(0, 0, 4);
    packet::send<bool>(0, 0, false);
    packet::send<bool>(0, 0, true);
    packet::send<vector<i32>>(0, 0, {1, 2, 3});
    packet::send<vector<tuple<i32, i32>>>(0, 0, {
      {1, 2},
      {3, 4}
    });
    packet::send<vector<i32>>(0, 0, {});
    packet::send<vector<tuple<i32, i32>>>(0, 0, {});
    console::log("Sent");

    console::log("Receiving...");
    {
      var packet = packet::receive<i32>(0, 0);
      console::log("Received %d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
    }
    {
      var packet = packet::receive<i32>(0, 0);
      console::log("Received %d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
    }
    {
      var packet = packet::receive<i32>(0, 0);
      console::log("Received %d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
    }
    {
      var packet = packet::receive<i32>(0, 0);
      console::log("Received %d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
    }
    {
      var packet = packet::receive<bool>(0, 0);
      console::log("Received %d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
    }
    {
      var packet = packet::receive<bool>(0, 0);
      console::log("Received %d %d %d %d", packet.timestamp, packet.source, packet.tag, packet.data);
    }
    {
      var packet = packet::receive<vector<i32>>(0, 0);
      console::log("Received %d %d %d %s", packet.timestamp, packet.source, packet.tag, str(packet.data).get());
    }
    {
      var packet = packet::receive<vector<tuple<i32,i32>>>(0, 0);
      console::log("Received %d %d %d %s", packet.timestamp, packet.source, packet.tag, str(packet.data).get());
    }
    {
      var packet = packet::receive<vector<i32>>(0, 0);
      console::log("Received %d %d %d %s", packet.timestamp, packet.source, packet.tag, str(packet.data).get());
    }
    {
      var packet = packet::receive<vector<tuple<i32,i32>>>(0, 0);
      console::log("Received %d %d %d %s", packet.timestamp, packet.source, packet.tag, str(packet.data).get());
    }
  }


  finalize();
}
