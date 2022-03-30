#pragma once
#include <shared/imports.hpp>

extern i32 Rank;
extern i32 Size;

extern i32 Poets;
extern i32 Volunteers;
extern u8 Clock;

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
