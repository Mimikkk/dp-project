#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../definitions/timestamp.hpp"
#include "../../definitions/packet.hpp"
#include "../../process.hpp"

[[noreturn]] fn volunteer::master_task() -> void {
  vector<i32> rooms;
  timestamp::set(process::Rank);
  vector<tuple<i32, i32>> queue;
  for (var i = 0; i < process::Volunteers; ++i) queue.emplace_back(i + process::Poets, i + process::Poets);

  let MaxRejections = 5;
  var reject_count = 0;
  optional<i32> saved_volunteer;
  i32 requsting_poet;

  fn service_room = [&]() {
    state::change(state::Servicing);
    state::raw()->signal();
  };

  fn remove_volunteer = [&](let volunteer) {
    console::info("Usuwanie %d wolontariusza  z kolejki %s", volunteer, str(queue).get());
    queue.erase(std::find_if(std::begin(queue), std::end(queue), [&](var pair) {
      let [_, pair_volunteer] = pair;
      return pair_volunteer == volunteer;
    }));

  };

  fn inform_poet_about_service_end = [&](let poet) {
    packet::send(poet, action::ResponseServiceEnd);
  };

  fn inform_volunteer_about_service_need = [&](let volunteer, let room) {
    packet::send(volunteer, action::RequestService, room);
  };

  fn inform_volunteers_about_service_start = [&]() {
    process::foreach_volunteer(
      [&](var volunteer) {
        packet::send(volunteer, action::ResponseServiceStart);
      }
    );
  };

  fn inform_volunteers_about_service_end = [&](let timestamp) {
    process::foreach_volunteer_except_me(
      [&](var volunteer) {
        packet::send(volunteer, action::ResponseServiceEnd, timestamp);
      }
    );
  };

  fn handshake = [&]() -> bool {
    console::info("Kolejka %s", str(queue).get());
    console::info("Liczba odmówień %d", reject_count);
    console::info("Zapisany wolontariusz to %s",
                  saved_volunteer.has_value()
                  ? str("%d", saved_volunteer.value()).get()
                  : "żaden");

    if (saved_volunteer.has_value()) {
      saved_volunteer.reset();
      ++reject_count;
      if (reject_count > MaxRejections) return true;
    } else reject_count = 0;

    let [_, volunteer] = queue.front();
    console::info("Zapisuje wolontariusza z początku kolejki %d", volunteer);
    saved_volunteer.emplace(volunteer);
    let room = rooms.front();
    console::info("Informuje wolontariusza %d o potrzebie posprzątania", volunteer);
    inform_volunteer_about_service_need(volunteer, room);
    return false;
  };

  fn put_volunteer_back_in_queue = [&](let timestamp, let volunteer) {
    console::event("%d zakończył sprzątanie", volunteer);

    queue.emplace_back(timestamp, volunteer);

    console::info("Sortowanie kolejki...");
    std::sort(std::begin(queue), std::end(queue), [&](var first, var second) {
      let [first_timestamp, first_source] = first;
      let [second_timestamp, second_source] = second;
      return first_timestamp < second_timestamp;
    });
  };

  fn inform_volunteer_about_service_accept = [&](let volunteer) {
    packet::send(volunteer, action::ResponseServiceAccept);
  };

  fn inform_volunteer_about_service_deny = [&](let volunteer) {
    packet::send(volunteer, action::ResponseServiceDeny);
  };

  loop {
    console::info("Oczekuję na informacje...");
    let packet = packet::receive<i32>();

    switch (packet.tag) {
      case poet::action::RequestRoomService: {
        let poet = packet.source;
        console::event("Poeta %d poprosił o posprzątanie", poet);
        rooms.emplace_back(poet);

        console::info("Pokoje do posprzątania %s", str(rooms).get());
        if (rooms.size() == 1) handshake();
      }
        break;
      case action::RequestService: {
        let volunteer = packet.source;
        console::event("Zostałem poproszony o sprzątanie pokoju poety %d", volunteer);

        if (state::get() == state::Idle) {
          requsting_poet = packet.data;

          console::info("Informuję o rozpoczęciu sprzątania...");
          inform_volunteers_about_service_start();
          inform_volunteer_about_service_accept(volunteer);

          console::info("Sprzątam...");
          service_room();
        }
        else {
          console::info("Informuję o odmowie sprzątania...");
          inform_volunteer_about_service_deny(volunteer);
        }

      }
        break;
      case action::ResponseServiceStart: {
        let volunteer = packet.source;
        console::event("%d rozpoczął sprzątanie", volunteer);
        remove_volunteer(volunteer);
      }
        break;
      case action::ResponseServiceEnd: {
        let timestamp = packet.data;
        let volunteer = packet.source;

        if (process::is_me(volunteer)) {
          state::change(state::Idle);

          console::info("Informuję poetę o zakończonym sprzątaniu...");
          inform_poet_about_service_end(requsting_poet);

          if (reject_count > MaxRejections) {
            console::info("Przekroczyłem limit odmówień...");

            requsting_poet = rooms.front();
            rooms.erase(begin(rooms));

            console::info("Sprzątam...");
            service_room();

            console::info("Pokoje do posprzątania %s", str(rooms).get());
            if (not rooms.empty()) handshake();
          }
          else {
            console::info("Informuję o zakończeniu sprzątania...");
            inform_volunteers_about_service_end(timestamp);

            put_volunteer_back_in_queue(timestamp, volunteer);
          }
        }
        else {
          put_volunteer_back_in_queue(timestamp, volunteer);
        }
      }
        break;
      case action::ResponseServiceAccept: {
        console::info("Osoba zapisana akceptuje");
        rooms.erase(begin(rooms));
        saved_volunteer.reset();

        console::info("Pokoje do posprzątania %s", str(rooms).get());
        if (not rooms.empty()) handshake();
      }
        break;
      case action::ResponseServiceDeny: {
        console::info("Osoba zapisana odmawia");

        console::info("Pokoje do posprzątania %s", str(rooms).get());
        handshake();
      }
        break;
    }
  }
}
