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
    vector<tuple<i32, i32>> queue(process::Volunteers);
    for (var i = 0; i < process::Volunteers; ++i) queue.emplace_back(i, i);
    
    fn inform_others_about_cleaning = [&]() {};
    fn clean = [&]() {};
    fn remove_volunteer = [&](var volunteer) {};

    var is_free = false;
    let MaxRejections = 5;
    var reject_count = 0;
    optional<i32> saved_volunteer;
    
    fn inform_about_cleaning = [&](var volunteer, var room) {
      packet::send(volunteer, volunteer::action::RequestRoomCleaning, room);
    };
    fn process = [&]() {
      if (queue.size() == 1) {
        saved_volunteer.reset();
        ++reject_count;
      } else {
        let [volunteer, _] = queue.front()
        saved_volunteer.emplace(volunteer);
        reject_count = 0;
        let room = rooms.front()
        inform_about_cleaning(volunteer, room);
      }
    };
    
    loop {
      let packet = packet::receive();

      switch (packet.tag) {
        case poet::action::RequestRoomService: {
          rooms.emplace_back(packet.source);  
          if (rooms.size() == 1) {
            // TODO
          }
        }
          break;
        case volunteer::action::RequestRoomService: {
          if (is_free) {
            inform_others_about_cleaning();
            clean();
          }
        }
          break;
        case action::ResponseRoomInService: {
          remove_volunteer(packet.source);
          if (packet.source == saved_volunteer) {
          }
        }
          break;
        case action::ResponseRoomServiced: {
          queue.emplace_back(packet.timestamp, packet.source);
          std::sort(std::begin(queue), std::end(queue), [&](var first, var second) {
            let [first_timestamp, first_source] = first;
            let [second_timestamp, second_source] = second;
            return first_timestamp - second_timestamp;
          });
        }
          break;

      }
    }
  }
}
