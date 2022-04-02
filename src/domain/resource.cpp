#include <domain/resource.hpp>
#include "shared/utils/random.hpp"

using namespace resource;

extern Resource resource::random() {
  static var uniform = rnd::create_i_uniform<u8>(Resource::Alcohol, Resource::Nothing);
  return static_cast<Resource>(rnd::use(uniform));
}
