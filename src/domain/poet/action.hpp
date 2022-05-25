#pragma once
#include "../../imports.hpp"

namespace poet::action {
  enum Action {
    RequestRoomService = 0x1000,
    RequestInvite = 0x1001,
    ResponseInvite = 0x1002,
    ResponseMembersList = 0x1003,
    ResponseDecisionsList = 0x1004,
    ResponsePartyStart = 0x1005,
    ResponseHadRoomService = 0x1006,
  };
}
