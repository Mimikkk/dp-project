#pragma once
#include <utility>

#include "../../imports.hpp"

namespace packet {
  template<typename T = void *>
  struct Packet {
    i32 timestamp{};
    i32 source{};
    i32 tag{};
    T data{};
  };

  template<typename T= void *>
  extern fn send(i32 destination, i32 tag, T item = nullptr) -> void;

  template<typename T= void *>
  extern fn receive(i32 tag = MPI_ANY_TAG, i32 source = MPI_ANY_SOURCE) -> Packet<T>;
}
