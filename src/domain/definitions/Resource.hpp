#pragma once
#include "../../imports.hpp"

template<typename T>
class Resource {
private:
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  T _value;

public:
  explicit Resource() = default;
  explicit Resource(T value) {
    _value = value;
  }
  ~Resource() {
    pthread_mutex_destroy(&mutex);
  }

  fn value() { return _value; }
  fn update(fn task) {
    pthread_mutex_lock(&mutex);
    task(&_value);
    pthread_mutex_unlock(&mutex);
  }
};
