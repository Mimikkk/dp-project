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
    var saved_volunteer = 0;
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
          if (packet.source = saved_volunteer) {
          }
        }
          break;
        case action::ResponseRoomServiced: {
          queue.emplace_back(packet.timestamp, packet.source);
          std::sort(std::begin(queue), std::end(queue), [&](var first, var second) {
            return get<0>(first) - get<0>(second);
          });
        }
          break;

      }
    }
  }
}
