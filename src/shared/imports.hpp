#pragma once
#include <functional>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <cstdarg>
#include <cstdio>
#include <memory>
#include <random>
#include <mpi.h>

using std::optional;

template <typename Tp, typename Dp = std::default_delete<Tp>>
using uptr = std::unique_ptr<Tp, Dp>;

template <typename Tp>
using sptr = std::shared_ptr<Tp>;
using std::move;

typedef double f64;
typedef float f32;

typedef size_t usize;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
