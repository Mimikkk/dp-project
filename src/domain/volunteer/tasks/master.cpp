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
  i32 requesting_poet;

  fn service_room = [&]() {
    state::change(state::Servicing);
    state::raw()->signal();
  };

  fn remove_volunteer = [&](let volunteer) {
    console::info("Usuwanie wolontariusza %d z kolejki %s", volunteer, str(queue).get());
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
    console::info("Kolejka to: %s", str(queue).get());
    console::info("Liczba odmówień to: %d", reject_count);
    console::info("Zapisany wolontariusz to: %s",
                  saved_volunteer.has_value()
                  ? str("%d", saved_volunteer.value()).get()
                  : "żaden");

    if (saved_volunteer.has_value()) {
      saved_volunteer.reset();
      ++reject_count;
      if (reject_count > MaxRejections) return true;
    } else reject_count = 0;

    if (queue.empty()) {
      console::info("Wszyscy wolontariusze są zajęci");
      return true;
    }

    let [_, volunteer] = queue.front();
    console::info("Zapisuje wolontariusza %d z początku kolejki", volunteer);
    saved_volunteer.emplace(volunteer);
    let room = rooms.front();
    console::info("Informuje wolontariusza %d o potrzebie posprzątania", volunteer);
    inform_volunteer_about_service_need(volunteer, room);
    return false;
  };

  fn put_volunteer_back_in_queue = [&](let timestamp, let volunteer) {
    console::event("Wolontariusz %d zakończył sprzątanie", volunteer);

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

        console::info("Pokoje do posprzątania to: %s", str(rooms).get());
        if (rooms.size() == 1) handshake();
      }
        break;
      case action::RequestService: {
        let volunteer = packet.source;
        console::event("Wolontariusz %d poprosił o posprzątanie", volunteer);

        if (state::get() == state::Idle) {
          requesting_poet = packet.data;

          console::info("Informuję wolontariuszy o rozpoczęciu sprzątania...");
          inform_volunteers_about_service_start();

          console::info("Informuję wolontariusza %d o akceptacji sprzątania...", volunteer);
          inform_volunteer_about_service_accept(volunteer);

          console::info("Sprzątam...");
          service_room();
        }
        else {
          console::info("Informuję wolontariusza %d o odmowie sprzątania...", volunteer);
          inform_volunteer_about_service_deny(volunteer);
        }

      }
        break;
      case action::ResponseServiceStart: {
        let volunteer = packet.source;
        console::event("Wolontariusz %d rozpoczął sprzątanie", volunteer);
        remove_volunteer(volunteer);
      }
        break;
      case action::ResponseServiceEnd: {
        let timestamp = packet.data;
        let volunteer = packet.source;
        console::event("Zakończenie serwisu przez %d", volunteer);

        if (process::is_me(volunteer)) {
          state::change(state::Idle);

          console::info("Informuję poetę %d o zakończonym sprzątaniu...", requesting_poet);
          inform_poet_about_service_end(requesting_poet);

          if (reject_count > MaxRejections) {
            console::info("Przekroczyłem limit odmówień...");
            reject_count = 0;

            requesting_poet = rooms.front();
            rooms.erase(begin(rooms));

            console::info("Sprzątam...");
            service_room();

            console::info("Pokoje do posprzątania to: %s", str(rooms).get());
            if (not rooms.empty()) handshake();
          }
          else {
            console::info("Informuję resztę wolontariuszy o zakończeniu sprzątania...");
            inform_volunteers_about_service_end(timestamp);

            put_volunteer_back_in_queue(timestamp, volunteer);
            if (queue.size() == 1 and !rooms.empty()) handshake();
          }
        }
        else {
          put_volunteer_back_in_queue(timestamp, volunteer);
          if (queue.size() == 1 and !rooms.empty() and reject_count <= MaxRejections and !saved_volunteer.has_value()) handshake();
        }
      }
        break;
      case action::ResponseServiceAccept: {
        console::event("Zapisany wolontariusz zaakceptował");
        rooms.erase(begin(rooms));
        saved_volunteer.reset();

        console::info("Pokoje do posprzątania to: %s", str(rooms).get());
        if (not rooms.empty()) handshake();
      }
        break;
      case action::ResponseServiceDeny: {
        console::event("Zapisany wolontariusz odmówił");

        console::info("Pokoje do posprzątania to: %s", str(rooms).get());
        handshake();
      }
        break;
    }
  }
}
