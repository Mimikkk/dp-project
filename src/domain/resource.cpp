#include <domain/resource.hpp>
#include <shared/utils/random.hpp>

using namespace resource;

extern fn resource::random() -> Resource {
  static var uniform = rnd::create_uniform(Resource::Alcohol, Resource::Nothing);
  return static_cast<Resource>(rnd::use(uniform));
}
