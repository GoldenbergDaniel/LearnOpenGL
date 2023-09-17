#include <stdio.h>

#include "../src/base_common.h"
#include "../src/base_math.h"

void print_mat3x3f(Mat3x3F matrix);
void print_mat4x4f(Mat4x4F matrix);

i32 main(void)
{
  Mat3x3F scale = scale_3x3f(2.0f, 4.0f);
  Mat3x3F shear = shear_3x3f(3.0f, 0.0f);
  Mat3x3F rotation = rotate_3x3f(90.0f);
  Mat3x3F translation = translate_3x3f(v2f(2.0f, 3.0f));
  
  Mat3x3F xform = mul_3x3f(mul_3x3f(mul_3x3f(translation, rotation), shear), scale);

  Vec3F position = {1.0f, 1.0f, 1.0f};
  position = transform_3f(position, xform);

  // printf("\n");
  // print_mat3x3f(xform);
  // printf("\n");

  Mat4x4F proj = orthographic_4x4f(0.0f, 800.0f, 0.0f, 450.0f);

  printf("\n");
  print_mat4x4f(proj);
  printf("\n");

  // for (u8 i = 0; i < 3; i++)
  // {
  //   printf("%.1f ", position.elements[i]);
  // }

  // printf("\n\n");

  return 0;
}

void print_mat3x3f(Mat3x3F matrix)
{
  for (u8 r = 0; r < 3; r++)
  {
    for (u8 c = 0; c < 3; c++)
    {
      printf("%.1f ", matrix.elements[r][c]);
    }

    printf("\n");
  }
}

void print_mat4x4f(Mat4x4F matrix)
{
  for (u8 r = 0; r < 4; r++)
  {
    for (u8 c = 0; c < 4; c++)
    {
      printf("%.4f ", matrix.elements[r][c]);
    }

    printf("\n");
  }
}
