#pragma once
#include <utility>

#include "../../imports.hpp"

namespace packet {
  extern fn timestamp() -> i32;

  template<typename T = void *>
  struct Packet {
    i32 timestamp{};
    i32 source{};
    i32 tag{};
    T data{};
  };

  template<typename T>
  extern fn send(i32 destination, i32 tag, T item = nullptr) -> void;

  template<typename T>
  extern fn receive(i32 tag = MPI_ANY_TAG, i32 source = MPI_ANY_SOURCE) -> Packet<T>;
}
