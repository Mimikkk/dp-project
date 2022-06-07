#pragma once
#include "../../imports.hpp"

namespace poet::action {
  enum Action {
    RequestRoomService = 1000,
    RequestInvite = 1001,
    ResponseInvite = 1002,
    ResponseMembersList = 1003,
    ResponseDecisionsList = 1004,
    ResponsePartyStart = 1005,
  };
}
