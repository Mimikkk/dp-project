#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../../utils/random.hpp"
#include "../../definitions/timestamp.hpp"

namespace volunteer {
  [[noreturn]] fn master_task() -> void {
    let cleaning_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));

    vector<i32> rooms;
    timestamp::set(process::Rank);
    vector<tuple<i32, i32>> queue;
    console::log("start queue: %s", str(queue).get());
    for (var i = 0; i < process::Volunteers; ++i) queue.emplace_back(i, i);
    console::log("start queue: %s", str(queue).get());
    
    fn clean = [&]() {};
    fn remove_volunteer = [&](var volunteer) {};

    let MaxRejections = 5;
    var reject_count = 0;
    optional<i32> saved_volunteer;

    fn inform_volunteer_about_service_need = [&](var volunteer, var room) {
      packet::send(volunteer, action::RequestService, room);
    };

    fn inform_volunteers_about_service_start = [&]() {
      process::foreach_volunteer(
        [&](var volunteer) {
          packet::send(volunteer, action::ResponseServiceStart);
        }
      );
    };

    fn inform_volunteers_about_service_end = [&]() {
      process::foreach_volunteer(
        [&](var volunteer) {
          packet::send(volunteer, action::ResponseServiceEnd);
        }
      );
    };

    fn handshake = [&]() -> bool {
      console::info("Queue %s", str(queue).get());
      console::info("saved volunteer is %s",
                    saved_volunteer.has_value()
                    ? str("%d", saved_volunteer.value()).get()
                    : "none");

      if (saved_volunteer.has_value()) {
        saved_volunteer.reset();
        ++reject_count;
        if (reject_count > MaxRejections) return true;
      } else reject_count = 0;
      
      let [volunteer, _] = queue.front();
      saved_volunteer.emplace(volunteer);
      let room = rooms.front();
      inform_volunteer_about_service_need(volunteer, room);
      return false;
    };

    loop {
      console::info("Awaiting service need...");
      let packet = packet::receive();
      console::info("Tag is %d", packet.tag);

      switch (packet.tag) {
        case poet::action::RequestRoomService: {
          console::event("Poet %d requested room service", packet.source);
          rooms.emplace_back(packet.source);
          if (rooms.size() == 1) handshake();
        }
          break;
        case action::ResponseServiceStart: {
          console::event("%d started a room service", packet.source);
          remove_volunteer(packet.source);
          if (packet.source == saved_volunteer) {
            if (rooms.front() == packet.source) {
              rooms.erase(begin(rooms));
              saved_volunteer.reset();
            }
            if (!rooms.empty()) handshake();
          }
        }
          break;
        case action::ResponseServiceEnd: {
          console::event("%d finished a room service", packet.source);
          queue.emplace_back(packet.timestamp, packet.source);
          std::sort(std::begin(queue), std::end(queue), [&](var first, var second) {
            let [first_timestamp, first_source] = first;
            let [second_timestamp, second_source] = second;
            return first_timestamp - second_timestamp;
          });
        }
          break;
        case volunteer::action::RequestService: {
          console::event("I've been requested to service the room of %d", packet.source);
          inform_volunteers_about_service_start();

          clean();
          if (reject_count > MaxRejections) {
            rooms.erase(begin(rooms));
            clean();
            if (!rooms.empty()) handshake();
          }

          inform_volunteers_about_service_end();
        }
          break;
      }
    }
  }
}
