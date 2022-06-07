#include "master.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"
#include "../../../utils/random.hpp"
#include "../../definitions/timestamp.hpp"

[[noreturn]] fn volunteer::master_task() -> void {
  let cleaning_distribution = rnd::create_f_uniform(rnd::real(0.5, 1.0), rnd::real(1.5, 2.5));

  vector<i32> rooms;
  timestamp::set(process::Rank);
  vector<tuple<i32, i32>> queue;
  for (var i = 0; i < process::Volunteers; ++i) queue.emplace_back(i + process::Poets, i + process::Poets);
  console::log("start rooms: %s", str(rooms).get());
  console::log("start queue: %s", str(queue).get());

  fn service_room = [&]() {
    process::sleep(rnd::use(cleaning_distribution));
  };
  fn remove_volunteer = [&](let volunteer) {
    console::log("Usuwanie %d wolontariusza  z kolejki %s", volunteer, str(queue).get());
    queue.erase(std::find_if(std::begin(queue), std::end(queue), [&](var pair) {
      let [_, pair_volunteer] = pair;
      return pair_volunteer == volunteer;
    }));

  };

  let MaxRejections = 5;
  var reject_count = 0;
  optional<i32> saved_volunteer;

  fn inform_poet_about_service_end = [&](let poet) {
    packet::send(poet, action::ResponseServiceEnd);
  };
  fn inform_volunteer_about_service_need = [&](let volunteer, let room) {
    packet::send(volunteer, volunteer::action::RequestService, room);
  };
  fn inform_volunteers_about_service_start = [&]() {
    process::foreach_volunteer(
      [&](var volunteer) {
        packet::send(volunteer, action::ResponseServiceStart);
      }
    );
  };
  fn inform_volunteers_about_service_end = [&](let poet) {
    process::foreach_volunteer(
      [&](var volunteer) {
        packet::send(volunteer, action::ResponseServiceEnd, poet);
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

  loop {
    console::info("Oczekuję na informacje...");
    let packet = packet::receive<i32>();

    switch (packet.tag) {
      case poet::action::RequestRoomService: {
        console::event("Poeta %d poprosił o posprzątanie", packet.source);

        rooms.emplace_back(packet.source);
        console::info("Pokoje do posprzątania %s", str(rooms).get());
        if (rooms.size() == 1) handshake();
      }
        break;
      case volunteer::action::RequestService: {
        var poet = packet.data;
        console::event("Zostałem poproszony o sprzątanie pokoju poety %d", poet);

        console::info("Informuję o rozpoczęciu sprzątania...");
        inform_volunteers_about_service_start();

        console::info("Sprzątam...");
        service_room();

        console::info("Informuję o zakończeniu sprzątania...");
        inform_volunteers_about_service_end(poet);
        console::info("Informuję poetę o zakończonym sprzątaniu...");
        inform_poet_about_service_end(poet);

        if (reject_count > MaxRejections) {
          console::info("Przekroczyłem limit odmówień...");
          console::error("Sam sprzątam!");

          poet = rooms.front();
          rooms.erase(begin(rooms));

          console::info("Sprzątam...");
          service_room();

          console::info("Informuję o zakończeniu sprzątania...");
          inform_volunteers_about_service_end(poet);
          console::info("Informuję poetę o zakończonym sprzątaniu...");
          inform_poet_about_service_end(poet);
          console::info("Pokoje do posprzątania %s", str(rooms).get());
          if (not rooms.empty()) handshake();
        }
      }
        break;
      case action::ResponseServiceStart: {
        console::event("%d rozpoczął sprzątanie", packet.source);
        remove_volunteer(packet.source);

        if (packet.source == saved_volunteer) {
          console::info("Osoba zapisana sprząta");

          if (rooms.front() == packet.data) {
            console::info("To był pokój poety");
            rooms.erase(begin(rooms));
            saved_volunteer.reset();
          }

          console::info("Pokoje do posprzątania %s", str(rooms).get());
          if (not rooms.empty()) handshake();
        }
      }
        break;
      case action::ResponseServiceEnd: {
        console::event("%d zakończył sprzątanie", packet.source);

        queue.emplace_back(packet.timestamp, packet.source);
        console::event("%d zakończył sprzątanie", packet.source);

        console::info("Sortowanie kolejki...");
        std::sort(std::begin(queue), std::end(queue), [&](var first, var second) {
          let [first_timestamp, first_source] = first;
          let [second_timestamp, second_source] = second;
          return first_timestamp < second_timestamp;
        });
      }
        break;
    }
  }
}
