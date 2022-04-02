#include <random>
#include <domain/resource.hpp>
#include <shared/imports.hpp>

using namespace resource;

extern Resource resource::random() {
  static std::random_device device;
  static std::mt19937 generator(device());
  static std::uniform_int_distribution<i32> uniform(Resource::Alcohol, Resource::Nothing);

  return static_cast<Resource>(uniform(generator));
}
