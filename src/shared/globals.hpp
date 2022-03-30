#pragma once
#include <shared/imports.hpp>
#include <mpi.h>
#include <shared/utils/common.hpp>

i32 Rank;
i32 Size;

i32 Poets;
i32 Volunteers;
u8 Clock;

bool is_poet(i32 rank);
bool is_volunteer(i32 rank);

void tick();
void finalize(i32 errorcode);
void initialize(int argc, char **argv);

#define command(block) \
  do {                 \
    block;             \
    tick();            \
  } while (0)
