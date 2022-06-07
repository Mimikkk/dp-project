#pragma once
#include "../../imports.hpp"

namespace volunteer::action {
  enum Action {
    RequestService = 2001,
    ResponseServiceStart = 2003,
    ResponseServiceEnd = 2004,
  };
}
