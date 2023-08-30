#include "hmm/hmm.h"

#include "common.h"
#include "util.h"

Mat4F diagonal_4x4f(f32 k)
{
  return (Mat4F) 
  {
    {
      {k, 0, 0, 0},
      {0, k, 0, 0},
      {0, 0, k, 0},
      {0, 0, 0, k}
    }
  };
}

Vec2F mul_1f();
Vec2F mul_2f();
Vec2F mul_3f();
Vec2F mul_4f();
Vec2F mul_4x4f();

Mat4F translate_4x4f(Vec3F v)
{
  Mat4F result = diagonal_4x4f(1.0f);

  return result;
}

/*

  ax bx cx dx     vx
  ay by cy dy  X  vy
  az bz cz dz     vz
  aw bw cw dw     vw

  ax ay az aw
  bx by bz bw  X  vx vy vz vw
  cx cy cz cw
  dx dy dz dw

*/
