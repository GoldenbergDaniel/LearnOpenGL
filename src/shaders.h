#include "base_common.h"

const i8 *v_shader_src = " \
#version 410 core \
layout (location = 0) in vec3 a_pos; \
layout (location = 1) in vec3 a_color; \
uniform mat4 u_proj; \
out vec3 color; \
void main() \
{ \
  gl_Position = vec4(a_pos, 1.0) * u_proj; \
  color = a_color; \
}";

const i8 *f_shader_src = " \
#version 410 core \
in vec3 color; \
out vec4 frag_color; \
void main() \
{ \
  frag_color = vec4(color, 1.0); \
}";
