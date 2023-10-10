#include <stdio.h>

#include "../src/base_common.h"
#include "../src/base_math.h"

#define DeferLoop(start, end) \
  for (int _i_ = ((start), 0); _i_ == 0; (_i_ += 1), (end))

i32 main(void)
{
  Mat3x3F sprite = scale_3x3f(1.0f, 1.0f);
  Mat3x3F camera = translate_3x3f(100.f, 100.0f);
  Mat3x3F projection = orthographic_3x3f(0.0f, 800.0f, 0.0f, 450.0f);

  return 0;
}
