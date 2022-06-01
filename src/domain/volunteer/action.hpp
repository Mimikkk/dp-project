#pragma once
#include "../../imports.hpp"

namespace volunteer::action {
  enum Action {
    ResponseRoomServiced = 0x2000,
    RequestRoomService = 0x2001,
    ResponseRoomInService = 0x2002,
  };
}
