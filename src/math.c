#include <stdio.h>

#include "common.h"
#include "math.h"

// Vector ----------------------------------------------------------------------

inline
Vec2F vec2f(f32 x, f32 y)
{
  return (Vec2F) {x, y};
}

inline
Vec3F vec3f(f32 x, f32 y, f32 z)
{
  return (Vec3F) {x, y, z};
}

inline
Vec4F vec4f(f32 x, f32 y, f32 z, f32 w)
{
  return (Vec4F) {x, y, z, w};
}

Vec3F transform_3f(Vec3F v, Mat3x3F transformation)
{
  Vec3F result;
  
  for (u8 c = 0; c < 3; c++)
  {
    result.x += transformation.elements[0][c] * v.elements[c];
    result.y += transformation.elements[1][c] * v.elements[c];
    result.z += transformation.elements[2][c] * v.elements[c];
  }

  return result;
}

Vec4F transform_4f(Vec4F v, Mat4x4F transformation)
{
  Vec4F result;
  
  for (u8 c = 0; c < 4; c++)
  {
    result.x += transformation.elements[0][c] * v.elements[c];
    result.y += transformation.elements[1][c] * v.elements[c];
    result.z += transformation.elements[2][c] * v.elements[c];
    result.w += transformation.elements[3][c] * v.elements[c];
  }

  return result;
}

// Matrix ----------------------------------------------------------------------

Mat3x3F mat3x3f(Vec3F v1, Vec3F v2, Vec3F v3, bool row_major)
{
  Mat3x3F result;

  if (row_major)
  {
    result = (Mat3x3F)
    {
      {
        {v1.x, v1.y, v1.z},
        {v2.x, v2.y, v2.z},
        {v3.x, v3.y, v3.z}
      }
    };
  }
  else
  {
    result = (Mat3x3F)
    {
      {
        {v1.x, v2.x, v3.x},
        {v1.y, v2.y, v3.y},
        {v1.z, v2.z, v3.z}
      }
    };
  }

  return result;
}

Mat4x4F mat4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4, bool row_major)
{
  Mat4x4F result;

  if (row_major)
  {
    result = (Mat4x4F)
    {
      {
        {v1.x, v1.y, v1.z, v1.w},
        {v2.x, v2.y, v2.z, v2.w},
        {v3.x, v3.y, v3.z, v3.w},
        {v4.x, v4.y, v4.z, v4.w}
      }
    };
  }
  else
  {
    result = (Mat4x4F)
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

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b)
{
  Mat3x3F result;

  for (u8 r = 0; r < 3; r++)
  {
    for (u8 c = 0; c < 3; c++)
    {
      result.elements[r][c] += a.elements[0][c] * b.elements[r][0];
      result.elements[r][c] += a.elements[1][c] * b.elements[r][1];
      result.elements[r][c] += a.elements[2][c] * b.elements[r][2];
    }
  }

  return result;
}

Mat4x4F mul_4x4f(Mat4x4F a, Mat4x4F b)
{
  Mat4x4F result;

  for (u8 r = 0; r < 4; r++)
  {
    for (u8 c = 0; c < 4; c++)
    {
      result.elements[r][c] += a.elements[0][c] * b.elements[r][0];
      result.elements[r][c] += a.elements[1][c] * b.elements[r][1];
      result.elements[r][c] += a.elements[2][c] * b.elements[r][2];
      result.elements[r][c] += a.elements[3][c] * b.elements[r][3];
    }
  }

  return result;
}

Mat3x3F diagonal_3x3f(f32 k)
{
  return (Mat3x3F)
  {
    {
      {k, 0, 0},
      {0, k, 0},
      {0, 0, k}
    }
  };
}

Mat4x4F diagonal_4x4f(f32 k)
{
  return (Mat4x4F)
  {
    {
      {k, 0, 0, 0},
      {0, k, 0, 0},
      {0, 0, k, 0},
      {0, 0, 0, k}
    }
  };
}

Mat3x3F transpose_3x3f(Mat3x3F m)
{
  Mat3x3F result = m;
  result.elements[0][1] = m.elements[1][0];
  result.elements[0][2] = m.elements[2][0];
  result.elements[1][0] = m.elements[0][1];
  result.elements[1][2] = m.elements[2][1];
  result.elements[2][0] = m.elements[0][2];
  result.elements[2][1] = m.elements[1][2];

  return result;
}

Mat4x4F transpose_4x4f(Mat4x4F m)
{
  Mat4x4F result = m;
  result.elements[0][1] = m.elements[1][0];
  result.elements[0][2] = m.elements[2][0];
  result.elements[0][3] = m.elements[3][0];
  result.elements[1][0] = m.elements[0][1];
  result.elements[1][2] = m.elements[2][1];
  result.elements[1][3] = m.elements[3][1];
  result.elements[2][0] = m.elements[0][2];
  result.elements[2][1] = m.elements[1][2];
  result.elements[2][3] = m.elements[3][2];
  result.elements[3][0] = m.elements[0][3];
  result.elements[3][1] = m.elements[1][3];
  result.elements[3][2] = m.elements[2][3];

  return result;
}

// NOTE: WIP
Mat4x4F translate_4x4f(Vec3F v)
{
  Mat4x4F result = diagonal_4x4f(1.0f);

  return result;
}
