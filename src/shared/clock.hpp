#pragma once
#include <shared/imports.hpp>
#include <functional>

namespace sharedtime {
    typedef u64 timestamp;

    namespace {
        inline void handle_overflow() {}
        inline timestamp stamp = 0;
        
        inline timestamp tick() {
          if (++stamp == 0) handle_overflow();
          return stamp;
        }
    }

    inline timestamp get() {
        return stamp;
    }
    
    inline void run(const std::function<void(timestamp)> &command) {
      command(tick());
    }
}
