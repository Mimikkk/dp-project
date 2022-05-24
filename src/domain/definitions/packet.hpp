#pragma once
#include <utility>

#include "../../imports.hpp"

namespace packet {
  extern fn timestamp() -> i32;

  template<typename T = void *>
  class Packet {
  public:
    i32 timestamp{};
    i32 source{};
    i32 tag{};
    T data{};
  };

  template<typename T = void *>
  extern fn send(i32 tag, i32 destination, T item = nullptr) -> void;

  template<typename T = void *>
  extern fn receive(i32 tag = MPI_ANY_TAG, i32 source = MPI_ANY_SOURCE, unique_ptr<MPI_Status> status = nullptr)
  -> Packet<T>;
}
