#include <stdio.h>

#include "../src/base_common.h"
#include "../src/base_math.h"

#define DeferLoop(start, end) \
  for (int _i_ = ((start), 0); _i_ == 0; (_i_ += 1), (end))

i32 main(void)
{
  i32 a = 0;

  DeferLoop (a++, 5)
  {
    printf("Hello, world!\n");
  }

  printf("%i\n", a);

  return 0;
}
