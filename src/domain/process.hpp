#pragma once
#include <shared/imports.hpp>
#include <shared/utils/common.hpp>

namespace process {
    enum ExitCode { Success = 0, Failure = 1 };

    inline i32 Rank;
    inline i32 Size;
    inline i32 Cooldown;
    inline MPI_Status Status;

    fn finalize(ExitCode code) -> void;
    fn initialize(i32 argc, char **argv) -> void;

    extern fn is_poet(i32 rank) -> bool;
    extern fn is_volunteer(i32 rank) -> bool;
}
