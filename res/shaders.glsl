// Vertex Shader
#version 410 core

layout (location = 0) in vec3 pos;
uniform vec2 u_pos;

void main()
{
  pos.x += u_pos.x;
  pos.y += u_pos.y;
  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
};

// Fragment Shader
#version 410 core

in vec3 color;
out vec4 frag_color;

void main()
{
  frag_color = vec4(color, 1.0);
};
