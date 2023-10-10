// @Vertex ==================================================================================
#version 410 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
out vec3 color;

uniform mat3 u_xform;

void main()
{
  gl_Position = vec4(a_pos * u_xform, 1.0);
  color = a_color;
}

// @Fragment ================================================================================
#version 410 core

in vec3 color;
out vec4 frag_color;

void main()
{
  frag_color = vec4(color, 1.0);
}
