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
    let create_club_distribution = rnd::create_b_uniform(0.3);
    let item_distribution = rnd::create_i_uniform(0, 2);
    let item_leak_distribution = rnd::create_b_uniform(0.5);
    let pick_item_distribution = rnd::create_b_uniform(0.5);
    optional<poet::item::Item> previous_item;

    var members = vector<i32>();
    fn find_next_member = [&]() {
      return members[
        (
          std::find(std::begin(members), std::end(members), process::Rank)
          - std::begin(members) + 1
        ) % members.size()
      ];
    };


    var decisions = vector<i32>({0, 0, 1});
    fn are_drinks_and_food_present = [&]() {
      return std::all_of(std::begin(decisions), std::end(decisions), [](auto d) { return d; });
    };

    fn reset_state = [&]() {
      members.clear();
      decisions = vector<i32>({0, 0, 1});
      state::change(state::Idle);
    };

    fn should_create_club = [&]() {
      return state::get() != state::Member and rnd::use(create_club_distribution);
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
      for (let member: members) {
        if (process::is_me(member)) continue;
        packet::send(member, volunteer::action::ResponseServiceEnd);
      }
    };
    fn inform_members_about_party = [&](bool shouldStart) {
      for (let member: members) {
        if (process::is_me(member)) continue;
        packet::send(member, action::ResponsePartyStart, (i32) shouldStart);
      }
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
    fn await_decisions_list = [&]() -> vector<i32> {
      return packet::receive<vector<i32>>(action::ResponseDecisionsList).data;
    };

    fn await_party_start = [&]() -> bool {
      return packet::receive<i32>(action::ResponsePartyStart).data;
    };

    fn pick_item = [&]() {
      i32 option;
      var options = vector<i32>();
      bool d0 = decisions[0];
      bool d1 = decisions[1];
      bool p0 = previous_item == 0 || previous_item == 2;
      bool p1 = previous_item == 1 || previous_item == 2;

      if (!d0 && !p0 || !d0 && p1 || d1 && !p0 || d1 && p1) { options.push_back(0); }
      if (d0 && p0 || d0 && !p1 || !d1 && p0 || !d1 && !p1) { options.push_back(1); }
      if (d1 && p0 && !p1 || d0 && !p0 && p1 || d0 && d1 && !p0) { options.push_back(2); }

      option = rnd::integer(0, -1 + static_cast<i32>(options.size()));
      return static_cast<item::Item>(options[option]);
    };

    fn request_room_service = [&]() {
      packet::send(process::random_volunteer(), action::RequestRoomService);
    };
    fn await_room_service = [&]() {
      packet::receive(volunteer::action::ResponseServiceEnd);
    };
    loop {
      process::sleep(0.5);

      var resource = state::raw();
      resource->lock();
      if (should_create_club()) {
        console::info("Tworzę klub...");
        console::info("Odrzucam każdą propozycję");

        resource->set(state::Member);
        resource->unlock();

        console::info("Zapraszam innych...");
        invite_poets();

        console::event("Oczekuję ich odpowiedzi...");
        await_invited_poets();
        console::info("Członkowie to: %s", str(members).get());

        if (members.size() < 2) {
          console::info("Impreza jest niemożliwa");
          console::event("Informuję resztę o rozwiązaniu koła...");
          inform_members_about_party(false);
          console::info("Odchodzę z koła");
          reset_state();
          continue;
        }

        let item = pick_item();
        decisions[item] = 1;

        console::info("Wybrałem: %d", item);
        console::info("Decyzje to: %s", str(decisions).get());

        let next_member = find_next_member();
        console::info("Następna osoba to %d", next_member);

        if (next_member == members.front()) {
          console::event("Przesyłam listę decyzji do szefa %d...", next_member);
          send_decisions_list(next_member);
        } else {
          console::event("Przesyłam listę członków do %d...", next_member);
          send_members_list(next_member);
          console::event("Przesyłam listę decyzji do %d...", next_member);
          send_decisions_list(next_member);
        }

        console::event("Oczekuje na ostatnią listę decyzji %d...", next_member);
        decisions = std::move(await_decisions_list());

        if (are_drinks_and_food_present()) {
          console::info("Impreza jest możliwa");
          console::info("Informuję resztę o jej rozpoczęciu...");
          inform_members_about_party(true);
          process::sleep(rnd::use(sleep_distribution));
          previous_item = item;

          console::event("Proszę o sprzątanie pokoju...");
          request_room_service();
          console::event("Oczekuję posprzątania pokoju...");
          await_room_service();
          console::event("Informuję resztę członków o sprzątaniu...");
          inform_members_about_room_service();
          console::info("Odchodzę z koła");
          reset_state();
        } else {
          console::info("Impreza jest niemożliwa");
          console::info("Informuję resztę o rozwiązaniu koła...");
          inform_members_about_party(false);
          console::info("Odchodzę z koła");
          reset_state();
        }
      } else if (state::get() == state::Member) {
        resource->unlock();
        console::event("Oczekuję na listę członków...");
        members = std::move(await_members_list());
        console::event("Oczekuję na listę decyzji...");
        decisions = std::move(await_decisions_list());
        let item = pick_item();
        decisions[item] = 1;
        console::info("Wybrałem: %d", item);
        console::info("Decyzje to: %s", str(decisions).get());

        let next_member = find_next_member();
        if (next_member == members.front()) {
          console::event("Przesyłam listę decyzji do szefa %d...", next_member);
          send_decisions_list(next_member);
        } else {
          console::event("Przesyłam listę członków do %d...", next_member);
          send_members_list(next_member);
          console::event("Przesyłam listę decyzji do %d...", next_member);
          send_decisions_list(next_member);
        }

        let is_party_happening = await_party_start();
        if (not is_party_happening) {
          console::info("Nie ma imprezy");
          console::info("Odchodzę z koła...");
          reset_state();
        } else {
          console::info("Jest impreza");
          console::info("Imprezuję...");
          process::sleep(rnd::use(party_distribution));
          previous_item = item;
          console::event("Oczekuję posprzątania pokoju...");
          await_room_service();
          console::info("Odchodzę z koła");
          reset_state();
        }
      } else {
        resource->unlock();
      }
    }
  }
}
