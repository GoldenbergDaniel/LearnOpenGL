#include <stdio.h>
#include <math.h>

#include "base_common.h"
#include "base_math.h"

// @Vector2 ====================================================================

inline
Vec2F v2f(f32 x, f32 y)
{
  return (Vec2F) {x, y};
}

inline
Vec2F add_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x + b.x, a.y + b.y};
}

inline
Vec2F sub_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x - b.x, a.y - b.y};
}

inline
Vec2F mul_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x * b.x, a.y * b.y};
}

inline
Vec2F div_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x / b.x, a.y / b.y};
}

inline
f32 dot_2f(Vec2F a, Vec2F b)
{
  return (a.x * b.x) + (a.y * b.y);
}

// NOTE: Could be wrong. Maybe should return a scalar
inline
Vec2F cross_2f(Vec2F a, Vec2F b)
{
  Vec2F result;
  result.x = (a.x * b.y);
  result.y = -(a.y * b.x);

  return result;
}

inline
Vec2F scale_2f(Vec2F v, f32 scale)
{
  return (Vec2F) {v.x * scale, v.y * scale};
}

inline
f32 magnitude_2f(Vec2F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));
}

inline
f32 magnitude_squared_2f(Vec2F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f);
}

inline
f32 distance_2f(Vec2F a, Vec2F b)
{
  Vec2F v = sub_2f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));
}

inline
f32 distance_squared_2f(Vec2F a, Vec2F b)
{
  Vec2F v = sub_2f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f);
}

inline
Vec2F normalize_2f(Vec2F v)
{
  return scale_2f(v, 1.0f / magnitude_2f(v));
}

inline
Vec2F lerp_2f(Vec2F curr, Vec2F target, f32 rate)
{
  return scale_2f(sub_2f(target, curr), rate);
}

// @Vector3 ====================================================================

inline
Vec3F v3f(f32 x, f32 y, f32 z)
{
  return (Vec3F) {x, y, z};
}

inline
Vec3F add_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline
Vec3F sub_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline
Vec3F mul_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline
Vec3F div_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x / b.x, a.y / b.y, a.z / b.z};
}

inline
f32 dot_3f(Vec3F a, Vec3F b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline
Vec3F cross_3f(Vec3F a, Vec3F b)
{
  Vec3F result;
  result.x = (a.y * b.z - a.z * b.y);
  result.y = -(a.x * b.z - a.z * b.x);
  result.z = (a.x * b.y - a.y * b.x);

  return result;
}

inline
Vec3F scale_3f(Vec3F v, f32 scale)
{
  return (Vec3F) {v.x * scale, v.y * scale, v.z * scale};
}

Vec3F transform_3f(Vec3F v, Mat3x3F m)
{
  Vec3F result;  
  for (u8 c = 0; c < 3; c++)
  {
    result.x += m.elements[0][c] * v.elements[c];
    result.y += m.elements[1][c] * v.elements[c];
    result.z += m.elements[2][c] * v.elements[c];
  }

  return result;
}

inline
f32 magnitude_3f(Vec3F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 magnitude_squared_3f(Vec3F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f);
}

inline
f32 distance_3f(Vec3F a, Vec3F b)
{
  Vec3F v = sub_3f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 distance_squared_3f(Vec3F a, Vec3F b)
{
  Vec3F v = sub_3f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f);
}

inline
Vec3F normalize_3f(Vec3F v)
{
  return scale_3f(v, 1.0f / magnitude_3f(v));
}

inline
Vec3F lerp_3f(Vec3F curr, Vec3F target, f32 rate)
{
  return scale_3f(sub_3f(target, curr), rate);
}

// @Vector4 ====================================================================

inline
Vec4F v4f(f32 x, f32 y, f32 z, f32 w)
{
  return (Vec4F) {x, y, z, w};
}

inline
Vec4F add_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline
Vec4F sub_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

inline
Vec4F mul_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

inline
Vec4F div_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

inline
f32 dot_4f(Vec4F a, Vec4F b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

// NOTE: Need to implement
inline
Vec4F cross_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {0};
}

inline
Vec4F scale_4f(Vec4F v, f32 scale)
{
  return (Vec4F) {v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

Vec4F transform_4f(Vec4F v, Mat4x4F m)
{
  Vec4F result;
  
  for (u8 c = 0; c < 4; c++)
  {
    result.x += m.elements[0][c] * v.elements[c];
    result.y += m.elements[1][c] * v.elements[c];
    result.z += m.elements[2][c] * v.elements[c];
    result.w += m.elements[3][c] * v.elements[c];
  }

  return result;
}

inline
f32 magnitude_4f(Vec4F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 magnitude_squared_4f(Vec4F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.z, 2.0f);
}

inline
f32 distance_4f(Vec4F a, Vec4F b)
{
  Vec4F v = sub_4f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f));
}

inline
f32 distance_squared_4f(Vec4F a, Vec4F b)
{
  Vec4F v = sub_4f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f);
}

