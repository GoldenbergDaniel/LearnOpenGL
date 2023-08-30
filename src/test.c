#include <stdio.h>

#include "../src/common.h"
#include "../src/math.h"

i32 main(void)
{
  Mat4x4F matrix =
  {
    {
      {0, 9, 9, 9},
      {1, 0, 8, 9},
      {1, 2, 0, 9},
      {1, 1, 1, 0}
    }
  };

  Mat4x4F result = transpose_4x4f(matrix);

  for (u32 i = 0; i < 4; i++)
  {
    for (u32 j = 0; j < 4; j++)
    {
      printf("%.0f ", result.elements[i][j]);
    }

    printf("\n");
  }

  return 0;
}
