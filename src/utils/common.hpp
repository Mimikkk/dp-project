#pragma once
#include "../imports.hpp"

using std::optional;
using std::vector;
using std::tuple;
using std::unique_ptr;
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
typedef char byte;
typedef unsigned char ubyte;

#define var auto
#define let const var
#define loop for(;;)
#define fn auto

inline fn str(const char *format, ...) {
  va_list arguments, size_check;
  va_start(arguments, format);
  va_copy(size_check, arguments);
  var size = vsnprintf(nullptr, 0, format, size_check);

  unique_ptr<char> buffer{new char[size + 1]};
  vsprintf(buffer.get(), format, arguments);
  va_end(arguments);
  return buffer;
}

inline fn str(const vector<i32> &items) {
  std::stringstream ss;
  bool first = true;
  for (auto item: items) {
    if (!first) ss << ", ";
    ss << item;
    first = false;
  }


  return str("[%s]", ss.str().c_str());
}

inline fn str(const tuple<i32, i32> pair) {
  return str("{%d, %d}", get<0>(pair), get<1>(pair));
}

inline fn str(const vector<tuple<i32, i32>> &pairs) {
  std::stringstream ss;
  bool first = true;
  for (auto pair: pairs) {
    if (!first) ss << ", ";
    ss << str(pair).get();
    first = false;
  }


  return str("[%s]", ss.str().c_str());
}
