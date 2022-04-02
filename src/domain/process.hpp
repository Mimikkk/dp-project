#pragma once
#include <shared/imports.hpp>

namespace process {
    enum ExitCode { Success = 0, Failure = 1 };

    inline i32 Rank;
    inline i32 Size;

    void finalize(ExitCode code);
    void initialize(int argc, char **argv);

    extern bool is_poet(i32 rank);
    extern bool is_volunteer(i32 rank);
}
