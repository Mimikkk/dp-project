#include "master.hpp"
#include "../../definitions/packet.hpp"
#include "../action.hpp"
#include "../../process.hpp"
#include "../../volunteer/action.hpp"
#include "../../../utils/console.hpp"
#include "../state.hpp"
#include "../item.hpp"

namespace poet {
  [[noreturn]] fn master_task() -> void {
    let sleep_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));
    let party_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));
    let create_club_distribution = rnd::create_b_uniform(0.5);
    let join_invite_distribution = rnd::create_b_uniform(0.5);
    let item_distribution = rnd::create_i_uniform(0, 2);
    let item_leak_distribution = rnd::create_b_uniform(0.5);
    let pick_item_distribution = rnd::create_b_uniform(0.5);
    optional<poet::item::Item> previous_item;

    var members = vector<i32>();
    fn find_next_member = [&]() {
      return (i32) (std::find(std::begin(members), std::end(members), process::Rank) - std::begin(members) + 1);
    };

    var decisions = vector<bool>({false, false, true});
    fn are_drinks_and_food_present = [&]() {
      return std::all_of(std::begin(decisions), std::end(decisions), [](auto d) { return d; });
    };

    fn reset_state = [&]() {
      members.clear();
      decisions = vector<bool>({false, false, true});
      state::change(state::Idle);
    };

    fn should_create_club = [&]() {
      return rnd::use(create_club_distribution);
    };
    fn invite_poets = [&]() {
      process::foreach_poet_except_me(
        [&](var poet) {
          packet::send(poet, action::RequestInvite);
        }
      );
    };
    fn await_invited_poets = [&]() {
      var new_members = vector<i32>({process::Rank});
      process::foreach_poet_except_me(
        [&](var) {
          let packet = packet::receive<i32>(action::ResponseInvite);
          if (packet.data) new_members.push_back(packet.source);
        }
      );
      members = std::move(new_members);
    };

    fn inform_members_about_room_service = [&]() {
      process::foreach_poet_except_me(
        [&](var poet) {
          packet::send(poet, action::ResponseHadRoomService);
        }
      );
    };
    fn inform_members_about_party = [&](bool shouldStart) {
      process::foreach_poet_except_me(
        [&](var poet) {
          packet::send(poet, action::ResponsePartyStart, shouldStart);
        }
      );
    };

    fn send_members_list = [&](var poet) {
      packet::send(poet, action::ResponseMembersList, members);
    };
    fn send_decisions_list = [&](var poet) {
      packet::send(poet, action::ResponseDecisionsList, decisions);
    };

    fn await_members_list = [&]() -> vector<i32> {
      return packet::receive<vector<i32>>(action::ResponseMembersList).data;
    };
    fn await_decisions_list = [&]() -> vector<bool> {
      return packet::receive<vector<bool>>(action::ResponseDecisionsList).data;
    };

    fn await_invitation = [&]() -> bool {
      let packet = packet::receive(action::RequestInvite);
      let decision = rnd::use(join_invite_distribution);
      packet::send(packet.source, action::ResponseInvite, decision);
      return decision;
    };
    fn await_party_start = [&]() -> bool {
      return packet::receive<bool>(action::ResponsePartyStart).data;
    };

    fn pick_item = [&]() {
      i32 item;
      loop {
        do {
          item = rnd::use(item_distribution);
        } while (previous_item.has_value() and item == previous_item.value());

        if (not decisions[item] or are_drinks_and_food_present()) {
          return static_cast<item::Item>(item);
        }

        if (rnd::use(item_leak_distribution)) {
          return static_cast<item::Item>(item);
        }
      }
    };

    fn request_room_service = [&]() {
      packet::send(process::random_volunteer(), action::RequestRoomService);
    };
    fn await_room_service = [&]() {
      packet::receive(volunteer::action::ResponseRoomServiced);
    };

    loop {
      if (state::get() == state::Member) {
        await_room_service();
        process::sleep(rnd::use(party_distribution));
        reset_state();
      }

      if (should_create_club()) {
        state::change(state::Member);
        invite_poets();
        await_invited_poets();

        previous_item = pick_item();
        decisions[previous_item.value()] = true;
        let next_member = find_next_member();
        send_members_list(next_member);
        send_decisions_list(next_member);

        decisions = std::move(await_decisions_list());

        if (are_drinks_and_food_present()) {
          inform_members_about_party(true);
          process::sleep(rnd::use(sleep_distribution));

          request_room_service();
          inform_members_about_room_service();
        } else {
          inform_members_about_party(false);
          reset_state();
        }
      } else {
        let has_joined = await_invitation();
        if (!has_joined) continue;

        state::change(state::Member);

        members = std::move(await_members_list());
        decisions = std::move(await_decisions_list());
        previous_item = pick_item();
        decisions[previous_item.value()] = true;

        let next_member = find_next_member();
        send_members_list(next_member);
        send_decisions_list(next_member);

        if (not await_party_start()) reset_state();
        else process::sleep(rnd::use(party_distribution));
      }
    }
  }
}
