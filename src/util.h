#pragma once

#include "common.h"

typedef struct Vec2F Vec2F;
struct Vec2F
{
  f32 x;
  f32 y;
};

typedef struct Vec3F Vec3F;
struct Vec3F
{
  f32 x;
  f32 y;
  f32 z;
};

typedef struct Vec4F Vec4F;
struct Vec4F
{
  union { f32 x; f32 r; };
  union { f32 y; f32 g; };
  union { f32 z; f32 b; };
  union { f32 w; f32 a; };
};

typedef struct Mat4F Mat4F;
struct Mat4F
{
  f32 elements[4][4];
};

#define MAJOR_R 0
#define MAJOR_C 1

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
Mat4F mat4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4, u8 major)
{
  Mat4F result = {};

  if (major == MAJOR_R)
  {
    result = (Mat4F)
    {
      {
        {v1.x, v1.y, v1.z, v1.w},
        {v2.x, v2.y, v2.z, v2.w},
        {v3.x, v3.y, v3.z, v3.w},
        {v4.x, v4.y, v4.z, v4.w}
      }
    };
  }
  else if (major == MAJOR_C)
  {
    result = (Mat4F)
    {
      {
        {v1.x, v2.x, v3.x, v4.x},
        {v1.y, v2.y, v3.y, v4.y},
        {v1.z, v2.z, v3.z, v4.z},
        {v1.w, v2.w, v3.w, v4.w}
      }
    };
  }

  return result;
}

// Other -----------------------------------------------------------------------

typedef struct String String;
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
