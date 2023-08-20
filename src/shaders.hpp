#include "common.hpp"

const i8 *vert_shader_source = "\n"
  "#version 410 core\n"
  "layout (location = 0) in vec3 a_pos;\n"
  "layout (location = 1) in vec3 a_color;\n"
  "out vec3 color;\n"
  "void main()"
  "{\n"
  "  gl_Position = vec4(a_pos, 1.0);\n"
  "  color = a_color;\n"
  "}\0";

const i8 *frag_shader_source = "\n"
  "#version 410 core\n"
  "in vec3 color;\n"
  "out vec4 frag_color;\n"
  "void main()\n"
  "{\n"
  "  frag_color = vec4(color, 1.0);\n"
  "}\0";
