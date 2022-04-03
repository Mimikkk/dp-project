#pragma once
#include <shared/imports.hpp>
#include <domain/resource.hpp>
#include "shared/utils/random.hpp"

namespace poet {
  using resource::Resource;

  inline i32 Count;
  inline i32 Cooldown;
  inline optional<Resource> LastResource;


  inline fn rest() {
    Cooldown = rnd::integer(0, 20);
  }

  inline fn logic() {
  }

  inline fn pick_resource() -> Resource {
    Resource resource;
    while ((resource = resource::random()) == LastResource);
    LastResource = resource;
    return resource;
  }
}
