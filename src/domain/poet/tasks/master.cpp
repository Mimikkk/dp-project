#include "master.hpp"
#include "../../definitions/packet.hpp"
#include "../action.hpp"
#include "../../process.hpp"
#include "../../volunteer/action.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"
#include "../item.hpp"

namespace poet {
  fn master_task() -> void {
    let sleep_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));
    optional<poet::item::Item> previous_item;

    var members = vector<i32>();
    var has_drinks = false;
    var has_food = false;

    fn should_create_club = [&]() {
      return rnd::boolean();
    };
    fn invite_poets = [&]() {
      process::foreach_poet_except_me(
        [&](var poet) {
          packet::send(action::RequestInvite, poet);
        }
      );
    };
    fn await_invited_poets = [&]() {
      var new_members = vector<i32>({process::Rank});
      process::foreach_poet_except_me(
        [&](var) {
          let packet = packet::receive(action::ResponseInvite);
          if (packet.data) new_members.push_back(packet.source);
        }
      );
      members = move(new_members);
    };
    fn request_room_service = [&]() {
      packet::send(action::RequestRoomService, process::random_volunteer());

      packet::receive(volunteer::action::ResponseRoomServiced);

      process::sleep(rnd::use(sleep_distribution));
    };

    loop {
      if (should_create_club()) {
        invite_poets();
        state::change(state::ClubOwner);
        await_invited_poets();
      } else {
      }

      request_room_service();
    }

  }
}
