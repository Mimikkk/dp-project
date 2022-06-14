#pragma once
#include "../../imports.hpp"

template<typename T>
class Resource {
private:
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
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
    lock();
    task(&_value);
    unlock();
  }

  fn lock() {
    pthread_mutex_lock(&mutex);
  }
  fn unlock() {
    pthread_mutex_unlock(&mutex);
  }
  fn wait() {
    lock();
    pthread_cond_wait(&cond, &mutex);
    unlock();
  }
  fn signal() {
    lock();
    pthread_cond_signal(&cond);
    unlock();
  }
  fn set(T value) {
    _value = value;
  }
};
