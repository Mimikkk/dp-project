#pragma once
#include "../../imports.hpp"

namespace volunteer::action {
  enum Action {
    RequestService = 2001,
    ResponseServiceStart = 2003,
    ResponseServiceEnd = 2004,
    ResponseServiceAccept = 2005,
    ResponseServiceDeny = 2006
  };
}
