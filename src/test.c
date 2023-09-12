#include <stdio.h>

#include "../src/base_common.h"
#include "../src/base_math.h"

void print_mat3x3f(Mat3x3F matrix);

i32 main(void)
{
  Mat3x3F scale = scale_3x3f(2.0f, 4.0f);
  Mat3x3F shear = shear_3x3f(3.0f, 0.0f);
  Mat3x3F rotation = rotate_3x3f(90.0f);
  Mat3x3F translation = translate_3x3f(v2f(2.0f, 3.0f));
  
  Mat3x3F result = mul_3x3f(mul_3x3f(mul_3x3f(translation, rotation), shear), scale);

  printf("\n");
  print_mat3x3f(result);
  printf("\n");

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
