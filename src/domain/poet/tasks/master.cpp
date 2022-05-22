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
    let create_club_distribution = rnd::create_b_uniform(0.5);
    let join_invite_distribution = rnd::create_b_uniform(0.5);
    let item_distribution = rnd::create_i_uniform(0, 2);
    let pick_item_distribution = rnd::create_b_uniform(0.5);
    optional<poet::item::Item> previous_item;

    var members = vector<i32>();
    var decisions = vector<i32>({false, false, true});

    fn should_create_club = [&]() {
      return rnd::use(create_club_distribution);
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
    
    // TODO - send list mechanism
    fn send_members_list = [&](var poet) {};
    fn send_decisions_list = [&](var poet) {};
    fn await_members_list = [&]() {};
    fn await_decisions_list = [&]() {};

    fn await_invitation = [&](var decision) {
      let packet = packet::receive(action::RequestInvite);
      packet::send(action::ResponseInvite, packet.source, packet::Packet(decision));
    };
    fn await_party_start = [&](){
      return packet::receive(action::ResponsePartyStart).data;
    };

    fn pick_item = [&]() {
      loop {
        let item = rnd::use(item_distribution);
        if (not decisions[item] and rnd::use(pick_item_distribution)) {
          return static_cast<item::Item>(item);
        }
        if (rnd::use(pick_item_distribution)) {
          return static_cast<item::Item>(item);
        }
      }
    };

    fn request_room_service = [&]() {
      packet::send(action::RequestRoomService, process::random_volunteer());
    };
    fn await_room_service = [&]() {
      packet::receive(volunteer::action::ResponseRoomServiced);
    };

    loop {
      if (state::get() == state::Member) {
        await_room_service();
        process::sleep(rnd::use(sleep_distribution));
        state::change(state::Idle);
      }

      if (should_create_club()) {
        invite_poets();
        state::change(state::Member);
        await_invited_poets();
        previous_item = pick_item();
        decisions[previous_item] = true;

        let second_member = members[1];
        let last_member = members.back();
        send_members_list(second_member);
        send_decisions_list(second_member);

        decisions = std::move(await_decisions_list().list);

        if (is_party_doable()) {
          inform_members_about_party();
          request_room_service();
          inform_members_about_room_service();
        } else {
          inform_members_about_party_cancelation();
          state::change(state::Idle);
        }
      } else {
        let join_decision = rnd::use(join_invite_distribution);
        await_invitation(join_decision);
        if (!join_decision) continue;
        state::change(state::Member);

        let packet = await_members_list();
        let previous_member = packet.source;

        members = std::move(packet.list);
        decisions = std::move(await_decisions_list().list);
        previous_item = pick_item();
        decisions[previous_item] = true;

        let next_member = std::find(std::begin(members), std::end(members), process::Rank) - std::begin(members) + 1;
        send_members_list(next_member);
        send_decisions_list(next_member);
        
        if (not await_party_start()) {
          state::change(state::Idle);
        } 
      }
    }

  }
}
