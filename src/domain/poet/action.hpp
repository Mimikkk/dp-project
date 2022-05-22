#pragma once
#include "../../imports.hpp"

namespace poet::action {
  enum Action {
    RequestRoomService = 0x1000,
    RequestInvite = 0x1001,
    ResponseInvite = 0x1002
  };
}
