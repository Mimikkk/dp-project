#pragma once
#include <shared/imports.hpp>
#include <domain/resource.hpp>

namespace poet {
    using resource::Resource;

    inline i32 Count;
    inline optional<Resource> LastResource;


    inline fn pick_resource() -> Resource {
      Resource resource;
      while ((resource = resource::random()) == LastResource);
      LastResource = resource;
      return resource;
    }
}