inline
Vec4F normalize_4f(Vec4F v)
{
  return scale_4f(v, 1.0f / magnitude_4f(v));
}

// @Matrix3x3 ==================================================================

inline
Mat3x3F m3x3f(f32 k)
{
  return (Mat3x3F)
  {
    {
      {k, k, k},
      {k, k, k},
      {k, k, k},
    }
  };
}

inline
Mat3x3F rows_3x3f(Vec3F v1, Vec3F v2, Vec3F v3, Vec3F v4)
{
  return (Mat3x3F)
  {
    {
      {v1.x, v1.y, v1.z},
      {v2.x, v2.y, v2.z},
      {v3.x, v3.y, v3.z}
    }
  };
}

inline
Mat3x3F cols_3x3f(Vec3F v1, Vec3F v2, Vec3F v3,Vec3F v4)
{
  return (Mat3x3F)
  {
    {
      {v1.x, v2.x, v3.x},
      {v1.y, v2.y, v3.y},
      {v1.z, v2.z, v3.z}
    }
  };
}

inline
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

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b)
{
  Mat3x3F result = m3x3f(0.0f);

  for (u8 r = 0; r < 3; r++)
  {
    for (u8 c = 0; c < 3; c++)
    {
      result.elements[r][c] += b.elements[0][c] * a.elements[r][0];
      result.elements[r][c] += b.elements[1][c] * a.elements[r][1];
      result.elements[r][c] += b.elements[2][c] * a.elements[r][2];
    }
  }

  return result;
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

Mat3x3F translate_3x3f(Vec2F v)
{
  Mat3x3F result = diagonal_3x3f(1.0f);
  result.elements[0][2] = v.x;
  result.elements[1][2] = v.y;

  return result;
}

Mat3x3F rotate_3x3f(f32 angle)
{
  Mat3x3F result = m3x3f(0.0f);
  result.elements[0][0] = cos(angle * (PI / 180.0f));
  result.elements[0][1] = -sin(angle * (PI / 180.0f));
  result.elements[1][0] = sin(angle * (PI / 180.0f));
  result.elements[1][1] = cos(angle * (PI / 180.0f));
  result.elements[0][2] = 1.0f;
  result.elements[1][2] = 1.0f;
  result.elements[2][2] = 1.0f;

  return result;
}

Mat3x3F scale_3x3f(f32 scale)
{
  Mat3x3F result = diagonal_3x3f(scale);
  result.elements[2][2] = 1.0f;

  return result;
}

Mat3x3F shear_3x3f(f32 shear, Vec2F axis)
{
  Mat3x3F result = diagonal_3x3f(1.0f);
  result.elements[0][1] = shear * axis.x;
  result.elements[1][0] = shear * axis.y;

  return result;
}

// @Matrix4x4 ==================================================================

inline
Mat4x4F m4x4f(f32 k)
{
  return (Mat4x4F)
  {
    {
      {k, k, k, k},
      {k, k, k, k},
      {k, k, k, k},
      {k, k, k, k}
    }
  };
}

inline
Mat4x4F rows_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4)
{
  return (Mat4x4F)
  {
    {
      {v1.x, v1.y, v1.z, v1.w},
      {v2.x, v2.y, v2.z, v2.w},
      {v3.x, v3.y, v3.z, v3.w},
      {v4.x, v4.y, v4.z, v4.w}
    }
  };
}

inline
Mat4x4F cols_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4)
{
  return (Mat4x4F)
  {
    {
      {v1.x, v2.x, v3.x, v4.x},
      {v1.y, v2.y, v3.y, v4.y},
      {v1.z, v2.z, v3.z, v4.z},
      {v1.w, v2.w, v3.w, v4.w}
    }
  };
}

inline
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

Mat4x4F translate_4x4f(Vec3F v)
{
  Mat4x4F result = diagonal_4x4f(1.0f);
  result.elements[0][3] = v.x;
  result.elements[1][3] = v.y;
  result.elements[2][3] = v.z;

  return result;
}