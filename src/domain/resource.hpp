#pragma once
#include <shared/utils/common.hpp>

namespace resource {
    enum Resource { Alcohol, Snack, Nothing };

    extern fn random() -> Resource;
}
