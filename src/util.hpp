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

struct Mat4x4F
{
  Vec4F r1;
  Vec4F r2;
  Vec4F r3;
  Vec4F r4;
};

inline
Vec2F vec2f(f32 x, f32 y)
{
  return (Vec2F) {x, y};
}

inline
Vec4F vec4f(f32 x, f32 y, f32 z, f32 w)
{
  return (Vec4F) {x, y, z, w};
}

inline
Mat4x4F mat4x4f(Vec4F r1, Vec4F r2, Vec4F r3, Vec4F r4)
{
  return (Mat4x4F) {r1, r2, r3, r4};
}

// Other -----------------------------------------------------------------------

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
