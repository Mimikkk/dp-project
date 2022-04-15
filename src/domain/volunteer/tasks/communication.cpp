#include "communication.hpp"
#include "../../../utils/console.hpp"
#include "../../poet/action.hpp"
#include "../action.hpp"
#include "../state.hpp"

namespace volunteer {
  fn communication_task(void *pointer) -> void * {
    using namespace process;
    using namespace packet;

    static let uniform = rnd::create_i_uniform<u8>(0, 255);
    static let sleeps = rnd::create_f_uniform<f64>(1, 8);
    i32 poet;

    loop {
      switch (state::get()) {
        case state::Finish:
          console::event("Kończę prace naura");
          return pointer;
        case state::CleaningRoom:
          console::event("To ja będę sprzątał");
          send(volunteer::action::CleanRoom, poet);
          process::sleep(rnd::use(sleeps));

          state::change(state::Idle);
          break;
        case state::Idle:
          console::info("Oczekuję informacji o sprzątaniu...");
          var packet = receive();
          console::info("Otrzymałem informację o sprzątaniu!");

          switch (packet.tag) {
            case volunteer::action::CleanRoom:
              poet = packet.data;
              state::change(state::CleaningRoom);
              break;
            case volunteer::action::PickANumber:
              console::log("Losuję liczbe...");
              send(action::PickANumber, packet.source, Packet(rnd::use(uniform)));
              break;
            case poet::action::RequestRoomService:
              poet = packet.source;
              console::log("Poeta %d poprosił o serwis pokoju!", poet);

              Packet straw;
              for (var i = 0; i < Volunteers; ++i) {
                if (i + Poets == Rank)
                  send(volunteer::action::PickANumber, Rank, Packet(rnd::use(uniform)));
                send(volunteer::action::PickANumber, i + Poets);
              }

              for (var i = 0; i < Volunteers; ++i) {
                packet = receive(volunteer::action::PickANumber);

                console::log("%03d Wylosował %03d", packet.source, packet.data);
                if (packet.data < straw.data) straw = packet;
              }

              console::log("Najkrótszy kijek Wylosował %03d o wartości %03d", straw.source, straw.data);
              send(volunteer::action::CleanRoom, straw.source, Packet(poet));
              break;
          }
      }
    }
  }
}
