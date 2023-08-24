#pragma once

#include "common.hpp"

struct Vec2F
{
  f32 x;
  f32 y;
};

struct Vec3F
{
  f32 x;
  f32 y;
  f32 z;
};

struct Vec4F
{
  union { f32 x; f32 r; };
  union { f32 y; f32 g; };
  union { f32 z; f32 b; };
  union { f32 w; f32 a; };
};

inline
Vec4F vec4f(f32 x, f32 y, f32 z, f32 w)
{
  return (Vec4F) {x, y, z, w};
}

struct String
{
  i8 *data;
  u32 len;
};

inline
String str_lit(i8 *str)
{
  u32 len = 0;
  for (; !str[len]; len++);

  return (String) {str, len};
}
