#include <random>
#include <domain/resource.hpp>
#include <shared/imports.hpp>

using namespace resource;

extern fn resource::random() -> Resource {
  static std::random_device device;
  static std::mt19937 generator(device());
  static std::uniform_int_distribution<i32> uniform(Resource::Alcohol, Resource::Nothing);

  return static_cast<Resource>(uniform(generator));
}
