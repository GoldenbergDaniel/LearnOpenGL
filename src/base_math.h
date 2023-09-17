#pragma once

#include "base_common.h"

// Base Math ===================================================================

#define PI 3.141592

typedef union Vec2F Vec2F;
union Vec2F
{
  struct
  {
    f32 x;
    f32 y;
  };

  f32 elements[2];
};

typedef union Vec3F Vec3F;
union Vec3F
{
  struct
  {
    f32 x;
    f32 y;
    f32 z;
  };

  f32 elements[3];
};

typedef union Vec4F Vec4F;
union Vec4F
{
  struct
  {
    union { f32 x; f32 r; };
    union { f32 y; f32 g; };
    union { f32 z; f32 b; };
    union { f32 w; f32 a; };
  };

  f32 elements[4];
};

typedef struct Mat3x3F Mat3x3F;
struct Mat3x3F
{
  f32 elements[3][3];
};

typedef struct Mat4x4F Mat4x4F;
struct Mat4x4F
{
  f32 elements[4][4];
};

// @Vector2 ====================================================================

#define V2F_ZERO (Vec2F) {0.0f, 0.0f}

Vec2F v2f(f32 x, f32 y);

Vec2F add_2f(Vec2F a, Vec2F b);
Vec2F sub_2f(Vec2F a, Vec2F b);
Vec2F mul_2f(Vec2F a, Vec2F b);
Vec2F div_2f(Vec2F a, Vec2F b);
f32 dot_2f(Vec2F a, Vec2F b);
f32 cross_2f(Vec2F a, Vec2F b);
Vec2F scale_2f(Vec2F v, f32 scale);

f32 magnitude_2f(Vec2F a);
f32 magnitude_squared_2f(Vec2F a);
f32 distance_2f(Vec2F a, Vec2F b);
f32 distance_squared_2f(Vec2F a, Vec2F b);
Vec2F normalize_2f(Vec2F a);

Vec2F lerp_2f(Vec2F curr, Vec2F target, f32 rate);

// @Vector3 ====================================================================

#define V3F_ZERO (Vec3F) {0.0f, 0.0f, 0.0f};
#define V2F_X (Vec2F) {1.0f, 0.0f, 0.0f}
#define V2F_Y (Vec2F) {0.0f, 1.0f, 0.0f}
#define V2F_Z (Vec2F) {0.0f, 0.0f, 1.0f}

Vec3F v3f(f32 x, f32 y, f32 z);

Vec3F add_3f(Vec3F a, Vec3F b);
Vec3F sub_3f(Vec3F a, Vec3F b);
Vec3F mul_3f(Vec3F a, Vec3F b);
Vec3F div_3f(Vec3F a, Vec3F b);
f32 dot_3f(Vec3F a, Vec3F b);
Vec3F cross_3f(Vec3F a, Vec3F b);
Vec3F scale_3f(Vec3F v, f32 scale);
Vec3F transform_3f(Vec3F v, Mat3x3F m);

f32 magnitude_3f(Vec3F v);
f32 magnitude_squared_3f(Vec3F v);
f32 distance_3f(Vec3F a, Vec3F b);
f32 distance_squared_3f(Vec3F a, Vec3F b);
Vec3F normalize_3f(Vec3F v);

Vec3F lerp_3f(Vec3F curr, Vec3F target, f32 rate);

// @Vector4 ====================================================================

#define V4F_ZERO (Vec4F) {0.0f, 0.0f, 0.0f, 0.0f};

Vec4F v4f(f32 x, f32 y, f32 z, f32 w);

Vec4F add_4f(Vec4F a, Vec4F b);
Vec4F sub_4f(Vec4F a, Vec4F b);
Vec4F mul_4f(Vec4F a, Vec4F b);
Vec4F div_4f(Vec4F a, Vec4F b);
f32 dot_4f(Vec4F a, Vec4F b);
Vec4F cross_4f(Vec4F a, Vec4F b);
Vec4F scale_4f(Vec4F v, f32 scale);
Vec4F transform_4f(Vec4F v, Mat4x4F m);

f32 magnitude_4f(Vec4F v);
f32 magnitude_squared_4f(Vec4F v);
f32 distance_4f(Vec4F a, Vec4F b);
f32 distance_squared_4f(Vec4F a, Vec4F b);
Vec4F normalize_4f(Vec4F v);

// @Matrix3x3 ==================================================================

Mat3x3F m3x3f(f32 k);

Mat3x3F rows_3x3f(Vec3F v1, Vec3F v2, Vec3F v3, Vec3F v4);
Mat3x3F cols_3x3f(Vec3F v1, Vec3F v2, Vec3F v3, Vec3F v4);
Mat3x3F diagonal_3x3f(f32 k);

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b);

Mat3x3F transpose_3x3f(Mat3x3F m);

Mat3x3F translate_3x3f(Vec2F v);
Mat3x3F rotate_3x3f(f32 angle);
Mat3x3F scale_3x3f(f32 x_scale, f32 y_scale);
Mat3x3F shear_3x3f(f32 x_shear, f32 y_shear);

void _print_3x3f(Mat3x3F matrix);

// @Matrix4x4 ==================================================================

Mat4x4F m4x4f(f32 k);

Mat4x4F rows_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4);
Mat4x4F cols_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4);
Mat4x4F diagonal_4x4f(f32 k);

Mat4x4F mul_4x4f(Mat4x4F a, Mat4x4F b);

Mat4x4F transpose_4x4f(Mat4x4F m);

Mat4x4F translate_4x4f(Vec3F v);
Mat4x4F rotate_4x4(f32 angle, Vec3F axis); // Should use quaternion
Mat4x4F scale_4x4f(f32 x_scale, f32 y_scale, f32 z_scale);
Mat4x4F shear_4x4f(f32 x_scale, f32 y_scale, f32 z_scale);

Mat4x4F orthographic_4x4f(f32 left, f32 right, f32 bot, f32 top);

void _print_4x4f(Mat4x4F matrix);
