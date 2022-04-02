#pragma once
#include <shared/imports.hpp>
#include <shared/utils/common.hpp>

namespace process {
    enum ExitCode { Success = 0, Failure = 1 };

    inline i32 Rank;
    inline i32 Size;

    fn finalize(ExitCode code) -> void;
    fn initialize(int argc, char **argv) -> void;

    extern fn is_poet(i32 rank) -> bool;
    extern fn is_volunteer(i32 rank) -> bool;
}
