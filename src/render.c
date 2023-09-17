#include <stdio.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "base_common.h"
#include "base_math.h"
#include "render.h"

static void r_verify_shader(u32 id, GLenum type);

b8 _r_check_error(void)
{
  for (u32 err = -1; (err = glGetError());)
  {
    printf("[OpenGL Error]: %u\n", err);
    return TRUE;
  }

  return FALSE;
}

void _r_clear_error(void)
{
  while (glGetError() != GL_NO_ERROR);
}

// @Shader =====================================================================

R_Shader r_create_shader(const i8 *vert_src, const i8 *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, NULL);
  glCompileShader(vert);
  r_verify_shader(vert, GL_COMPILE_STATUS);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, NULL);
  glCompileShader(frag);
  r_verify_shader(frag, GL_COMPILE_STATUS);

  u32 id = glCreateProgram();
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);
  r_verify_shader(id, GL_LINK_STATUS);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (R_Shader) {id};
}

inline
void r_bind_shader(R_Shader *shader)
{
  R_ASSERT(glUseProgram(shader->id));
}

inline
void r_unbind_shader(void)
{
  glUseProgram(0);
}

i32 r_set_uniform_1u(R_Shader *shader, i8 *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, val);
  
  return loc;
}

i32 r_set_uniform_1(R_Shader *shader, i8 *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, val);
  
  return loc;
}

i32 r_set_uniform_1f(R_Shader *shader, i8 *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, val);
  
  return loc;
}

i32 r_set_uniform_2f(R_Shader *shader, i8 *name, Vec2F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, vec.x, vec.y);

  return loc;
}

i32 r_set_uniform_3f(R_Shader *shader, i8 *name, Vec3F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, vec.x, vec.y, vec.z);
  
  return loc;
}

i32 r_set_uniform_4f(R_Shader *shader, i8 *name, Vec4F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
  
  return loc;
}

i32 r_set_uniform_4x4f(R_Shader *shader, i8 *name, Mat4x4F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix4fv(loc, 1, FALSE, &mat.elements[0][0]);
  
  return loc;
}

static
void r_verify_shader(u32 id, GLenum type)
{
  i32 success;

  if (type == GL_LINK_STATUS)
  {
    glValidateProgram(id);
  }

  glGetShaderiv(id, type, &success);

  if (!success)
  {
    i32 length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    i8 log[length];
    glGetShaderInfoLog(id, length, &length, log);

    if (type == GL_COMPILE_STATUS)
    {
      printf("[GLObject Error]: Failed to compile shader!\n");
    }
    else
    {
      printf("[GLObject Error]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}

// @Buffer =====================================================================

R_Object r_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  return (R_Object) {id};
}

inline
void r_bind_vertex_buffer(R_Object *buffer)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
}

inline
void r_unbind_vertex_buffer()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

R_Object r_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return (R_Object) {id};
}

inline
void r_bind_index_buffer(R_Object *buffer)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id);
}

inline
void r_unbind_index_buffer()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// @VertexArray ================================================================

R_Object r_create_vertex_array(u8 attrib_count)
{
  u32 id;
  R_ASSERT(glGenVertexArrays(1, &id));

  return (R_Object) {id, attrib_count, 0};
}

inline
void r_bind_vertex_array(R_Object *vertex_array)
{
  R_ASSERT(glBindVertexArray(vertex_array->id));
}

inline
void r_unbind_vertex_array(void)
{
  glBindVertexArray(0);
}

R_Layout r_add_vertex_layout(R_Object *vertex_array, GLenum type, u32 count)
{
  u8 type_size;
  switch (type)
  {
    case GL_BYTE: {type_size = sizeof (i8);} break;
    case GL_SHORT: {type_size = sizeof (i16);} break;
    case GL_INT: {type_size = sizeof (i32);} break;
    case GL_FLOAT: {type_size = sizeof (f32);} break;
    default: type_size = 1;
  }

  R_Layout layout =
  {
    .index = vertex_array->attrib_index,
    .count = count,
    .data_type = type,
    .normalized = FALSE,
    .stride = count * vertex_array->attrib_count * type_size,
    .first = (void *) (u64) (vertex_array->attrib_index * count * type_size)
  };

  vertex_array->attrib_index++;

  return layout;
}

void r_set_vertex_layout(R_Object *vertex_array, R_Layout *layout)
{
  R_ASSERT(glVertexAttribPointer(
                                 layout->index,
                                 layout->count,
                                 layout->data_type,
                                 layout->normalized,
                                 layout->stride,
                                 layout->first));

  R_ASSERT(glEnableVertexAttribArray(layout->index));
}

// @Draw =======================================================================

void r_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void r_draw(R_Object *vertex_array, R_Shader *shader)
{
  r_bind_shader(shader);
  r_bind_vertex_array(vertex_array);
  R_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));
  // r_unbind_vertex_array();
  // r_unbind_shader();
}
